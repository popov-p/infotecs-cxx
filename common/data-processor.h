#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

class DataProcessor {
public:
  static std::string validate(const std::string& input) {
    if (input.empty() || input.size() > 64 || !std::all_of(input.begin(), input.end(), ::isdigit)) {
      std::cerr << "Некорректный ввод. Строка должна состоять только из цифр и не превышать 64 символа." << std::endl;
      return "";
    }

    std::string sorted_input = input;
    std::sort(sorted_input.begin(), sorted_input.end(), std::greater<char>());

    std::string result;
    for (char c : sorted_input) {
      if ((c - '0') % 2 == 0)
        result += "КВ";
      else
        result += c;
    }
    return result;
  }

  static int process(const std::string& input) {
    int sum = 0;
    std::stringstream ss(input);
    char ch;
    while (ss >> ch) {
      if (isdigit(ch))
        sum += ch - '0';
    }
    return sum;
  }

  static void analyze(const std::string& data) {
    if (data.length() > 2) {
      if (data.length() % 32 == 0)
        std::cout << "Получены данные: " << data << std::endl;
      else
        std::cout << "Ошибка: длина данных не кратна 32." << std::endl;
    }
    else
      std::cout << "Ошибка: данные содержат меньше 3 символов." << std::endl;
  }
};

#endif // DATA_PROCESSOR_H
