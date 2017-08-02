MKIMG = mkimage_imx8
DCD_CFG_SRC = imx8qx_dcd.cfg
DCD_CFG = imx8qx_dcd.cfg.tmp

CC ?= gcc
CFLAGS ?= -O2 -Wall -std=c99 -static
INCLUDE = ./lib

N ?= latest
SERVER=http://yb2.am.freescale.net
DIR = build-output/Linux_IMX_MX8/$(N)/common_bsp

$(MKIMG): mkimage_imx8.c $(DCD_CFG_SRC)
	@echo "Converting iMX8 DCD file"
	$(CC) -E -Wp,-MD,.imx8qx_dcd.cfg.cfgtmp.d  -nostdinc -Iinclude -I$(INCLUDE) -x c -o $(DCD_CFG) $(DCD_CFG_SRC)

	@echo "Compiling mkimage_imx8"
	$(CC) $(CFLAGS) -o $@ mkimage_imx8.c

u-boot-atf.bin: u-boot.bin bl31.bin
	@cp bl31.bin u-boot-atf.bin
	@dd if=u-boot.bin of=u-boot-atf.bin bs=1K seek=128

.PHONY: clean nightly
clean:
	@rm -f $(MKIMG) $(DCD_CFG) .imx8qx_dcd.cfg.cfgtmp.d

flash_scfw: $(MKIMG) scfw_tcm.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -out flash.bin
	@echo "Example: dd if=flash.bin of=[SD device] bs=1k seek=33" 

flash_dcd: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot-atf.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -ap u-boot-atf.bin a35 0x80000000 -out flash.bin

flash: $(MKIMG) scfw_tcm.bin u-boot-atf.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -ap u-boot-atf.bin a35 0x80000000 -out flash.bin

flash_flexspi: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot-atf.bin
	./mkimage_imx8 -dev flexspi -dcd $(DCD_CFG) -scfw scfw_tcm.bin -ap u-boot-atf.bin a35 0x80000000 -out flash.bin

flash_cm4_0: $(MKIMG) $(DCD_CFG) scfw_tcm.bin m4_image.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -m4 m4_image.bin 0 0x34FE0000 -out flash.bin

flash_all: $(MKIMG) $(DCD_CFG) scfw_tcm.bin m4_image.bin u-boot-atf.bin scd.bin csf.bin csf_ap.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -m4 m4_image.bin 0 0x34FE0000 -ap u-boot-atf.bin a35 0x80000000 -scd scd.bin -csf csf.bin -csf_ap csf_ap.bin -out flash.bin

flash_ca35_ddrstress: $(MKIMG) scfw_tcm.bin mx8qxp_ddr_stress_test.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -ap mx8qxp_ddr_stress_test.bin a35 0x00112000 -out flash.bin	

nightly :
	@rm -rf boot
	@wget -q $(SERVER)/$(DIR)/imx-boot/imx-boot-tools/mx8qx-scfw-tcm.bin -O scfw_tcm.bin
	@wget -q $(SERVER)/$(DIR)/imx-boot/imx-boot-tools/bl31-imx8qxp.bin -O bl31.bin
	@wget -q $(SERVER)/$(DIR)/imx-boot/imx-boot-tools/u-boot-imx8qxplpddr4arm2.bin-sd -O u-boot.bin
	@wget -qr -nd -l1 -np $(SERVER)/$(DIR) -P boot -A "Image-fsl-imx8qxp-*.dtb"
	@wget -q $(SERVER)/$(DIR)/Image-imx8_all.bin -O Image
	@mv -f Image boot
	@rename "Image-" "" boot/*.dtb
