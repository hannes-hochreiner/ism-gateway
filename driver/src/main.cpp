#include "main.h"

int main(int argc, char const *argv[])
{
  try {
    args args(argc, argv);

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
    aes_ecb aes(args.get_key());

    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);

    publisher.bind("ipc://sensor-net.ipc");

    while (true) {
      try {
        dev.bulk_transfer(EP_IN_ADDR, buffer_data, buffer_size, &transfer_size, 1000);
        std::cout << "received " << transfer_size << " bytes\n";

        if (transfer_size % 4 != 1) {
          throw std::runtime_error("inconsistent data length");
        }

        unsigned char buffer_decrypt_data[transfer_size - 1];

        aes.decrypt((buffer_data + 1), buffer_decrypt_data, transfer_size - 1);

        if (buffer_decrypt_data[0] == 1) {
          message_0001_t* msg = (message_0001_t*)(buffer_decrypt_data);
          float rssi = (float)(buffer_data[0]) / 2;

          std::string msg_json = format_message_1(rssi, msg);
          std::cout << msg_json << "\n";
          zmq::message_t msg_zmq(msg_json.c_str(), msg_json.size());
          
          publisher.send(msg_zmq);
        } else {
          std::cout << "unknown message type received\n";
        }
      } catch (const usb_exception& ex) {
        std::cerr << "usb exception: " << ex.what() << "\n";
      }
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }  

  return 0;
}

std::string format_message_1(const float& rssi, message_0001_t const *const msg) {
  json j;

  j["rssi"] = rssi;
  j["timestamp"] = get_timestamp();
  j["message"]["type"] = msg->type;
  j["message"]["mcu_id"] = mcu_id_to_hex(msg->mcu_id_1, msg->mcu_id_2, msg->mcu_id_3);
  j["message"]["message_index"] = msg->message_index;
  j["message"]["sensor_id"] = sensor_id_to_hex(msg->sensor_id);
  j["message"]["humidity"] = msg->humidity;
  j["message"]["temperature"] = msg->temperature;

  return j.dump();
}

std::string get_timestamp() {
  time_t now;
  time(&now);
  char buf[sizeof("YYYY-MM-DDTHH:MM:SSZ")];
  strftime(buf, sizeof(buf), "%FT%TZ", gmtime(&now));

  return std::string(buf);
}

std::string sensor_id_to_hex(uint16_t sensor_id) {
  std::ostringstream os;

  os.width(4);
  os.fill('0');
  os << std::hex << std::uppercase << sensor_id;

  return os.str();
}

std::string mcu_id_to_hex(uint32_t mcu_id_1, uint32_t mcu_id_2, uint32_t mcu_id_3) {
  std::ostringstream os;

  os.width(8);
  os.fill('0');
  os << std::hex << std::uppercase << mcu_id_3 << "-" << mcu_id_2 << "-" << mcu_id_1;

  return os.str();
}
