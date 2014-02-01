OpenInsider project
===================

  The project OpenInsider is aimed to develop an GUI and target-independent
firmware for real-time watch and edit of variables inside the embedded device.
The protocol is constructed and implemented independent on the link layer of
communication. Therefore many link layers can be implemented (serial, CAN or
Ethernet). The addresses of the watched variables are read from binary ELF
file, so the user can easily specify the variable name.

  The first versions of this project is intended to be compatible with
Freescale tool called FreeMASTER at message level over serial link, but this
tool is intended to be run on other architectures than Motorola M68k. (ARM,
Atmel AVR etc.)

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


Coding style and development guidelines
---------------------------------------

See file [HACKING](HACKING).


License
-------

The OpenInsider code is released under the terms of the GNU Lesser General
Public License (LGPL), version 3 or later.

See file [COPYING.LGPL3](COPYING.LGPL3) for details.



