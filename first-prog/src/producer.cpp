#include "producer.h"
#include "../../common/data-processor.h"

#include <sys/select.h>
#include <unistd.h>


void Producer::start() {
  if(_socket->connect()) {
    _reader_thread = std::make_unique<std::thread>(&Producer::read, this);
    _writer_thread = std::make_unique<std::thread>(&Producer::write, this);
  }
}

void Producer::stop() {
  for (const auto& thread: {_reader_thread.get(), _writer_thread.get()}) {
    if (thread && thread->joinable())
      thread->join();
  }
  std::cout << "DEBUG: ALL THREADS JOINED" << std::endl;
}

void Producer::read() {
  while (!_stop_requested) {
    if (wait_for_input(10)) {
      std::string user_input;
      std::cin >> user_input;
      std::string result = DataProcessor::validate(user_input);
      if (!result.empty()) {
        _shared_buffer.write(result);
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
      std::cout << "Получена строка: " << data << std::endl;
      std::cout << "Сумма чисел: " << DataProcessor::process(data) << std::endl;
      _socket->send(data);
    }
  }
}

bool Producer::wait_for_input(int timeout_seconds) { /* Таймаут добавлен для красоты */
  fd_set read_fds;
  FD_ZERO(&read_fds);
  FD_SET(STDIN_FILENO, &read_fds);

  struct timeval timeout;
  timeout.tv_sec = timeout_seconds;
  timeout.tv_usec = 0;

  int result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
  return (result == 1);
}


