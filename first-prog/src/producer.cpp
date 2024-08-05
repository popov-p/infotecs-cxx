#include "producer.h"
#include "data-processor.h"

void Producer::start() {
  _reader_thread = new std::thread(&Producer::read, this);
  _writer_thread = new std::thread(&Producer::write, this);
}

void Producer::stop() {
  _stop_requested = true;
  for (const auto& thread: {_reader_thread, _writer_thread}) {
      if (thread->joinable())
        thread->join();
      delete thread;
  }
}

void Producer::read() {
  while (!_stop_requested) {
    std::string user_input;
    std::cout << "Введите строку (только цифры, до 64 символов): ";
    std::cin >> user_input;
    if (_stop_requested)
      break;

    std::string result = DataProcessor::validate(user_input);

    if (!result.empty()) {
      _shared_buffer.write(result);
      std::cout << "Результат помещен в буфер. Ожидание следующего ввода..." << std::endl;
    }
  }
}

void Producer::write() {
  while (!_stop_requested) {
    std::string data = _shared_buffer.read();
    _shared_buffer.clear();

    if (!data.empty()) {
      std::cout << "Полученные данные: " << data << std::endl;

      std::cout << "Сумма чисел: " << DataProcessor::process(data) << std::endl;
          // IPC here
    }
  }
}
