##
## This file is part of the OpenInsider project.
##
## Copyright (C) 2014 Frantisek Burian <BuFran@seznam.cz>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

ifeq ($(TOOLCHAIN),)
$(error TOOLCHAIN not specified ! Stop.)
endif

ifeq ($(TARGET),)
$(error TARGET not specified ! Stop.)
endif

###############################################################################
# tweaking variables

INSIDER_BUFFER_SIZE	?= 256
INSIDER_BOARD_FW	?= 0x0100
INSIDER_BOARD_NAME	?= "OpenInsider board driver"


# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
endif

include toolchain/Makefile.$(TOOLCHAIN)
include target/Makefile.$(TARGET)
include Makefile.common

VPATH	+= src

OBJS	+= insider.o buffer.o packet.o protocol.o memory.o scope.o recorder.o

DEFS	+= -DINSIDER_BUFFER_SIZE=$(INSIDER_BUFFER_SIZE)
DEFS	+= -DINSIDER_BOARD_FW=$(INSIDER_BOARD_FW)
DEFS	+= -DINSIDER_BOARD_NAME=$(INSIDER_BOARD_NAME)

OBJS	:= $(addprefix tmp/,$(OBJS))

###############################################################################
# Rules

.PHONY: all
all: bin/libopeninsider_$(TARGET).a

.PHONY: clean
clean:
	@printf "  CLEAN\n"
	$(Q)$(RM) -d -rf tmp bin

.PHONY: distclean
distclean:
	@printf "  DISTCLEAN\n"
	$(Q)$(RM) -d -rf tmp

bin/libopeninsider_$(TARGET).a: $(TOBJS) bin
	@printf "  AR      libopeninsider_$(TARGET).a\n"
	$(Q)$(AR) $(ARFLAGS) $@ $(TOBJS)

tmp/%.o: %.c tmp
	@printf "  CC      $<\n"
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $@ -c $<

bin tmp:
	@printf "  DIR     $@\n"
	@mkdir -p $@

-include $(OBJS:.o=.d)
