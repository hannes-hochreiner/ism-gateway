#include "usb_exception.h"

usb_exception::usb_exception(std::string description) {
  this->_description = description;
}

const char* usb_exception::what() const throw() {
    return this->_description.c_str();
}
