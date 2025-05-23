#include "executioner_utils.hpp"
#include "lexer.hpp"
void Exec::waitAll(Executioner &executioner) {
  while (executioner.getPsCount()) {
    executioner.reap();
  }
}
bool Exec::execTree(Executioner &executioner, Node &node) {
  if (node.token->type == TKN_STRING) {
    auto cmd = parser::getArgs(node.token->string);
    int keepLoop = executioner.exec(cmd[0], cmd);
    return keepLoop;
  }
  if (node.token->type == TKN_PIPE) {
    // setup the pipe fd
    executioner.setupPipeFD(node.children.size() - 1);
    auto fdPairs = executioner.getPipeFDs();
    int keepLoop = 1;
    {
      int i = 0;
      for (Node &child : node.children) {
        auto cmd = parser::getArgs(child.token->string);
        keepLoop = executioner.exec(cmd[0], cmd, fdPairs[i]);
        if (!keepLoop)
          break;
        i++;
      }
    }
    executioner.closePipeFD();
    executioner.wait();
    waitAll(executioner);
    return keepLoop;
  }
  bool run = execTree(executioner, node.children[0]);
  executioner.wait();
  if (run == false) {
    return run;
  }
  if (node.token->type == TKN_SEMICOLON) {
    return execTree(executioner, node.children[1]);
  }
  if (node.token->type == TKN_AND && executioner.getExitCode() == 0) {
    return execTree(executioner, node.children[1]);
  }
  if (node.token->type == TKN_OR && executioner.getExitCode() == 1) {
    return execTree(executioner, node.children[1]);
  }
  executioner.wait();
  return run;
}
