//
// async_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <server.h>

#include <iostream>
#include <map>
#include "biquad.h"
using namespace std;

map<int, char> alphabet = {{2400, '0'}, {1060, '1'}, {1160, '2'}, {1270, '3'},
{1400, '4'}, {1530, '5'}, {1670, '6'}, {1830, '7'}, {2000, '8'}, {2200, '9'},
{2800, 'A'}, {810, 'B'}, {970, 'C'}, {885, 'D'}, {2600, 'E'}, {680, 'F'}
};

#define SQR(x) ((x)*(x))

class FrequencyServer : public Server<float> {
  float *_values;
  float *_averages;
  int _index;
  int _buf_size;
  int _sample_rate;
  int _duration;
  int _pausing;
  Biquad _biquad;
  int _written;
  char _last_char;
  float get_value(int index) {
    while (index < 0)
      index += _buf_size;
    return _values[(_index + index) % _buf_size];
  }
  void push_value(float value) {
    _values[_index] = value;
    _averages[_index] = 0.0f;
    for (int i=0; i<4; i++) {
      _averages[_index] += get_value(-i);
    }
    _averages[_index] /= 4.0;
    _averages[_index] = _biquad.process(_averages[_index]);
    if (++_index == _buf_size) {
      _index = 0;
    }
  }
  
  float calculate_frequency(float* array, float &frequency, float &deviation, vector<float> &crossings) {
    bool positive = true;
    crossings.clear();
    int crossingSum = 0;
    float lastTime = 0;
    float lastVal = 0;
    float audioSum = 0;
    for (int i=0; i<_buf_size; i++) {
      float sum = array[(i + _index) % _buf_size];
      if (sum > 0 && !positive) {
	float diff = sum - lastVal;
	float correction = 1.0 - sum / diff;
	float currentTime = i + correction;
	if (lastTime != 0) {
	  crossings.push_back(currentTime - lastTime);
	  crossingSum += currentTime - lastTime;
	}
	lastTime = currentTime;
      }
      positive = sum > 0;
      lastVal = sum;
      audioSum += fabs(sum);
    }
    audioSum /= (float)_buf_size;
    deviation = 0;
    float crossingAvg = (float)(crossingSum) / (float)(crossings.size());
    for (float cross : crossings) {
      deviation += SQR(cross - crossingAvg);
    }
    deviation /= (float)crossings.size();
    frequency = _sample_rate / ((crossingSum) / ((float)crossings.size()));
    return audioSum;
  }
  
  void check() {
    float frequency, deviation;
    vector<float> crossings;
    float volume = calculate_frequency(_averages, frequency, deviation, crossings);
    if (deviation < 7.0f && frequency > 200 && crossings.size() > 50) {
      _print_mutex.lock();
      //cout << "\r";
      int closest = 0;
      
      //cout << "crossings: " << crossings.size() << "\tdeviation: " << deviation << "\tfrequency: ";
      //cout << frequency << endl;
      
      for (auto& kv : alphabet) {
	if (closest == 0 || abs(kv.first - frequency) < abs(frequency - closest)) {
	  closest = kv.first;
	}
      }
      if (abs(closest - frequency) < 300) {
	_written++;
	if (alphabet[closest] == 'E') {
	  cout << _last_char;
    if (_written % 5 < 3)
        _written = 2;
  }
	else {
	  cout << alphabet[closest];
	  _last_char = alphabet[closest];
	}
	cout.flush();
	if (_written % 5 == 0)
	  cout << endl;
      }
      //cout << closest << " " << frequency << endl;
      _pausing = _buf_size;
      _print_mutex.unlock();
    }
    //if (crossings.size() < 30 || deviation > 1000)
    //  _pausing = _buf_size;
    if (_pausing == 0)
      _pausing = _sample_rate / 1000;
  }
public:
  ~FrequencyServer() {
    delete[] _values;
  }
  FrequencyServer(boost::asio::io_service &service, int sample_rate, int duration) :
      Server<float>(service, 1234),
      _biquad(bq_type_notch, 120.0 / (float)sample_rate, 1, 0) {
    _buf_size = duration * sample_rate / 1000 + 1;
    _values = new float[_buf_size];
    _averages = new float[_buf_size];
    _index = 0;
    _sample_rate = sample_rate;
    _duration = duration;
    _pausing = 0;
    _written = 0;
    _last_char = 0;
    cout << "Sample rate: " << sample_rate << endl;
    cout << "Duration:    " << duration << "ms" << endl;
    cout << "Buffer size: " << _buf_size << endl;
    mainLoop();
  }
  
  void onReceive(vector<float> values) override {
    for(auto val : values) {
      push_value(val);
      if (_pausing > 0) {
	_pausing--;
      } else {
	check();
      }
    }
  }
};

int main(int argc, char* argv[])
{
  boost::asio::io_service service;
  int samp_rate = 48000;
  if (argc >= 2)
    samp_rate = atoi(argv[1]);
  FrequencyServer server(service, samp_rate, 69);
}