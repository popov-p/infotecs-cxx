#include "consumer.h"

#include <iostream>


void Consumer::start() {
  if(_socket->connect()) {
    _socket->read();
  }
}

void Consumer::stop() {
  _socket->close();
}
