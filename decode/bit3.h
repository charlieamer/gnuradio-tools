#ifndef _BIT3_H
#define _BIT3_H

#include "decoder.h"

class Bit3 : public Decoder {
  char _current_queue[3];
  int _current_index;
  bool _invert;
public:
  Bit3(boost::program_options::variables_map map);
  void onNewLine() override;
  void onNewChar(char value) override;
};

#endif