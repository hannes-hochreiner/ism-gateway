debug:
	-ssh pi@raspberrypi "sudo kill \`pidof openocd\`"
	scp stm32.cfg pi@raspberrypi:/tmp/stm32.cfg
	ssh pi@raspberrypi "sudo openocd -f /tmp/stm32.cfg > /dev/null 2>&1 &"
	-arm-none-eabi-gdb -x gdb.cmd ismGateway/build/ismGateway.elf
	ssh pi@raspberrypi "sudo kill \`pidof openocd\`"
	ssh pi@raspberrypi "rm /tmp/stm32.cfg"
