#ifndef HISTORY_HPP
#define HISTORY_HPP
#include <deque>
class History {
public:
  int getSize();
  int getMaxSize();
  /* return full list of history */
  const std::deque<std::deque<char>> &getHistory();
  std::deque<char> at(int nth);
  void clear();
  void push(std::deque<char> line);

private:
  int maxHist_ = 5;
  std::deque<std::deque<char>> queue_;
};

#endif // !HISTORY_HPP
