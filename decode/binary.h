#ifndef _BINARY_H
#define _BINARY_H

#include "decoder.h"
#include <queue>
using namespace std;

typedef unsigned char uchar;

class Binary : public Decoder {
  uchar _current_value;
  int _current_count;
  int _bits_per_byte;
  queue<uchar> _queue;
  string _format;
public:
  Binary(boost::program_options::variables_map map);
  void onNewLine() override;
  void onNewChar(char value) override;
  void write(uchar c);
};

#endif