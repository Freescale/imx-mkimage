MKIMG = mkimage_imx8
#DCD_CFG_SRC = imx8qm_dcd_800MHz.cfg
#DCD_CFG_SRC = imx8qm_dcd_1.2GHz.cfg
DCD_CFG_SRC = imx8qm_dcd_1.6GHz.cfg

DCD_CFG = imx8qm_dcd.cfg.tmp

CC ?= gcc
CFLAGS ?= -O2 -Wall -std=c99 -static
INCLUDE = ./lib

$(MKIMG): mkimage_imx8.c $(DCD_CFG_SRC)
	@echo "Converting iMX8 DCD file" 
	$(CC) -E -Wp,-MD,.imx8qm_dcd.cfg.cfgtmp.d  -nostdinc -Iinclude -I$(INCLUDE) -x c -o $(DCD_CFG) $(DCD_CFG_SRC)

	@echo "Compiling mkimage_imx8"
	$(CC) $(CFLAGS) -o $@ mkimage_imx8.c

u-boot-atf.bin: u-boot.bin bl31.bin
	@cp bl31.bin u-boot-atf.bin
	@dd if=u-boot.bin of=u-boot-atf.bin bs=1K seek=128

.PHONY: clean
clean:
	@rm -f $(MKIMG) $(DCD_CFG) .imx8_dcd.cfg.cfgtmp.d

flash_scfw: $(MKIMG) $(DCD_CFG) scfw_tcm.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -out flash.bin

flash_dcd: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot-atf.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -ap u-boot-atf.bin a53 0x80000000 -out flash.bin

flash: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot-atf.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -ap u-boot-atf.bin a53 0x80000000 -out flash.bin

flash_flexspi: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot-atf.bin
	./mkimage_imx8 -dev flexspi -scfw scfw_tcm.bin -ap u-boot-atf.bin a53 0x80000000 -out flash.bin

flash_ca72: $(MKIMG) $(DCD_CFG) scfw_tcm.bin u-boot-atf.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -ap u-boot-atf.bin a72 0x80000000 -out flash.bin

flash_cm4_0: $(MKIMG) $(DCD_CFG) scfw_tcm.bin m4_image.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -m4 m4_image.bin 0 0x34FE0000 -out flash.bin

flash_cm4_1: $(MKIMG) $(DCD_CFG) scfw_tcm.bin m4_image.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -m4 m4_image.bin 1 0x38FE0000 -out flash.bin

flash_all: $(MKIMG) $(DCD_CFG) scfw_tcm.bin m4_image.bin u-boot-atf.bin scd.bin csf.bin csf_ap.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -m4 m4_image.bin 0 0x34FE0000 -ap u-boot-atf.bin a53 0x80000000 -scd scd.bin -csf csf.bin -csf_ap csf_ap.bin -out flash.bin

flash_ca72_ddrstress: $(MKIMG) scfw_tcm.bin mx8qm_ddr_stress_test.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -ap mx8qm_ddr_stress_test.bin a72 0x00112000 -out flash.bin

flash_ca53_ddrstress: $(MKIMG) scfw_tcm.bin mx8qm_ddr_stress_test.bin
	./mkimage_imx8 -scfw scfw_tcm.bin -ap mx8qm_ddr_stress_test.bin a53 0x00112000 -out flash.bin

flash_ca72_ddrstress_dcd: $(MKIMG) $(DCD_CFG) scfw_tcm.bin mx8qm_ddr_stress_test.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -ap mx8qm_ddr_stress_test.bin a72 0x00112000 -out flash.bin

flash_ca53_ddrstress_dcd: $(MKIMG) $(DCD_CFG) scfw_tcm.bin mx8qm_ddr_stress_test.bin
	./mkimage_imx8 -dcd $(DCD_CFG) -scfw scfw_tcm.bin -ap mx8qm_ddr_stress_test.bin a53 0x00112000 -out flash.bin

