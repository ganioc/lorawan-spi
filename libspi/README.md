## Steps
- 测试树莓派下的串口通讯

./libspi -d /dev/ttyUSB0
### 发送
$ ./libspi -d /dev/ttyUSB0 --tx

### 接收
$ ./libspi -d /dev/ttyUSB1 --rx

### 执行usb2gpio
$ ./libspi -d /dev/ttyACM0 --usb2gpio

### 执行usb2spi
$ ./libspi -d /dev/ttyACM0 --usb2spi
$ ./libspi -d /dev/ttyACM0 --test  // OK

