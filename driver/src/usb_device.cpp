#include "usb_device.h"

usb_device::usb_device(libusb_device* dev)
{
  this->_cntr = std::make_shared<int>();
  this->_dev = libusb_ref_device(dev);
}

usb_device::~usb_device()
{
  if (this->_cntr.use_count() == 1) {
    if (this->_conf != nullptr) {
      for (uint cntr = 0; cntr < this->_conf->bNumInterfaces; cntr++) {
        libusb_release_interface(this->_hndl, cntr);
      }
    }

    if (this->_hndl != nullptr) {
      libusb_close(this->_hndl);
    }
  
    if (this->_dev != nullptr) {
      libusb_unref_device(this->_dev);
    }
  }
}

uint16_t usb_device::get_vendor_id() {
  struct libusb_device_descriptor desc;

  libusb_get_device_descriptor(this->_dev, &desc);
  return desc.idVendor;
}

uint16_t usb_device::get_product_id() {
  struct libusb_device_descriptor desc;

  libusb_get_device_descriptor(this->_dev, &desc);
  return desc.idProduct;
}

void usb_device::open() {
  if (libusb_open(this->_dev, &this->_hndl) != LIBUSB_SUCCESS) {
    throw usb_exception("could not open device");
  }

  if (libusb_set_auto_detach_kernel_driver(this->_hndl, 1) != LIBUSB_SUCCESS) {
    throw usb_exception("could not set auto detach");
  }
}

void usb_device::claim() {
  if (libusb_get_active_config_descriptor(this->_dev, &this->_conf) != LIBUSB_SUCCESS) {
    throw usb_exception("could not get configuration");
  }

  for (uint cntr = 0; cntr < this->_conf->bNumInterfaces; cntr++) {
    if (libusb_claim_interface(this->_hndl, cntr) != LIBUSB_SUCCESS) {
      throw usb_exception("could not claim interface");
    }
  }
}

void usb_device::control_transfer(uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout) {
  int res = libusb_control_transfer(this->_hndl, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);

  if (res >= 0) {
    return;
  }

  switch (res) {
    case LIBUSB_ERROR_TIMEOUT: throw usb_exception("control transfer failed: timeout");
    case LIBUSB_ERROR_PIPE: throw usb_exception("control transfer failed: request not supported");
    case LIBUSB_ERROR_NO_DEVICE: throw usb_exception("control transfer failed: device disconnected");
    default: throw usb_exception("control transfer failed");
  }
}

void usb_device::bulk_transfer(unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout) {
  int res = libusb_bulk_transfer(this->_hndl, endpoint, data, length, transferred, timeout);

  if (res >= 0) {
    return;
  }

  switch (res) {
    case LIBUSB_ERROR_TIMEOUT: throw usb_exception("bulk transfer failed: timeout");
    case LIBUSB_ERROR_PIPE: throw usb_exception_pipe();
    case LIBUSB_ERROR_NO_DEVICE: throw usb_exception("bulk transfer failed: device disconnected");
    case LIBUSB_ERROR_OVERFLOW: throw usb_exception("bulk transfer failed: overflow");
    default: throw usb_exception("bulk transfer failed");
  }
}

void usb_device::clear_halt(unsigned char endpoint) {
  int res = libusb_clear_halt(this->_hndl, endpoint);

  if (res >= 0) {
    return;
  }

  switch (res) {
    case LIBUSB_ERROR_NOT_FOUND: throw usb_exception("clear halt failed: endpoint not found");
    case LIBUSB_ERROR_NO_DEVICE: throw usb_exception("clear halt failed: device disconnected");
    default: throw usb_exception("clear halt failed");
  }
}

void usb_device::reset() {
  int res = libusb_reset_device(this->_hndl);

  if (res >= 0) {
    return;
  }

  switch (res) {
    case LIBUSB_ERROR_NO_DEVICE: throw usb_exception("reset failed: device disconnected");
    default: throw usb_exception("reset failed");
  }
}
