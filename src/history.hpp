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
  std::deque<char> prev();
  std::deque<char> next();
  void clear();
  void push(std::deque<char> line);
  void resetIndex();
  int index();

private:
  int maxHist_ = 5;
  // index \[[-1, getSize() - 1\]]
  int index_ = -1;
  std::deque<std::deque<char>> queue_;
};

#endif // !HISTORY_HPP
