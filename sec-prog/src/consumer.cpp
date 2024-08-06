#include "consumer.h"

void Consumer::start() {
  if(_socket->connect()) {
    ServerSocket* server_socket = dynamic_cast<ServerSocket*>(_socket.get());
    if (server_socket) {
      server_socket->read();
    }
  }
}
