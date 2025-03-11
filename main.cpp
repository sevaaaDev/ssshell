#include <array>
#include <cerrno>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

class Forshell {
private:
  std::vector<char *> m_ParsedBuffer{};
  std::string m_InputBuffer{};
  std::array<std::string, 2> m_BuiltInCmd;
  std::string cwd{};
  std::string prompt{"> "};
  void fsh_cd() {
    if (m_ParsedBuffer[1] != nullptr) {
      chdir(m_ParsedBuffer[1]);
      update_cwd();
    }
  };
  void fsh_exit() { exit(0); };
  void update_cwd() {
    // TODO: this seems overkill
    char *cwdir = getcwd(nullptr, 0);
    cwd = cwdir;
    free(cwdir);
    prompt = cwd + "> ";
  }

public:
  Forshell() : m_BuiltInCmd({"cd", "exit"}) {
    char *cwdir = getcwd(nullptr, 0);
    cwd = cwdir;
    free(cwdir);
    prompt = cwd + "> ";
  }
  void shell_getline() {
    std::cout << prompt;
    std::getline(std::cin, m_InputBuffer, '\n');
    if (std::cin.eof()) {
      std::cout << "\nexit" << std::endl;
      exit(0);
    }
  }
  void shell_parse() {
    for (int i = 0, pos = 0; i < m_InputBuffer.length(); i++) {
      char *c = &m_InputBuffer.at(i);
      if (i == pos) {
        m_ParsedBuffer.push_back(c);
      }
      if (*c == ' ') {
        pos = i + 1;
        *c = 0;
      }
    }
    m_ParsedBuffer.push_back(nullptr);
  }
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
        std::cout << strerror(errno) << std::endl;
        exit(2);
      }
    } else {
      // NOTE: parent
      wait(nullptr);
      std::cout << "fsh: done deal" << std::endl;
    }
  }
  void shell_exec() {
    std::string cmd = m_ParsedBuffer[0];
    if (cmd == "exit") {
      fsh_exit();
      return;
    }
    if (cmd == "cd") {
      fsh_cd();
      return;
    }
    shell_exec_external();
  }
  void shell_clearBuffer() {
    m_ParsedBuffer.clear();
    m_ParsedBuffer.shrink_to_fit();
    m_InputBuffer.clear();
    m_InputBuffer.shrink_to_fit();
  }
  void shell_loop() {
    shell_getline();
    shell_parse();
    shell_exec();
    shell_clearBuffer();
  }
};

// TODO:
// - cd
int main() {
  Forshell shell;
  while (true) {
    shell.shell_loop();
  }
  //
  // get input
  // parse
  // exec - output
}
