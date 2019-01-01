#include <iostream>
#include "usb_session.h"

#define ACM_REQUEST_SET_CONTROL_LINE_STATE 0x22
#define ACM_REQUEST_SET_LINE_CODING 0x20
#define ACM_CTRL_DTR 0x01
#define ACM_CTRL_RTS 0x02
#define EP_IN_ADDR 0x81

int main(int argc, char const *argv[])
{
  try {
    uint16_t vendor_id = 0x0483;
    uint16_t product_id = 0x5740;

    std::cout << "looking for devices...\n";

    usb_session session;
    std::list<usb_device> list = session.get_device_list();

    list.remove_if([vendor_id, product_id](usb_device dev) {
      return (dev.get_vendor_id() != vendor_id) || (dev.get_product_id() != product_id);
    });

    std::cout << list.size() << " device";
  
    if (list.size() != 1) {
      std::cout << "s";
    }
  
    std::cout << " found\n";

    if (list.size() > 1) {
      throw usb_exception("found more than one device");
    } else if (list.size() == 0) {
      throw usb_exception("no device found");
    }
    
    usb_device& dev = list.front();

    dev.open();
    std::cout << "device opened\n";
    dev.claim();
    std::cout << "device claimed\n";
    dev.control_transfer(0x21, ACM_REQUEST_SET_CONTROL_LINE_STATE, ACM_CTRL_DTR | ACM_CTRL_RTS, 0, NULL, 0, 0);
    unsigned char encoding[] = { 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08 };
    dev.control_transfer(0x21, ACM_REQUEST_SET_LINE_CODING, 0, 0, encoding, sizeof(encoding), 0);

    int buffer_size = 64;
    int transfer_size;
    unsigned char buffer_data[buffer_size];

    while (true) {
      try {
        dev.bulk_transfer(EP_IN_ADDR, buffer_data, buffer_size, &transfer_size, 1000);
        std::cout << "data received " << transfer_size << " bytes\n";
      } catch (const usb_exception& ex) {
        std::cerr << "usb exception: " << ex.what() << "\n";
      }
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }  

  return 0;
}
