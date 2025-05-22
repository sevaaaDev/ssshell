#ifndef EXEC_UTILS_HPP
#define EXEC_UTILS_HPP
#include "executioner.hpp"
#include "parser.hpp"
namespace Exec {
void waitAll(Executioner &executioner);
bool execTree(Executioner &executioner, Node &node);
} // namespace Exec

#endif // ! EXEC_UTILS_HPP
