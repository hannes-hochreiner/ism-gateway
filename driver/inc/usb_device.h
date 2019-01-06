#ifndef __USB_DEVICE_H__
#define __USB_DEVICE_H__

#include <memory>
#include <iostream>
#include <libusb-1.0/libusb.h>
#include "usb_exception.h"

class usb_device
{
private:
  std::shared_ptr<int> _cntr;
  libusb_device* _dev = nullptr;
  libusb_device_handle* _hndl = nullptr;
  struct libusb_config_descriptor * _conf = nullptr;
  usb_device();
public:
  ~usb_device();
  usb_device(libusb_device* dev);
  uint16_t get_vendor_id();
  uint16_t get_product_id();
  void open();
  void claim();
  void control_transfer(uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout);
  void bulk_transfer(unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout);
  void clear_halt(unsigned char endpoint);
  void reset();
};

#endif
