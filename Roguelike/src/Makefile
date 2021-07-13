include /usr/include/n64/make/PRdefs

# The directory which has the include file and library of NuSYSTEM
NUSYSINCDIR = /usr/include/n64/nusys
NUSYSLIBDIR = /usr/lib/n64/nusys

LIB = /usr/lib/n64
LPR = $(LIB)/PR
INC = /usr/include/n64

LCDEFS  = -DNDEBUG -DF3DEX_GBI_2
LCINCS  = -I. -I$(NUSYSINCDIR) -I/usr/include/n64/PR
LCOPTS  = -G 0
LDIRT   = $(ELF) $(ASMOBJECTS) $(CP_LD_SCRIPT) $(MAP) $(TARGETS)
LDFLAGS = $(MKDEPOPT) -L$(LIB) -L$(NUSYSLIBDIR) -lnusys -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

OPTIMIZER =	-g

ELF		= roguelike.elf
TARGETS	= ../roguelike.z64
MAP		= roguelike.map
LD_SCRIPT	= roguelike.ld
CP_LD_SCRIPT= roguelike_cp.ld


ASMFILES   := $(wildcard asm/*.s)
ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES  := $(wildcard *.c)
SCREENFILES:= $(wildcard screens/*.c)
DATAFILES  := $(wildcard data/*.c)
CODEOBJECTS = $(CODEFILES:.c=.o) $(SCREENFILES:.c=.o) $(DATAFILES:.c=.o) $(NUSYSLIBDIR)/nusys_rom.o

DATAFILES2   = 
DATAOBJECTS = $(DATAFILES2:.c=.o)

CODESEGMENT = codesegment.o

BOOT	 = /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ = boot.6102.o

OBJECTS = $(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJECTS)

default: $(TARGETS)

cleaner:
	find . -name '*.v64' -delete
	find . -name '*.z64' -delete
	find . -name '*.elf' -delete
	find . -name '*.o' -delete
	find . -name '*.bin' -delete
	find . -name '*.plan_bak*' -delete
	find . -name '*.sprite' -delete
	find . -name '*.dfs' -delete
	find . -name '*.raw' -delete
	find . -name '*.gen.h' -delete
	rm -f ../*.z64 ../*.n64

include $(COMMONRULES)

.s.o:
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINCDIR) -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
