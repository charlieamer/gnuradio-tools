#include "binary.h"

void Binary::onNewLine() {
  if (_format == "hex") {
    out() << endl;
  }
  _current_count = 0;
  _current_value = 0;
}

void Binary::onNewChar(char value) {
  _current_value = _current_value |
    ( (value == '1') << (7 - (_current_count++ % 8)) );
  if (_current_count == _bits_per_byte) {
    while (!_queue.empty()) {
      uchar tmp = _queue.front();
      _queue.pop();
      write(tmp);
    }
    write(_current_value >> (_bits_per_byte % 8));
    _current_count = 0;
    _current_value = 0;
  }
  if (_current_count == 8) {
    _queue.push(_current_value);
    _current_value = 0;
  }
}

void Binary::write(uchar c)
{
  if (_format == "binary") {
    out().write(reinterpret_cast<char*>(&c), 1);
  } else if (_format == "hex") {
    out() << std::hex << (unsigned)c << " ";
  }
  out().flush();
}

Binary::Binary(boost::program_options::variables_map map)
  : Decoder(map), _current_count(0), _current_value(0),
  _bits_per_byte(8), _format("binary") {
  if (map.count("bpb")) {
    _bits_per_byte = map["bpb"].as<int>();
  }
  if (map.count("format")) {
    _format = map["format"].as<string>();
  }
}