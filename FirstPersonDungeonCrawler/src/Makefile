#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.9 $
#        $Date: 1999/04/02 10:10:48 $
# --------------------------------------------------------------------
#
# Makefile for game template
#
#

include /usr/include/n64/make/PRdefs

OPTIMIZER = -O2
LCDEFS    = -D_FINALROM -DNDEBUG -DF3DEX_GBI_2
N64LIB    = -lultra_rom

# for nustd
NUSTDINCDIR = $(N64KITDIR)/nustd/include
NUSTDLIBDIR = $(N64KITDIR)/nustd/lib

ELF		= dungeon_crawler.elf
TARGETS	= ../dungeon_crawler.z64
MAP		= dungeon_crawler.map

LD_SCRIPT	 = dungeon_crawler.ld
CP_LD_SCRIPT = dungeon_crawler_cp.ld

ASMFILES   := $(wildcard asm/*.s)
ASMOBJECTS  = $(ASMFILES:.s=.o)

CODEFILES  := $(wildcard *.c)
OBJECTFILES  := $(wildcard objects/*.c)
CODEOBJECTS = $(CODEFILES:.c=.o) $(OBJECTFILES:.c=.o)

DATAFILES  := $(wildcard data/*.c)
DATAOBJECTS = $(DATAFILES:.c=.o)

BOOT	    = /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ    = boot.6102.o
CODESEGMENT = codesegment.o

OBJECTS = $(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCDEFS += $(HW_FLAGS)
LCINCS  = -I. -I$(NUSTDINCDIR)
LCOPTS  = -G 0
LDFLAGS = -L/usr/lib/n64 $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc -L$(NUSTDLIBDIR) -lnustd_d

LDIRT  = $(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS) $(TARGETS)

default: $(TARGETS)

cleaner:
	find ../ -name '*.o' -delete
	find ../ -name '*.gen.h' -delete

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
