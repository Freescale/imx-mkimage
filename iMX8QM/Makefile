MKIMG = mkimage_imx8
DCD_CFG_SRC = imx8_dcd.cfg
DCD_CFG = imx8_dcd.cfg.tmp

CC ?= gcc
CFLAGS ?= -O2 -Wall -std=c99 -static
INCLUDE = ./lib

$(MKIMG): mkimage_imx8.c utility.c utility.h boot.h $(DCD_CFG_SRC)
	@echo "Converting iMX8 DCD file" 
	$(CC) -E -Wp,-MD,.imx8_dcd.cfg.cfgtmp.d  -nostdinc -Iinclude -I$(INCLUDE) -x c -o $(DCD_CFG) $(DCD_CFG_SRC)

	@echo "Compiling mkimage_imx8"
	$(CC) $(CFLAGS) -o $@ mkimage_imx8.c utility.c

.PHONY: clean
clean:
	@rm -f $(MKIMG) $(DCD_CFG) .imx8_dcd.cfg.cfgtmp.d flash.bin

help: $(MKIMG)
	./mkimage_imx8 -help

flash: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot.bin
	./mkimage_imx8 -dev sd -dcd $(DCD_CFG) -img scfw_tcm.bin:scu -img u-boot.bin:ca53

flash_ca72: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot.bin
	./mkimage_imx8 -dev sd -dcd $(DCD_CFG) -img scfw_tcm.bin:scu -img u-boot.bin:ca72

flash_cm4_0: $(MKIMG) $(DCD_CFG) scfw_tcm.bin m4.bin
	./mkimage_imx8 -dev sd -dcd $(DCD_CFG) -img scfw_tcm.bin:scu -img m4.bin:cm4_0

flash_cm4_1: $(MKIMG) $(DCD_CFG) scfw_tcm.bin m4.bin
	./mkimage_imx8 -dev sd -dcd $(DCD_CFG) -img scfw_tcm.bin:scu -img m4.bin:cm4_1

flash_full_addresses: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot.bin
	./mkimage_imx8 -dev sd -dcd $(DCD_CFG) -img scfw_tcm.bin:scu:entry=0x1FFE0000:to=0x30FE0000 -img u-boot.bin:ca53:entry=0x80000000:to=0x80000000

flash_all: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot.bin m4.bin scd.bin csf.bin
	./mkimage_imx8 -dev sd -dcd $(DCD_CFG) -img scfw_tcm.bin:scu:entry=0x1FFE0000:to=0x30FE0000 -img u-boot.bin:ca53:entry=0x80000000:to=0x80000000 -img m4.bin:cm4_0:entry=0x1FFE0000:to=0x34FE0000 -scd scd.bin -csf csf.bin
