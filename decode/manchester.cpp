#include "manchester.h"

Manchester::Manchester(boost::program_options::variables_map map): Decoder(map),
_ieee(true), _last_char(0)
{
  if (map.count("std")) {
    string val = map["std"].as<string>();
    if (val == "ieee802") {
      _ieee = true;
    } else if(val == "thomas") {
      _ieee = false;
    } else {
      throw invalid_argument("Invalid std argument. Expected either thomas or ieee802");
    }
  }
}

void Manchester::onNewLine() {
  out() << endl;
}

void Manchester::onNewChar(char value) {
  if (value != '0' && value != '1')
    return;
  if (_last_char != 0) {
    if (_last_char == value) {
      out() << "?";
    } else {
      out() << ((_last_char == '1') ^ _ieee);
    }
    _last_char = 0;
    out().flush();
  } else {
    _last_char = value;
  }
}