# gnuradio-tools
A collection of tools that help add custom functionality more easily (via UDP) to gnuradio.

# Components
This project currently consists of 4 components -
* *gnuradio_udp_server* - This is base library. If you want to add custom functionality to gnuradio, you should extend a main class in this library.
* *freq2text* - this is actually a zvei3 decoder *(you can modify the frequencies and corresponding characters in order to make other seltome decoder)* - prints decoded numbers to stdut. **Depends on gnuradio_udp_server**.
* *bitPrinter* - this receives raw bits **at baud rate** and prints them to stdout. Can be easily used with *decode* project in order to save bits in raw binary format. **Depends on gnuradio_udp_server**.
* *decode* - this is independent project that aims on easily decoding various encoded data and outputing it on stdout so it can be piped. Currently it can decode only Manchester and Binary (binary is converting textual 1s and 0s to ascii).

# Example
```sh
bitprinter 9600 192000 | decode --mode manchester | decode --mode binary --output output.bin
```
This command will listen for raw bits on port 1234 (this is configured in source code of bitprinter), it will expect bits to be changed at 9600 times per second (baud rate) and it will expect data from gnuradio at rate of 192000 times per second (sample rate). The output is piped to decode executable which will decode manchaster encoded data. That output is piped to binary decoder which will collect textual 1s and 0s to ascii value and store that value to output.bin, and you can then open that file in hex viewer, do hexdump, or whatever.

# Setting up GnuRadio
It's pretty straightforward to setup gnuradio to send to your program. All you need is a udp sink and that's it (it works on both default udp sink and baz build udp sink).

![gnuradio example](https://github.com/charlieamer/gnuradio-tools/blob/master/images/gnuradio.png?raw=true)

**Important note:** You have to set payload size to something size of your data is divisible with. For example, in this case we are sending floats (which is 32bit, or 4 bytes), and 1472 is divisible by 4 (1472 is default value so it works fine).

*Note:* Send Null Pkt has no effect on udp server yet, i am planning on doing it in future.
# Building
## Requiraments
Boost libraries (not really sure which exact dev package is needed, so i am mentioning here all) and Cmake.
On ubuntu:
```sh
sudo apt-get install libboost-all-dev cmake
```
Optional, if it is easier for you, you can use cmake-gui
```sh
sudo apt-get install cmake-gui
```
## gnuradio_udp_server
```sh
cd gnuradio_udp_server
mkdir build
cd build
cmake ..
make
sudo make install
```

## freq2text
```sh
cd freq2text
mkdir build
cd build
cmake ..
make
sudo make install
```
## bitprinter
```sh
cd bitPrinter
mkdir build
cd build
cmake ..
make
sudo make install
```
## decode
```sh
cd decode
mkdir build
cd build
cmake ..
make
sudo make install
```
## Building notes
*Note:* On my machine sudo is not required for make install, and i prefer to install stuff without sudo where possible so you try it for yourself.

# Building your own functionality
Let's say you are building your own class that extends gnuradio_udp_server, you have two options: with cmake and without it. Without it is less complicated and gets you up and running in no time, while cmake requires a bit more configuration.

## main.cpp
Let's say your class is called foo and it will receive data as stream of floats. **Important note:** I suggest you pass sample rate as well, let's say from command line, because from gnuradio you will not receive any info regarding 

```cpp
#include <server.h>

class Foo : public Server<float>  {
public:
    Foo(boost::asio::io_service &service, short port) :
        Server<float>(service, port) {
        // init your stuff here...
        mainLoop(); // THIS CALL IS MANDATORY.
    }
    // The onReceive method will be automatically called
    // when there is something received from gnuradio.
    void onReceive(vector<float> values) override {
        // do your stuff here...
    }
}

int main() {
    boost::asio::io_service service;
    Foo foo(service, 1234);
}
```
## Compiling without cmake
```sh
g++ main.cpp -lboost_system -lboost_thread -lgnuradio_udp_server -lrt -pthread
```

## Compiling with cmake
Too lazy maybe some other time. Essentially, you can copy bitPrinter/CMakeLists.txt to your project directory, change project name (line 2) and roll off from there.
