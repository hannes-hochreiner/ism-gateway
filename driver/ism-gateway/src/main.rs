extern crate libusb;

fn main() {
    let version = libusb::version();

    println!("Using libusb version {}.{}.{}.{}", version.major(), version.minor(), version.micro(), version.nano());
}
