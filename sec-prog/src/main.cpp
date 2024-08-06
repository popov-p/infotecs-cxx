#include <iostream>
#include "consumer.h"

int main() {
  const std::string socket_path = "/home/pavelpopov/dev/infotecs-cxx/sock";
  Consumer cr(socket_path);
  return 0;
}
