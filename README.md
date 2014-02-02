OpenInsider project
===================

  The project OpenInsider is aimed to develop an GUI and target-independent
firmware for real-time watch and edit of variables inside the embedded device.
The protocol is constructed and implemented independent on the link layer of
communication. Therefore many link layers can be implemented (serial, CAN or
Ethernet). The addresses of the watched variables are read from binary ELF
file, so the user can easily specify the variable name.

  The first versions of this project is intended to be compatible with
Freescale tool called [FreeMASTER](http://www.freescale.com/Freemaster) at
message level over serial link, but this tool is intended to be run on other
architectures than Motorola M68k. (ARM, Atmel AVR etc.)


OpenInsider-lib
===============

  This repository contains shared library code meant to run on the target
machine.

Status
------

  The OpenInsider project is currently work in progress. Not all functions
are supported, yet.

`IMPORTANT`:
The API of the library is NOT yet considered stable! Please do not rely on
it, yet! Changes to function names, macro names etc. can happen at any time
without prior notice!

Prerequisites
-------------
  

Building
--------

The library is built in the bin directory by typing:


```
$ make TOOLCHAIN=<your compiler toolchain> TARGET=<target cpu>
```

Currently works the arm-gcc toolchain and stm32f4 target

```
make TOOLCHAIN=arm-gcc TARGET=stm32f4
```

Tweaking the library
--------------------

There are some additional variables, specfied in the make command that can
affect library building process.

```
make VAR=value
```

Available variables:


`INSIDER_BUFFER_SIZE=256`

This parameter sets the size of receiving and transmitting communication ring
buffer. Be aware that the value must be 2^n, and must be in the range <64,256>
so that currently only values 64,128 and 256 are supported. Larger values will
break compatibility with FreeMASTER and smaller values will break the FreeMASTER
board detection process.

`INSIDER_BOARD_FW=0x0100`

This parameter sets the firmware version number in the information structure
in the board detection process. The value is two bytes, where high byte is major
version number and low byte is minor version number.

`INSIDER_BOARD_NAME="OpenInsider board driver"`

The board name string returned in board detection process. The string is
truncated at 25 characters.


Coding style and development guidelines
---------------------------------------

See file [HACKING](HACKING).


License
-------

The OpenInsider code is released under the terms of the GNU Lesser General
Public License (LGPL), version 3 or later.

See file [COPYING.LGPL3](COPYING.LGPL3) for details.



