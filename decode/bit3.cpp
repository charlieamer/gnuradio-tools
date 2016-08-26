#include "bit3.h"

Bit3::Bit3(boost::program_options::variables_map map) :
  Decoder(map), _invert(false), _current_index(0) {
  if (map.count("invert")) {
    _invert = map["invert"].as<bool>();
  }
}

void Bit3::onNewLine() {
  out() << endl;
  _current_index = 0;
}

void Bit3::onNewChar(char value) {
  _current_queue[(_current_index++ % 3)] = value;
  if (_current_index > 2) {
    if (_current_queue[_current_index % 3] == '1' &&
        _current_queue[(_current_index + 2) % 3] == '0') {
      out() << ((_current_queue[(_current_index + 1) % 3] == '1') ^ _invert);
      _current_index = 0;
    }
  }
}