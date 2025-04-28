#include <array>
#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <deque>
#include <errno.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

// TODO: refactor the function to return the buffer instead of modifying global
// buf

using CmdVector = std::vector<char *>;

void handle_SIGTSTP(int sig) { return; }
enum ChainTypes {
  PIPE = 1,
  AND,
  OR,
  CHAIN,
};

struct Command {
  char *cmd;
  ChainTypes indicator;
};

class Forshell {
private:
  std::vector<char *> m_ParsedBuffer{};
  std::string m_InputBuffer{};
  std::array<std::string, 2> m_BuiltInCmd;
  std::deque<char *> m_CommandQueue{};
  std::deque<ChainTypes> m_ChainTypeQueue{};
  std::string prompt{"> "};
  int m_exitCode{};
  int m_histsize = 10;
  bool m_isPipe = false;
  bool m_isAND = false;
  bool m_isOR = false;
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
  CmdVector parse_args(std::string_view cmd) {
    std::vector<char *> parsed;
    for (int i = cmd.find_first_not_of(' '), pos = i; i < cmd.length(); i++) {
      char *c = const_cast<char *>(&cmd.at(i));
      if (i == pos) {
        parsed.push_back(c);
      }
      if (*c == ' ') {
        pos = i + 1;
        *c = '\0';
      }
    }
    parsed.push_back(nullptr);
    return parsed;
  };
  void parse_cmd(std::deque<char *> &m_CommandQueue,
                 std::deque<ChainTypes> &m_ChainTypeQueue) {
    bool foundAND = false;
    bool foundOR = false;
    ChainTypes indicator = CHAIN;
    for (int i = 0, pos = 0; i < m_InputBuffer.length(); i++) {
      char *c = &m_InputBuffer.at(i);
      if (i == pos) {
        m_CommandQueue.push_back(c);
        if (i != 0) {
          m_ChainTypeQueue.push_back(indicator);
        }
      }
      if (*c == ';') {
        pos = i + 1;
        *c = '\0';
        indicator = CHAIN;
      }
      if (*c == '|') {
        // BUG: pipe and or have the same first char (|), u really need to
        // refactor this
        if (m_InputBuffer[i + 1] == '|') {
          if (foundAND)
            return;
          pos = i + 1;
          indicator = OR;
          *c = '\0';
          foundOR = !foundOR;
          continue;
        }
        pos = i + 1;
        *c = '\0';
        indicator = PIPE;
      }
      if (*c == '&') {
        if (foundOR)
          return;
        if (foundAND) {
          pos = i + 1;
          indicator = AND;
        }
        *c = '\0';
        foundAND = !foundAND;
      }
      // TODO: syntax error when &| used
      // altho it should be piping std error, but i havent implement it so it
      // should be syntax error
      // TODO: change how we parse the line, maybe use string.find instead
    }
  };
  void shell_exec_external(std::vector<char *> &parsed) {
    pid_t pid = fork();
    if (pid == -1) {
      std::cerr << "fork failed" << std::endl;
      exit(1);
    }
    if (pid == 0) {
      // NOTE: child
      int errcode = execvp(parsed[0], parsed.data());
      if (errcode == -1) {
        if (errno == ENOENT) {
          std::cout << parsed[0] << ": command not found" << std::endl;
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
      // std::cout << "fsh: done deal" << std::endl;
    }
  }
  void shell_exec_with_pipes(std::vector<char *> &cmd,
                             std::vector<char *> &cmd2) {
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
  void shell_exec(std::vector<char *> &parsed) {
    if (parsed[0] == nullptr) {
      return;
    }
    std::string cmd = parsed[0];
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
    shell_exec_external(parsed);
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
    parse_cmd(m_CommandQueue, m_ChainTypeQueue);
    ChainTypes indicator;
    while (!m_CommandQueue.empty()) {
      CmdVector cmd = parse_args(m_CommandQueue.front());
      m_CommandQueue.pop_front();
      if (m_ChainTypeQueue.empty()) {
        shell_exec(cmd);
        break;
      }
      ChainTypes chain = m_ChainTypeQueue.front();
      m_ChainTypeQueue.pop_front();
      switch (chain) {
      case PIPE: {
        CmdVector cmd2 = parse_args(m_CommandQueue.front());
        m_CommandQueue.pop_front();
        shell_exec_with_pipes(cmd, cmd2);
        break;
      }
      case AND:
        shell_exec(cmd);
        if (m_exitCode != 0) {
          m_CommandQueue.pop_front();
        }
        break;
      case OR:
        shell_exec(cmd);
        if (m_exitCode == 0) {
          m_CommandQueue.pop_front();
        }
        break;
      case CHAIN:
        shell_exec(cmd);
        break;
      }
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
  // inputBuff
  // cmdQueue
  // get input()
  // parse cmd()
  // parsed = parse args(cmd)
  // exec(parsed)
}
