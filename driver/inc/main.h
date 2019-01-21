#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <stdexcept>
#include <ctime>
#include "usb_session.h"
#include "aes_ecb.h"
#include "message.h"
#include "args.h"
#include "zmq.hpp"
#include "json.hpp"

using json = nlohmann::json;

#define ACM_REQUEST_SET_CONTROL_LINE_STATE 0x22
#define ACM_REQUEST_SET_LINE_CODING 0x20
#define ACM_CTRL_DTR 0x01
#define ACM_CTRL_RTS 0x02
#define EP_IN_ADDR 0x81

std::string format_message_1(const int16_t& rssi, message_0001_t const *const msg);
std::string format_message_2(const int16_t& rssi, message_0002_t const *const msg);
std::string mcu_id_to_hex(uint32_t mcu_id_1, uint32_t mcu_id_2, uint32_t mcu_id_3);
std::string sensor_id_to_hex(uint16_t sensor_id);
std::string get_timestamp();

#endif
