#include "binary.h"

void Binary::onNewLine() {
}

void Binary::onNewChar(char value) {
  _current_value = _current_value | ( (value == '1') << (7 - _current_count++) );
  if (_current_count == 8) {
    write(_current_value);
    _current_count = 0;
    _current_value = 0;
  }
}

void Binary::write(char c)
{
  out().write(&c, 1);
  out().flush();
}