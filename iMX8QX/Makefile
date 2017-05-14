MKIMG = mkimage_imx8
DCD_CFG_SRC = imx8qx_dcd.cfg
DCD_CFG = imx8qx_dcd.cfg.tmp

CC ?= gcc
CFLAGS ?= -O2 -Wall -std=c99 -static
INCLUDE = ./lib

$(MKIMG): mkimage_imx8.c $(DCD_CFG_SRC)
	@echo "Converting iMX8 DCD file"
	$(CC) -E -Wp,-MD,.imx8qx_dcd.cfg.cfgtmp.d  -nostdinc -Iinclude -I$(INCLUDE) -x c -o $(DCD_CFG) $(DCD_CFG_SRC)

	@echo "Compiling mkimage_imx8"
	$(CC) $(CFLAGS) -o $@ mkimage_imx8.c

.PHONY: clean
clean:
	@rm -f $(MKIMG) $(DCD_CFG) .imx8qx_dcd.cfg.cfgtmp.d

flash_scfw: $(MKIMG) scfw_tcm.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -out flash.bin

flash_dcd: $(MKIMG) $(DCD_CFG) scfw_tcm.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -out flash.bin

flash: $(MKIMG) scfw_tcm.bin u-boot.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -ap u-boot.bin a35 0x80000000 -out flash.bin

flash_flexspi: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot.bin
	./mkimage_imx8 -dev flexspi -dcd $(DCD_CFG) -scfw scfw_tcm.bin -ap u-boot.bin a35 0x80000000 -out flash.bin

flash_cm4_0: $(MKIMG) $(DCD_CFG) scfw_tcm.bin m4_image.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -m4 m4_image.bin 0 0x34FE0000 -out flash.bin

flash_all: $(MKIMG) $(DCD_CFG) scfw_tcm.bin m4_image.bin u-boot.bin scd.bin csf.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -m4 m4_image.bin 0 0x34FE0000 -ap u-boot.bin a35 0x80000000 -scd scd.bin -csf csf.bin -out flash.bin
