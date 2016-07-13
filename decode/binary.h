#ifndef _BINARY_H
#define _BINARY_H

#include "decoder.h"

class Binary : public Decoder {
  char _current_value;
  int _current_count;
public:
  Binary(boost::program_options::variables_map map) : Decoder(map), _current_count(0), _current_value(0) {}
  void onNewLine() override;
  void onNewChar(char value) override;
  void write(char c);
};

#endif