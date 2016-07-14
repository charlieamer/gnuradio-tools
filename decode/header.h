#ifndef _HEADER_H
#define _HEADER_H

#include "decoder.h"

class Header : public Decoder {
  string _header;
  string _buffer;
  int _cur_pos;
  bool _writing;
  
  void init_buffer();
  
public:
  Header(boost::program_options::variables_map map);
  void onNewLine() override;
  void onNewChar(char value) override;
};

#endif