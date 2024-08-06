#include "producer.h"
#include <iostream>
int main() {
  const std::string socket_path = "/home/pavelpopov/dev/infotecs-cxx/sock";
  Producer pr(socket_path);
  pr.start();
  pr.stop();
  return 0;
}
