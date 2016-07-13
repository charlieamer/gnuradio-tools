#include <iostream>
#include <server.h>

typedef unsigned char uchar;

class BitServer : public Server<uchar> {
  int _bit_rate;
  int _samp_rate;
  int _last_value;
  int _count;
  bool _last_print_endl;
public:
  BitServer(boost::asio::io_service& service, int bit_rate, int sample_rate) :
    Server(service, 1234), _bit_rate(bit_rate), _samp_rate(sample_rate),
    _last_value(0), _count(0), _last_print_endl(true)
  {
    /*cout << "Bit rate:    " << bit_rate << endl;
    cout << "Sample rate: " << sample_rate << endl;*/
    mainLoop();
  }
protected:
  void onReceive(vector<uchar> values) {
    for (uchar val : values) {
      if (val == _last_value) {
	_count++;
      } else {
	if (_count < _samp_rate / 600) {
	  int print_count = round((float)_count / ((float)_samp_rate / (float)_bit_rate));
	  for (int i=0;i<print_count;i++) {
	    cout << _last_value;
	    cout.flush();
	    _last_print_endl = false;
	  }
	} else {
	  if (!_last_print_endl) {
	    cout << endl;
	    _last_print_endl = true;
	  }
	}
	_last_value = val;
	_count = 0;
      }
    }
  }
};

int main(int argc, char **argv) {
  boost::asio::io_service service;
  int bit_rate = 9600;
  int samp_rate = 48000;
  if (argc >= 2)
    bit_rate = atoi(argv[1]);
  if (argc >= 3)
    samp_rate = atoi(argv[2]);
  BitServer server(service, bit_rate, samp_rate);
  service.run();
}
