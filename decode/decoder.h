#ifndef _DECODER_H
#define _DECODER_H

#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
using namespace std;

class Decoder {
  boost::program_options::variables_map _map;
  ostream *_out;
public:
  Decoder(boost::program_options::variables_map map);
  ~Decoder();
  
  ostream& out();
  virtual void onNewChar(char value) = 0;
  virtual void onNewLine() = 0;
};

#endif