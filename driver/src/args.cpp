#include "args.h"

args::args(int argc, char const **argv) : _argc(argc), _argv(argv) {
}

args::~args() {
}

std::array<uint8_t, 16> args::get_key() {
  std::array<uint8_t, 16> res;

  for (int cntr = 1; (cntr + 1) < this->_argc; cntr += 2) {
    std::string tmp_arg(this->_argv[cntr]);
    if (tmp_arg == "-k") {
      std::string arg(this->_argv[cntr + 1]);

      if (arg.length() != 32) {
        throw std::runtime_error("key length must be 32");
      }

      for (int cntr_byte = 0; cntr_byte < 16; cntr_byte++) {
        res[cntr_byte] = (uint8_t)strtoul(arg.substr(cntr_byte * 2, 2).c_str(), nullptr, 16);
      }

      return res;
    }
  }

  throw std::runtime_error("key must be specified");
}
