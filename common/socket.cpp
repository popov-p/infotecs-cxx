#include "socket.h"
#include "data-processor.h"

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


Socket::Socket(const std::string& socket_path) : _socket_path(socket_path), _sockfd(-1) {
  create();
}

ServerSocket::~ServerSocket() {
  if (_sockfd >= 0) {
    close();
  }
}

void Socket::create() {
  _sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (_sockfd < 0) {
    perror("Socket creation failed");
    throw std::runtime_error("Socket creation failed");
  }
}

bool ClientSocket::connect() {
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, _socket_path.c_str(), sizeof(addr.sun_path) - 1);

  const int max_retries = 5;
  const int retry_delay = 2;

  for (int i = 0; i < max_retries; ++i) {
    if (::connect(_sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
      std::cerr << "Не удалось подключиться, переподключение..." << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(retry_delay));
    }
    else {
      std::cout << "Соединение установлено." << std::endl;
      std::cout << "Введите строку (только цифры, до 64 символов): " << std::endl;
      std::cout.flush();
      return true;
    }

  }
  return false;
}

bool ServerSocket::connect() {
  struct sockaddr_un server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, _socket_path.c_str(), sizeof(server_addr.sun_path) - 1);

  unlink(server_addr.sun_path);

  if (bind(_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("Bind failed");
    close();
    return false;
  }

  if (listen(_sockfd, 5) < 0) {
    perror("Listen failed");
    close();
    return false;
  }
  return true;
};

bool ClientSocket::send(const std::string& data) {
  ssize_t bytes_sent = ::send(_sockfd, data.c_str(), data.size(), 0);
  if (bytes_sent < 0) {
    if (errno == EPIPE) {
      perror("Send failed");
      close();
      create();
      connect();
    }
    return false;
  }
  return true;
}

void Socket::close() {
  if (_sockfd >= 0) {
    ::close(_sockfd);
    _sockfd = -1;
  }
}

void ServerSocket::read() {
  while(true) {
    int client_sockfd = ::accept(_sockfd, nullptr, nullptr);
    if (client_sockfd < 0) {
      perror("Accept failed");
      return;
    }
    else {
      std::cout << "Cоединение установлено." << std::endl;
      while (true) {
        char buffer[256];
        ssize_t bytes_read = ::read(client_sockfd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
          buffer[bytes_read] = '\0';
          std::string data(buffer);
          DataProcessor::analyze(data);
        }
        else if (bytes_read == 0) {
          std::cout << "Соединение закрыто." << std::endl;
          ::close(client_sockfd);
          break;
        }
        else {
          std::cerr << "Error reading data from socket: " << strerror(errno) << std::endl;
          ::close(client_sockfd);
          break;
        }
      }
    }
  }
}
