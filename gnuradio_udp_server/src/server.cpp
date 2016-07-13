#include "../include/server.h"
#include <iostream>
using namespace std;

template<typename T>
Server<T>::Server(boost::asio::io_service& io_service, short port) :
_socket(io_service, udp::endpoint(udp::v4(), port)),
_io_service(io_service),
_mq(boost::interprocess::open_or_create, "queue", 65535, 65535),
_network_thread(&Server<T>::initReceive, this)
{
  total_recvd = 0;
  work = true;
}

template<typename T>
void Server<T>::mainLoop()
{
  char buffer[65535];
  while (work) {
    size_t bytes_recvd;
    unsigned int priority;
    _mq.receive(buffer, 65535, bytes_recvd, priority);
    updateProgressbar();
    onReceive(asValue(buffer, bytes_recvd));
    total_recvd += (long long) bytes_recvd;
  }
}

template<typename T>
void Server<T>::initReceive() {
  while (work) {
    size_t len = _socket.receive_from(boost::asio::buffer(data, 65535), sender_endpoint);
    _mq.send(data, len, 0);
    updateProgressbar();
  }
}

template<typename T>
vector<T> Server<T>::asValue(char* data, int bytes) {
  vector<T> ret;
  T *arr = (T*)data;
  bytes/=sizeof(T);
  for (T* ptr = arr; ptr-arr < bytes; ptr++) {
    ret.push_back(*ptr);
  }
  return ret;
}

template<typename T>
void Server<T>::onError(const boost::system::error_code& error) {
  cout << "Error on receive: " << error << endl;
}

template<typename T>
void Server<T>::updateProgressbar() {
  _print_mutex.lock();
  //cout << "\r" << _mq.get_num_msg() << "/" << _mq.get_max_msg();
  //cout.flush();
  _print_mutex.unlock();
}

template class Server<int>;
template class Server<float>;
template class Server<double>;
template class Server<char>;
template class Server<unsigned char>;
template class Server<complex<float> >;