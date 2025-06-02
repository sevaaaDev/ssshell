#ifndef HISTORY_HPP
#define HISTORY_HPP
#include <deque>
#include <string>
class History {
  int getSize();
  int getMaxSize();
  /* return full list of history */
  const std::deque<std::string> &getHistory();
  std::string getPrevLine(int nth);
  void clear();
  void push(std::string_view line);

private:
  int maxHist_ = 5;
  std::deque<std::string> queue_;
};

#endif // !HISTORY_HPP
