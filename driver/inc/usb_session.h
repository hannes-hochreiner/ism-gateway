#ifndef __USB_SESSION_H__
#define __USB_SESSION_H__

#include <list>
#include "usb_device.h"
#include <libusb-1.0/libusb.h>

class usb_session
{
private:
  libusb_context* _ctx;
public:
  usb_session();
  ~usb_session();
  std::list<usb_device> get_device_list();
};

#endif
