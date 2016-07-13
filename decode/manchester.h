#ifndef _MANCHESTER_H
#define _MANCHESTER_H

#include "decoder.h"

class Manchester : public Decoder {
  bool _ieee;
  char _last_char;
public:
  Manchester(boost::program_options::variables_map map);
  void onNewLine() override;
  void onNewChar(char value) override;
};

#endif