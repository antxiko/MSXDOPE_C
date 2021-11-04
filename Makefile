# MAKEFILE for 8/16K ROMs using SDCC compiler
# CFILENAME = Source name without extension
# ROMFILENAME = ROM file name without extension
# ROMSIZE = 2000 for 8KBytes ROM or 4000 for 16KBytes ROM
CFILENAME=game
ROMFILENAME=DOPE
CODELOC=0x4050
CRT=libs/crt0_MSX32kROM4000.rel

# add paths from our objects files into LIB variable
LIBS=libs/VDP_TMS9918A_ROM.rel libs/VDP_VPRINT.rel libs/AY38910BF_S.rel libs/PT3player.rel libs/unRLEWBtoVRAM.rel

all: bin/$(ROMFILENAME).ROM
	# get SDCC version
	sdcc -v

build:
	echo "mkdir build"
	mkdir build

bin:
	echo "mkdir bin"
	mkdir bin


build/$(CFILENAME).ihx: build bin
	echo "Compiling..."
	sdcc -mz80 -o build/ --code-loc $(CODELOC) --data-loc 0xC000 --use-stdout --no-std-crt0 $(CRT) $(LIBS) src/$(CFILENAME).c

build/$(CFILENAME).bin: build/$(CFILENAME).ihx
	hex2bin -e bin -s 4000 -l 8000 build/$(CFILENAME).ihx

bin/$(ROMFILENAME).ROM: build/$(CFILENAME).bin 
	cp build/$(CFILENAME).bin bin/$(ROMFILENAME).ROM

clean: build/$(CFILENAME).ihx
	rm build/$(CFILENAME).ihx
	rm bin/$(ROMFILENAME).ROM