MKIMG = mkimage_imx8

CC = gcc
CFLAGS = -O2 -Wall -std=c99 -static

$(MKIMG): mkimage_imx8.c imx8dv_dcd.cfg
	$(CC) -E -Wp,-MD,.imx8dv_dcd.cfg.cfgtmp.d  -nostdinc -Iinclude -x c -o imx8dv_dcd.cfg.tmp imx8dv_dcd.cfg
	$(CC) $(CFLAGS) -o $@ mkimage_imx8.c

.PHONY: clean
	
clean:
	@rm -f mkimage_imx8 imx8dv_dcd.cfg.tmp .imx8dv_dcd.cfg.cfgtmp.d

flash: $(MKIMG)
	./mkimage_imx8 -dcd imx8dv_dcd.cfg.tmp -scfw scfw_tcm.bin -ap u-boot.bin a53 0x80000000 -out flash.bin
