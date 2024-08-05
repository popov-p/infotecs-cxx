#ifndef PRODUCER_H
#define PRODUCER_H

#include "shared-buffer.h"
#include <thread>
#include <atomic>

class Producer {
public:
  Producer() = default;
  void start();
  void stop();
private:
  void read();
  void write();
  bool wait_for_input(int timeout_seconds);
  std::thread* _reader_thread;
  std::thread* _writer_thread;
  SharedBuffer _shared_buffer;
  std::atomic<bool> _stop_requested = false;
};

#endif // PRODUCER_H
