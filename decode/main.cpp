#include <iostream>
#include <string>
#include <set>
#include <boost/program_options.hpp>
using namespace std;

#include "decoder.h"
#include "manchester.h"
#include "binary.h"
#include "header.h"
#include "bit3.h"

int main(int argc, char **argv) {
  boost::program_options::options_description description("Allowed options");
  description.add_options()
  ("help", "See help.")
  ("mode", boost::program_options::value<string>(), "Set decoding mode.")
  ("input", boost::program_options::value<string>(), "Set input type.")
  ("std", boost::program_options::value<string>(), "Set standard. (manchester decoding)")
  ("bpb", boost::program_options::value<int>(), "Set bits per byte (binary decoding)")
  ("format", boost::program_options::value<string>(), "Set output format (binary decoding)")
  ("invert", boost::program_options::value<string>(), "Should output be inverted (3bit decoding)")
  ("value", boost::program_options::value<string>(), "Set header value (header decoding)")
  ("output", boost::program_options::value<string>(), "Set output file.");
  
  boost::program_options::variables_map map;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), map);
  
  if (map.count("help")) {
    cout << "Usage: decode --mode [manchester | binary | header | 3bit] --input [binary | text]\n";
    cout << "  Mode specific options:\n";
    cout << "    manchester:\n";
    cout << "      --std [ieee802 | thomas]\n";
    cout << "        (which manchester standard to use, default is ieee802)\n";
    cout << "    binary:\n";
    cout << "      --bpb number\n";
    cout << "        (bits per byte, default is 8)\n";
    cout << "      --format binary|hex\n";
    cout << "        (output format, default is binary)\n";
    cout << "    header:\n";
    cout << "      --value header_value\n";
    cout << "        (bits denoting header, mendatory)\n";
    cout << "    3bit:\n";
    cout << "      --invert value\n";
    cout << "        (should or not output value be inverted)";
    return 0;
  }
  
  string mode = "binary";
  if (map.count("mode")) {
    mode = map["mode"].as<string>();
  }

  string input = "text";
  if (map.count("input")) {
    input = map["input"].as<string>();
  }
  
  Decoder *decoder = NULL;
  
  if (mode == "manchester") {
    decoder = new Manchester(map);
  } else if (mode == "binary") {
    decoder = new Binary(map);
  } else if (mode == "header") {
    decoder = new Header(map);
  } else if (mode == "3bit") {
    decoder = new Bit3(map);
  } else {
    cerr << "Invalid mode " << mode << ". See --help for list of available modes.";
    return -1;
  }
  
  while (true) {
    char tmp;
    cin.read(&tmp, 1);
    if (!cin.good())
      return 0;
    if (input == "text") {
      if (tmp == '\n')
        decoder->onNewLine();
      else
        decoder->onNewChar(tmp);
    } else if (input == "binary") {
      for (int i=0; i<8; i++) {
        decoder->onNewChar((tmp & (1 << i)) ? '1' : '0');
      }
    }
  }
    
  return 0;
}
