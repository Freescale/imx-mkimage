

MKIMG = $(PWD)/mkimage_imx8
CC ?= gcc
CFLAGS ?= -g -O2 -Wall -std=c99 -static
INCLUDE += $(CURR_DIR)/src

SRCS = src/imx8qm.c  src/imx8qx.c  src/mkimage_imx8.c

vpath $(INCLUDE)

.PHONY:  clean all bin


.DEFAULT:
	$(MAKE) bin
	$(MAKE) -C $(SOC) $@

#print out usage as the default target
all: $(MKIMG) help


clean:
	@rm $(MKIMG)
	$(MAKE) -C iMX8QM clean
	$(MAKE) -C iMX8QX clean
	$(MAKE) -C iMX8M clean
	$(MAKE) -C iMX8dv clean

$(MKIMG): $(SRCS)
	@echo "Compiling mkimage_imx8"
	$(CC) $(CFLAGS) $(SRCS) -o $(MKIMG) -I src

bin: $(MKIMG)

help:
	@echo $(CURR_DIR)
	@echo "usage ${MAKE} SOC=<SOC_TARGET> [TARGET]"
	@echo "i.e.  ${MAKE} SOC=iMX8QX flash_dcd"
	@echo "Common Targets:"
	@echo
	@echo "Parts with SCU"
	@echo "	  flash_scfw    - Only boot SCU"
	@echo "	  flash_dcd     - SCU + AP cluster with DCDs"
	@echo "	  flash         - SCU + AP (no DCD)"
	@echo "	  flash_flexspi - SCU + AP (FlexSPI device) "
	@echo "	  flash_cm4_0   - SCU + M4_0 w/ DCDs "
	@echo "	  flash_all     - SCU + AP + M4 + SCD + CSF(AP & SCU) w/ DCDs"
	@echo ""
	@echo "Parts w/o SCU"
	@echo "	  flash_hdmi_spl_uboot     - HDMI FW + u-boot spl"
	@echo "	  flash_hdmi_spl_uboot_tee - HDMI FW + u-boot spl w/ tee"
	@echo "	  flash_spl_uboot          - u-boot spl"
	@echo "	  flash_spl_uboot_tee      - u-boot spl w/tee"
	@echo
	@echo


