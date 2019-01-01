BUILD_DIR = bld/

ism-gateway-driver: Makefile src/main.cpp src/usb_session.cpp src/usb_device.cpp src/usb_exception.cpp
	g++ -g -Iinc -o ${BUILD_DIR}ism-gateway-driver src/main.cpp src/usb_session.cpp src/usb_device.cpp src/usb_exception.cpp -lusb-1.0

run-local: ism-gateway-driver
	bld/ism-gateway-driver

run-remote: build-remote
	ssh hc2 "sudo ism-gateway-driver/bld/ism-gateway-driver"

build-remote: sync-source
	ssh hc2 "make -C ~/ism-gateway-driver"

sync-source:
	rsync -arvz -e "ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null" --progress . hc2:~/ism-gateway-driver