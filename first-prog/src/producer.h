#ifndef PRODUCER_H
#define PRODUCER_H

#include "shared-buffer.h"
#include <thread>

class Producer {
public:
  Producer() = default;
  void start();
  void stop();
private:
  void read();
  void write();
  std::thread* _reader_thread;
  std::thread* _writer_thread;
  SharedBuffer _shared_buffer;
  bool _stop_requested = false;
};

#endif // PRODUCER_H
