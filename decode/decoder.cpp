#include "decoder.h"

Decoder::Decoder(boost::program_options::variables_map map) : _map(map) {
  _out = NULL;
  if (map.count("output")) {
    string fname = map["output"].as<string>();
    if (fname != "-") {
      _out = new ofstream(fname.c_str());
      if (_out == NULL || _out->bad()) {
	throw runtime_error("Error opening file for writing: " + fname);
      }
    }
  }
  if (_out == NULL) {
    _out = &cout;
  }
}

Decoder::~Decoder()
{
  ofstream *tmp = static_cast<ofstream*>(_out);
  if (tmp != NULL) {
    tmp->close();
  }
  delete tmp;
}

ostream& Decoder::out()
{
  return *_out;
}
