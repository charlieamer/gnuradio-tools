#include <iostream>
#include <string>
#include <set>
#include <boost/program_options.hpp>
using namespace std;

#include "decoder.h"
#include "manchester.h"
#include "binary.h"

int main(int argc, char **argv) {
  boost::program_options::options_description description("Allowed options");
  description.add_options()
  ("help", "See help.")
  ("mode", boost::program_options::value<string>(), "Set decoding mode.")
  ("std", boost::program_options::value<string>(), "Set standard. (manchester decoding)")
  ("output", boost::program_options::value<string>(), "Set output file.");
  
  boost::program_options::variables_map map;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), map);
  
  if (map.count("help")) {
    cout << "Usage: decode --mode [manchester | binary]\n";
    cout << "  Mode specific options:\n";
    cout << "    manchester:\n";
    cout << "      --std [ieee802 | thomas]\n";
    cout << "        (default is ieee802)\n";
    return 0;
  }
  
  string mode = "binary";
  if (map.count("mode")) {
    mode = map["mode"].as<string>();
  }
  
  Decoder *decoder = NULL;
  
  if (mode == "manchester") {
    decoder = new Manchester(map);
  } else if (mode == "binary") {
    decoder = new Binary(map);
  } else {
    cout << "Invalid mode " << mode << ". See --help for list of available modes.";
    return -1;
  }
  
  while (true) {
    char tmp;
    cin.read(&tmp, 1);
    if (!cin.good())
      return 0;
    if (tmp == '\n')
      decoder->onNewLine();
    else
      decoder->onNewChar(tmp);
  }
    
  return 0;
}
