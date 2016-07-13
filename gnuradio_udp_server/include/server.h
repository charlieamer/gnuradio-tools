#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <vector>
#include <complex>

using boost::asio::ip::udp;
using namespace std;

template<typename T>
class Server {
public:
  Server(boost::asio::io_service&, short);
protected:
  void mainLoop();
  udp::socket _socket;
  udp::endpoint sender_endpoint;
  boost::asio::io_service& _io_service;
  char data[65535];
  
  bool work;
  
  unsigned long long total_recvd;
  
  void initReceive();
  vector<T> asValue(char *data, int bytes);
  
  boost::mutex _print_mutex;
  boost::interprocess::message_queue _mq;
  boost::thread _network_thread;
  void updateProgressbar();
  
  virtual void onReceive(vector<T> values) = 0;
  virtual void onError(const boost::system::error_code& error);
};