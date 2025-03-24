#include <array>
#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <errno.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

void handle_SIGTSTP(int sig) { return; }

class Forshell {
private:
  std::vector<char *> m_ParsedBuffer{};
  std::string m_InputBuffer{};
  std::array<std::string, 2> m_BuiltInCmd;
  std::vector<char *> m_CommandQueue{};
  std::string prompt{"> "};
  int m_exitCode{};
  int m_histsize = 10;
  std::vector<std::string> m_HistoryBuffer{};
  void fsh_cd() {
    if (m_ParsedBuffer[1] != nullptr) {
      chdir(m_ParsedBuffer[1]);
      update_cwd();
    }
  };
  void fsh_echo() {
    if (m_ParsedBuffer[1] != nullptr) {
      if (std::string_view(m_ParsedBuffer[1]) == "$?") {
        std::cout << m_exitCode << std::endl;
        return;
      }
      std::cout << m_ParsedBuffer[1] << std::endl;
    }
  }
  void fsh_exit() {
    std::cout << "\nexit" << std::endl;
    exit(0);
  };
  void fsh_history() {
    if (m_ParsedBuffer[1] == nullptr) {
      for (int i = 0; i < m_HistoryBuffer.size() - 1; i++) {
        std::cout << "  " << i << "  " << m_HistoryBuffer[i] << std::endl;
      }
    }
  }
  void update_cwd() {
    std::unique_ptr<char> cwdir(getcwd(nullptr, 0));
    prompt = std::string(cwdir.get()) + "> ";
  }
  void add_history() {
    if (m_HistoryBuffer.size() > m_histsize) {
      m_HistoryBuffer.erase(m_HistoryBuffer.begin());
    }
    m_HistoryBuffer.push_back(m_InputBuffer);
  }
  std::string expandString(std::string &string) {
    if (string == "!!") {
      return m_HistoryBuffer.back();
    }
    if (string[0] == '!') {
      try {
        int n = std::stoi(&string[1]);
        return m_HistoryBuffer[n];
      } catch (std::invalid_argument const &err) {
        std::cerr << "fsh: " << err.what() << std::endl;
      }
    }
    return string;
  }

public:
  Forshell() : m_BuiltInCmd({"cd", "exit"}) { update_cwd(); }
  void shell_getline() {
    std::cout << prompt;
    std::getline(std::cin, m_InputBuffer, '\n');
    if (std::cin.eof()) {
      fsh_exit();
    }
    int pos = m_InputBuffer.find_first_not_of(' ');
    m_InputBuffer.erase(0, pos);
    m_InputBuffer = expandString(m_InputBuffer);
    add_history();
  }
  void shell_parse() {
    shell_parse_cmd();
    for (auto &cmd : m_CommandQueue) {
      shell_parse_args(cmd);
    }
  }
  void shell_parse_args(std::string_view cmd) {
    for (int i = cmd.find_first_not_of(' '), pos = i; i < cmd.length(); i++) {
      char *c = const_cast<char *>(&cmd.at(i));
      if (i == pos) {
        m_ParsedBuffer.push_back(c);
      }
      if (*c == ' ') {
        pos = i + 1;
        *c = '\0';
      }
    }
    // TODO: remove leading whitespace
    m_ParsedBuffer.push_back(nullptr);
  };
  void shell_parse_cmd() {
    for (int i = 0, pos = 0; i < m_InputBuffer.length(); i++) {
      char *c = &m_InputBuffer.at(i);
      if (i == pos) {
        m_CommandQueue.push_back(c);
      }
      if (*c == ';') {
        pos = i + 1;
        *c = '\0';
      }
    }
  };
  void shell_exec_builtin() {}
  void shell_exec_external() {
    pid_t pid = fork();
    if (pid == -1) {
      std::cerr << "fork failed" << std::endl;
      exit(1);
    }
    if (pid == 0) {
      // NOTE: child
      int errcode = execvp(m_ParsedBuffer[0], m_ParsedBuffer.data());
      if (errcode == -1) {
        if (errno == ENOENT) {
          std::cout << m_ParsedBuffer[0] << ": command not found" << std::endl;
          exit(127);
        } else {
          std::cout << strerror(errno) << std::endl;
          exit(126);
        }
      }
    } else {
      // NOTE: parent
      int status;
      pid_t wpid = waitpid(-1, &status, 0);
      if (WIFEXITED(status)) {
        m_exitCode = WEXITSTATUS(status);
      }
      if (WIFSIGNALED(status)) {
        m_exitCode = WTERMSIG(status) + 128;
      }
      std::cout << "fsh: done deal" << std::endl;
    }
  }
  void shell_exec_with_pipes() {
    int pipefd[2];
    int pipeerr = pipe(pipefd);
    if (pipeerr == -1) {
      exit(2);
    }
    pid_t pid1 = fork();
    if (pid1 == -1) {
      exit(1);
    };
    if (pid1 == 0) {
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[0]);
      // TODO: exec first cmd
    } else {
      // parent
      pid_t pid2 = fork();
      if (pid2 == -1) {
        exit(1);
      }
      if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        // TODO: exec second cmd
      } else {
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, nullptr, 0);
        waitpid(pid2, nullptr, 0);
        // TODO: handle exit code;
      }
    }
  }
  void shell_exec() {
    if (m_ParsedBuffer[0] == nullptr) {
      return;
    }
    std::string cmd = m_ParsedBuffer[0];
    if (cmd == "exit") {
      fsh_exit();
      return;
    }
    if (cmd == "cd") {
      fsh_cd();
      return;
    }
    if (cmd == "echo") {
      fsh_echo();
      return;
    }
    if (cmd == "history") {
      fsh_history();
      return;
    }
    shell_exec_external();
  }
  void shell_clearBuffer() {
    m_ParsedBuffer.clear();
    m_ParsedBuffer.shrink_to_fit();
    m_InputBuffer.clear();
    m_InputBuffer.shrink_to_fit();
    m_CommandQueue.clear();
    m_CommandQueue.shrink_to_fit();
  }
  void shell_loop() {
    shell_getline();
    shell_parse_cmd();
    for (auto cmd : m_CommandQueue) {
      shell_parse_args(cmd);
      shell_exec();
      m_ParsedBuffer.clear();
      m_ParsedBuffer.shrink_to_fit();
    }
    shell_clearBuffer();
  }
};

// TODO:
// - cd
int main() {
  signal(SIGTSTP, handle_SIGTSTP);
  Forshell shell;
  while (true) {
    shell.shell_loop();
  }
  //
  // get input
  // parse
  // exec - output
}

int exec_program(std::vector<char *> cmd) {
  int errcode = execvp(cmd[0], cmd.data());
  if (errno == ENOENT) {
    return 127;
  }
  return 126;
}

/*
 if (piping) {
 shell_exec_with_pipes()
 }
 else {
 shell_exec_external()
 }
*/
