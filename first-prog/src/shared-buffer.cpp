#include "shared-buffer.h"

#include <iostream>


void SharedBuffer::write(const std::string& data) {
  {
    std::lock_guard<std::mutex> lock(mtx);
    buffer = data;
  }
  cv.notify_all();
}

std::string SharedBuffer::read() {
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait_for(lock, std::chrono::milliseconds(5), [this]() { return !buffer.empty(); });
  std::string data = buffer;
  buffer.clear();

  return data;
}

void SharedBuffer::clear() {
  std::lock_guard<std::mutex> lock(mtx);
  buffer.clear();
}
