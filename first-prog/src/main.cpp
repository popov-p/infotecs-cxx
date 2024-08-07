#include "producer.h"
#include <iostream>
#include <csignal>

int main() {
  struct sigaction sa;
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGPIPE, &sa, nullptr) == -1) {
    perror("Sigaction");
    exit(EXIT_FAILURE);
  }

  const std::string socket_path = "/tmp/internsocket";
  Producer pr(socket_path);
  pr.start();
  pr.stop();
  exit(EXIT_SUCCESS);
}
