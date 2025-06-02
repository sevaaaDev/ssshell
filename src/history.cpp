#include "history.hpp"

int History::getSize() { return queue_.size(); }
int History::getMaxSize() { return maxHist_; }

const std::deque<std::string> &History::getHistory() { return queue_; };

std::string History::getPrevLine(int nth) {
  if (getSize() == 0 || nth == 0)
    return "";
  int index = getSize() - nth;
  if (index < 0)
    index = 0;
  return queue_[index];
};

void History::clear() { queue_.clear(); };
void History::push(std::string_view line) {
  if (getSize() == 0 || queue_[getSize() - 1] != line) {
    queue_.push_back(std::string(line));
  }
  if (getSize() > maxHist_) {
    queue_.pop_front();
  }
};
