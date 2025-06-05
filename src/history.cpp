#include "history.hpp"

int History::getSize() { return queue_.size(); }
int History::getMaxSize() { return maxHist_; }

const std::deque<std::deque<char>> &History::getHistory() { return queue_; };

std::deque<char> History::getHistoryLine(int nth) {
  int base = 0;
  if (nth < 0) {
    base = getSize();
  }
  if (getSize() == 0 || nth == 0)
    return {};
  int index = base + nth;
  if (index < 0)
    index = 0;
  return queue_[index];
};

void History::clear() { queue_.clear(); };
void History::push(std::deque<char> line) {
  queue_.push_back(line);
  if (getSize() > maxHist_) {
    queue_.pop_front();
  }
};
