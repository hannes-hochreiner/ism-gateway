BUILD_DIR = bld/
SOURCE_DIR = src/
INCLUDE_DIR = inc/
PROG_DIR = prog/

CPPFLAGS += -Iinc

ism-gateway-driver_LDFLAGS = -lusb-1.0 -lcryptopp -lzmq
ism-gateway-driver_OBJS = main.o usb_session.o usb_device.o usb_exception.o aes_ecb.o aes_exception.o args.o

$(PROG_DIR)ism-gateway-driver: Makefile $(addprefix $(BUILD_DIR), $(ism-gateway-driver_OBJS)) | $(PROG_DIR)
	$(LINK.cpp) -g -o $@ $(filter %.o, $^) $(ism-gateway-driver_LDFLAGS)

${PROG_DIR}aes-ecb-test: Makefile src_test/gtest.cpp inc/aes_ecb.h src/aes_ecb.cpp inc/aes_exception.h src/aes_exception.cpp src_test/aes_ecb.test.cpp
	g++ -g -Iinc -o $@ $(filter %.cpp, $^) -lcryptopp -lgtest

${PROG_DIR}args-test: Makefile src_test/gtest.cpp inc/args.h src/args.cpp src_test/args.test.cpp
	g++ -g -Iinc -o $@ $(filter %.cpp, $^) -lgtest

run-local: $(PROG_DIR)ism-gateway-driver
	$<

run-remote: build-remote
	ssh hc2 "sudo ism-gateway-driver/bld/ism-gateway-driver"

build-remote: sync-source
	ssh hc2 "make -C ~/ism-gateway-driver"

sync-source:
	rsync -arvz -e "ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null" --exclude=bld --exclude=prog --progress . hc2:~/ism-gateway-driver

test-aes-ecb: ${BUILD_DIR}aes-ecb-test
	$<

test-args: ${BUILD_DIR}args-test
	$<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(PROG_DIR):
	mkdir $(PROG_DIR)

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp $(INCLUDE_DIR)%.h | $(BUILD_DIR)
	$(COMPILE.cpp) -g $< -o $@

clean:
	rm -r $(PROG_DIR)
	rm -r $(BUILD_DIR)
