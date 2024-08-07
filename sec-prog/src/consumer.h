#ifndef CONSUMER_H
#define CONSUMER_H

#include "../../common/socket.h"

#include <memory>


class Consumer {
public:
  Consumer(const std::string socket_path) : _socket(std::make_unique<ServerSocket>(socket_path)) {};
  void start();
  void stop();
private:
  std::unique_ptr<ServerSocket> _socket;
};

#endif // CONSUMER_H
