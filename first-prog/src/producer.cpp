#include "producer.h"
#include "data-processor.h"
#include <sys/select.h>
#include <unistd.h>

void Producer::start() {
  _reader_thread = new std::thread(&Producer::read, this);
  _writer_thread = new std::thread(&Producer::write, this);
}

void Producer::stop() {
  for (const auto& thread: {_reader_thread, _writer_thread}) {
    if (thread->joinable())
      thread->join();
    delete thread;
  }
  std::cout << "DEBUG: ALL THREADS JOINED" << std::endl;
}

void Producer::read() {
  while (!_stop_requested) {
    std::cout << "Введите строку (только цифры, до 64 символов): " << std::endl;
    std::cout.flush();
    if (wait_for_input(2)) {
      std::string user_input;
      std::cin >> user_input;
      std::string result = DataProcessor::validate(user_input);
      if (!result.empty()) {
        _shared_buffer.write(result);
        std::cout << "Результат помещен в буфер. Ожидание следующего ввода..." << std::endl;
      }
    }
    else {
      std::cout << "DEBUG: TIMEOUT EXCEEDED. EXITING ..." << std::endl;
      _stop_requested = true;
      break;
    }
  }
}

void Producer::write() {
  while (!_stop_requested) {
    std::string data = _shared_buffer.read();
    _shared_buffer.clear();
    if (_stop_requested) {
        std::cout << "DEBUG: WRITER THREAD REQUESTED STOP, EXITING..." << std::endl;
        break;
    }
    if (!data.empty()) {
      std::cout << "Полученные данные: " << data << std::endl;
      std::cout << "Сумма чисел: " << DataProcessor::process(data) << std::endl;
      // IPC here
    }
  }
}

bool Producer::wait_for_input(int timeout_seconds) {
  fd_set read_fds;
  FD_ZERO(&read_fds);
  FD_SET(STDIN_FILENO, &read_fds);

  struct timeval timeout;
  timeout.tv_sec = timeout_seconds;
  timeout.tv_usec = 0;

  int result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
  return (result == 1);
}


