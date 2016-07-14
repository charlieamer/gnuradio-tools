#include "header.h"

Header::Header(boost::program_options::variables_map map):
  Decoder(map), _cur_pos(0)
{
  if (!map.count("value")) {
    throw invalid_argument("--value argument is required");
  }
  _header = map["value"].as<string>();
  init_buffer();
}

void Header::onNewChar(char value)
{
  if (_writing) {
    out() << value;
    out().flush();
  } else {
    _buffer[_cur_pos++] = value;
    int hlen = _header.length();
    _writing = true;
    for (int i=0; i<hlen; i++) {
      if (_buffer[(i + _cur_pos) % hlen] != _header[i]) {
	_writing = false;
	break;
      }
    }
    if (_cur_pos == hlen) {
      _cur_pos = 0;
    }
    if (_writing) {
      out() << _header;
      out().flush();
    }
  }
}

void Header::onNewLine()
{
  _cur_pos = 0;
  init_buffer();
  if (_writing) {
    out() << endl;
  }
  _writing = false;
}

void Header::init_buffer()
{
  _buffer.clear();
  for (int i=0; i<_header.length(); i++)
    _buffer.push_back('x');
}
