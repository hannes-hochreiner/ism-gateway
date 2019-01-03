BUILD_DIR = bld/

ism-gateway-driver: Makefile src/main.cpp src/usb_session.cpp src/usb_device.cpp src/usb_exception.cpp src/aes_ecb.cpp src/aes_exception.cpp src/args.cpp
	g++ -g -Iinc -o ${BUILD_DIR}$@ $(filter %.cpp, $^) -lusb-1.0 -lcryptopp

${BUILD_DIR}aes-ecb-test: Makefile src_test/gtest.cpp inc/aes_ecb.h src/aes_ecb.cpp inc/aes_exception.h src/aes_exception.cpp src_test/aes_ecb.test.cpp
	g++ -g -Iinc -o $@ $(filter %.cpp, $^) -lcryptopp -lgtest

${BUILD_DIR}args-test: Makefile src_test/gtest.cpp inc/args.h src/args.cpp src_test/args.test.cpp
	g++ -g -Iinc -o $@ $(filter %.cpp, $^) -lgtest

run-local: ism-gateway-driver
	bld/ism-gateway-driver

run-remote: build-remote
	ssh hc2 "sudo ism-gateway-driver/bld/ism-gateway-driver"

build-remote: sync-source
	ssh hc2 "make -C ~/ism-gateway-driver"

sync-source:
	rsync -arvz -e "ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null" --progress . hc2:~/ism-gateway-driver

test-aes-ecb: ${BUILD_DIR}aes-ecb-test
	$<

test-args: ${BUILD_DIR}args-test
	$<
