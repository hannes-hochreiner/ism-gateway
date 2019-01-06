#ifndef __USB_EXCEPTION_H__
#define __USB_EXCEPTION_H__

#include <exception>
#include <string>

class usb_exception: public std::exception {
private:
  std::string _description;
public:
  usb_exception(std::string description);
  virtual const char* what() const throw();
};

class usb_exception_pipe: public usb_exception {
public:
  usb_exception_pipe();
};

#endif
