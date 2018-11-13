extern crate libusb; // http://dcuddeback.github.io/libusb-rs/libusb/

use std::time::Duration;

fn main() {
    let vendor_id = 0x0483;
    let product_id = 0x5740;
    let version = libusb::version();
    let encoding = [0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08];
    let ACM_CTRL_DTR = 0x01;
    let ACM_CTRL_RTS = 0x02;
    let EP_IN_ADDR = 0x81;
    let EP_OUT_ADDR = 0x01;

    println!("Using libusb version {}.{}.{}.{}", version.major(), version.minor(), version.micro(), version.nano());
    
    let context = libusb::Context::new().unwrap();
    let devices_all = context.devices().unwrap();
    let devices_relevant = devices_all.iter().filter(|dev| {
        let device_desc = dev.device_descriptor().unwrap();

        device_desc.vendor_id() == vendor_id && device_desc.product_id() == product_id
    });

    for device in devices_relevant {
        let con_desc = device.active_config_descriptor().unwrap();
        let mut dev_hndl = device.open().unwrap();

        for interface in con_desc.interfaces() {
            if dev_hndl.kernel_driver_active(interface.number()).unwrap() {
                dev_hndl.detach_kernel_driver(interface.number()).unwrap();
            }

            dev_hndl.claim_interface(interface.number()).unwrap();
        }

        let dummy: [u8; 0] = [0; 0];

        dev_hndl.write_control(0x21, 0x20, ACM_CTRL_DTR | ACM_CTRL_RTS, 0, &dummy, Duration::from_secs(1)).unwrap();
        dev_hndl.write_control(0x21, 0x20, 0, 0, &encoding, Duration::from_secs(1)).unwrap();

        loop {
            let mut buf: [u8; 64] = [0; 64];
            
            match dev_hndl.read_bulk(EP_IN_ADDR, &mut buf, Duration::from_secs(1)) {
                Ok(size) => {
                    let strs: Vec<String> = buf[..size].iter().map(|b| format!("{:02X}", b)).collect();

                    println!("data: {}", strs.connect(" "));

                    let rssi = -((buf[0] as u8) as f32) / 2.0;
                    let temp = -45.0 + 175.0 * ((((buf[1] as u16) << 8) + buf[2] as u16) as f32) / 16_f32.exp2();
                    let hum = ((((buf[4] as u16) << 8) + buf[5] as u16) as f32) / 16_f32.exp2() * 100.0;
                    println!("rssi: {} dB, temp: {} °C, hum: {} %", rssi, temp, hum);
                },
                Err(error) => {
                    eprintln!("error: {}", error);
                }
            }
            
        }
    }
}
