#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#include <string>
#include <mutex>
#include <condition_variable>

class SharedBuffer {
public:
  SharedBuffer() = default;
  void write(const std::string& data);
  std::string read();
  void clear();
private:
  std::string buffer;
  std::mutex mtx;
  std::condition_variable cv;
};

#endif // SHARED_BUFFER_H
