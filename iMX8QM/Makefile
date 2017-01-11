MKIMG = mkimage_imx8qm
DCD_CFG_SRC = imx8qm_dcd.cfg
DCD_CFG = imx8qm_dcd.cfg.tmp

CC ?= gcc
CFLAGS ?= -O2 -Wall -std=c99 -static
INCLUDE = ./lib

$(DCD_CFG): $(DCD_CFG_SRC)
	@echo "Converting iMX8QM DCD file" 
	$(CC) -E -Wp,-MD,.imx8qm_dcd.cfg.cfgtmp.d  -nostdinc -Iinclude -I$(INCLUDE) -x c -o $(DCD_CFG) $(DCD_CFG_SRC)

$(MKIMG): mkimage_imx8qm.c
	@echo "Compiling mkimage_imx8qm"
	$(CC) $(CFLAGS) -o $@ mkimage_imx8qm.c

.PHONY: clean
	
clean:
	@rm -f $(MKIMG) $(DCD_CFG) .imx8qm_dcd.cfg.cfgtmp.d

flash_8qm: $(MKIMG) $(DCD_CFG)
	@echo "Creating bootable image flash_8qm.bin"
	./$(MKIMG) -dcd $(DCD_CFG) -scfw scfw_tcm.bin -ap u-boot.bin a53 0x80000000 -out flash_8qm.bin

