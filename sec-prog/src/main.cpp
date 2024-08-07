#include "consumer.h"
#include <iostream>
#include <sys/un.h>
#include <csignal>

const std::string socket_path = "/tmp/internsocket";
Consumer cr(socket_path);

void signal_handler(int signum) {
  if (std::remove(socket_path.c_str()) != 0) {
    std::cerr << "Ошибка при удалении файла сокета: " << strerror(errno) << std::endl;
  }
  else {
    std::cerr << std::endl << "Завершение работы серевера." << std::endl;
    cr.stop();
    std::exit(signum);
  }
}

int main() {
  std::signal(SIGINT, signal_handler);
  cr.start();
  exit(EXIT_SUCCESS);
}
