#include "aes_exception.h"

aes_exception::aes_exception(std::string description) {
  this->_description = description;
}

const char* aes_exception::what() const throw() {
    return this->_description.c_str();
}
