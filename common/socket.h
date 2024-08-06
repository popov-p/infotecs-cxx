#ifndef SOCKET_H
#define SOCKET_H

#include <string>

class Socket {
public:
  Socket(const std::string& socket_path);
  virtual ~Socket();
  virtual bool connect() = 0;
  void close();

protected:
  std::string _socket_path;
  int _sockfd;
};

class ClientSocket: public Socket {
public:
  ClientSocket(const std::string& socket_path): Socket(socket_path) {};
  bool connect() override;
  bool send(const std::string& data);
};

class ServerSocket : public Socket {
public:
  ServerSocket(const std::string& socket_path): Socket(socket_path) {};
  bool connect() override;
  void read();
};

#endif // SOCKET_H
