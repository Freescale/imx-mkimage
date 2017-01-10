MKIMG = mkimage_imx8

CC ?= gcc
CFLAGS ?= -O2 -Wall -std=c99 -static
INCLUDE = ./imx8qm/lib

imx8qm_dcd.cfg.tmp: imx8qm_dcd.cfg
	@echo "Converting iMX8QM DCD file" 
	$(CC) -E -Wp,-MD,.imx8qm_dcd.cfg.cfgtmp.d  -nostdinc -Iinclude -I$(INCLUDE) -x c -o imx8qm_dcd.cfg.tmp imx8qm_dcd.cfg

$(MKIMG): mkimage_imx8.c imx8qm_dcd.cfg.tmp
	@echo "Compiling mkimage_imx8"
	$(CC) $(CFLAGS) -o $@ mkimage_imx8.c

.PHONY: clean
	
clean:
	@rm -f mkimage_imx8 imx8qm_dcd.cfg.tmp .imx8qm_dcd.cfg.cfgtmp.d

flash_8qm: $(MKIMG) flash_8qm.bin imx8qm_dcd.cfg.tmp scfw_tcm.bin u-boot.bin
	@echo "Creating bootable image flash_8qm.bin"
	./mkimage_imx8 -dcd imx8qm_dcd.cfg.tmp -scfw scfw_tcm.bin -ap u-boot.bin a53 0x80000000 -out flash_8qm.bin

