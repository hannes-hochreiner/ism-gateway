extern crate libusb;

fn main() {
    let version = libusb::version();

    println!("Using libusb version {}.{}.{}.{}", version.major(), version.minor(), version.micro(), version.nano());
    
    let context = libusb::Context::new().unwrap();

    for device in context.devices().unwrap().iter() {
        let device_desc = device.device_descriptor().unwrap();
        let is_custom = if device_desc.vendor_id() == 0x0483 && device_desc.product_id() == 0x5740 {
            true
        } else {
            false
        };

        println!("Bus {:03} Device {:03} ID {:04x}:{:04x} {}",
            device.bus_number(),
            device.address(),
            device_desc.vendor_id(),
            device_desc.product_id(),
            is_custom);
    }
}
