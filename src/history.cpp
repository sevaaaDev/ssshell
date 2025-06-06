#include "history.hpp"

int History::getSize() { return queue_.size(); }
int History::getMaxSize() { return maxHist_; }

const std::deque<std::deque<char>> &History::getHistory() { return queue_; };

std::deque<char> History::at(int nth) {
  int base = 0;
  if (nth < 0) {
    base = getSize();
  }
  int index = base + nth;
  if (index < 0)
    index = 0;
  return queue_[index];
};
std::deque<char> History::prev() {
  if (getSize() == 0) {
    return {};
  }
  if (++index_ >= getSize()) {
    index_ = getSize() - 1;
  }
  return queue_[index_];
};

std::deque<char> History::next() {
  if (--index_ < 0) {
    index_ = -1;
    return {};
  }
  return queue_[index_];
};

int History::index() { return index_; };
void History::resetIndex() { index_ = -1; };
void History::clear() { queue_.clear(); };
void History::push(std::deque<char> line) {
  queue_.push_front(line);
  if (getSize() > maxHist_) {
    queue_.pop_back();
  }
  resetIndex();
};
