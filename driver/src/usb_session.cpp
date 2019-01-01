#include "usb_session.h"

usb_session::usb_session()
{
  libusb_init(&this->_ctx);
}

usb_session::~usb_session()
{
  libusb_exit(this->_ctx);
}

std::list<usb_device> usb_session::get_device_list() {
  std::list<usb_device> out;
  libusb_device** list_ptr;
  ssize_t count;

  count = libusb_get_device_list(this->_ctx, &list_ptr);

  for (ssize_t cntr = 0; cntr < count; cntr++) {
    out.push_back(usb_device(list_ptr[cntr]));
  }

  libusb_free_device_list(list_ptr, 1);

  return out;
}
