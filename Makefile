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

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
endif

include toolchain/Makefile.$(TOOLCHAIN)
include target/Makefile.$(TARGET)
include Makefile.common

VPATH	+= src

OBJS	+= packet.o protocol.o memory.o scope.o


TOBJS	:= $(OBJS:%.o=tmp/%.o)
TDEPS	:= $(OBJS:%.o=tmp/%.o.d)

###############################################################################
# Rules

.PHONY: all clean

all: bin/libopeninsider_$(TARGET).a

clean:
	@printf "  \e[31;1mCLEAN\e[0m\n"
	$(Q)$(RM) tmp/*.o tmp/*.d bin/*.a

bin/libopeninsider_$(TARGET).a: $(TOBJS)
	@printf "  \e[33;1mAR\e[0m      libopeninsider_$(TARGET).a\n"
	$(Q)$(AR) $(ARFLAGS) $@ $(TOBJS)

tmp/%o: %c
	@printf "  \e[32;1mCC\e[0m      $<\n"
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $@ -c $<

-include $(TDEPS)