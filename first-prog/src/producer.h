#ifndef PRODUCER_H
#define PRODUCER_H

#include "shared-buffer.h"
#include "../../common/socket.h"
#include <thread>
#include <memory>

class Producer {
public:
  Producer(const std::string socket_path) : _socket(std::make_unique<ClientSocket>(socket_path)) {};
  void start();
  void stop();
private:
  void read();
  void write();
  bool wait_for_input(int timeout_seconds);
  std::unique_ptr<std::thread> _reader_thread;
  std::unique_ptr<std::thread> _writer_thread;
  SharedBuffer _shared_buffer;
  bool _stop_requested = false;
  std::unique_ptr<ClientSocket> _socket;
};

#endif // PRODUCER_H
