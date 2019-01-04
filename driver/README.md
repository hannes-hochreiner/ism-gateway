# ISM-Gateway Driver
This is a simple driver to read messages from the ism-gateway and publish it to zeromq.

## Installation
The program can be compiled from source by running make:

```
make
```

### 3rd party libraries

#### Crypto++
The [Crypto++ Library](https://cryptopp.com) is used for decrypting the messages from the sensors.
```
sudo aptitude install libcrypto++-dev
```

#### JSON for Modern C++
[JSON for Modern C++](https://github.com/nlohmann/json) is used as stand-alone hpp-import.

#### ZeroMQ
The [C++ bindings for ZeroMQ](https://github.com/zeromq/cppzmq) are used.
Therefore, the czmq-library must be installed.

```
sudo dnf install czmq-devel
```
or
```
sudo apt install libczmq-dev
```

## Running
The program can be run from command line.
A decryption key needs to be supplied.

```
sudo ism-gateway-driver/bld/ism-gateway-driver -k <key> 2>/dev/null
```
