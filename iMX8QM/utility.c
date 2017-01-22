#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "boot.h"
#include "utility.h"

extern img_container_opts_t container_opts[MAX_NUM_OF_CONTAINER];
extern img_misc_opts	misc_opts;
extern unsigned int g_iContainer;

#define IVT_OFFSET_NAND     (0x400)
#define IVT_OFFSET_I2C      (0x400)
#define IVT_OFFSET_FLEXSPI  (0x1000)
#define IVT_OFFSET_SD		(0x400)
#define IVT_OFFSET_SATA		(0x400) 

#define IMAGE_OFFSET_SD    (0x8000)
#define IMAGE_OFFSET_SATA  (0x8000)

#define SPLIT_NUM_MAX	16

#define M4_VIEW_TCML_ADDR 			0x1FFE0000
#define DEFAULT_SCU_ENTRY_ADDR 		M4_VIEW_TCML_ADDR
#define DEFAULT_M4_0_ENTRY_ADDR 	M4_VIEW_TCML_ADDR
#define DEFAULT_M4_1_ENTRY_ADDR 	M4_VIEW_TCML_ADDR
#define DEFAULT_AP_ENTRY_ADDR 		0x80000000

#define DEFAULT_SCU_LOAD_ADDR 		0x30FE0000
#define DEFAULT_M4_0_LOAD_ADDR 		0x34FE0000
#define DEFAULT_M4_1_LOAD_ADDR 		0x38FE0000
#define DEFAULT_AP_LOAD_ADDR 		0x80000000


typedef enum {
	BOOT_DEV_SD = 0,
	BOOT_DEV_SATA,
	BOOT_DEV_NAND,
	BOOT_DEV_FLEXSPI,
	BOOT_DEV_I2C,
}boot_dev;

img_misc_opts boot_dev_opts[] = {
	{IVT_OFFSET_SD, IMAGE_OFFSET_SD, 0x200},
	{IVT_OFFSET_SATA, IMAGE_OFFSET_SATA, 0x200},
	{IVT_OFFSET_NAND, 0, 0x800},
	{IVT_OFFSET_FLEXSPI, 0, 0x1},
	{IVT_OFFSET_I2C, 0, 0x1},
};


int dump_imx_header_v3(imx_header_v3_t * imx_header)
{
    int i = 0, j = 0;
    flash_header_v3_t *fhdr = NULL;
    boot_data_v3_t *boot_data = NULL;
    boot_img_t *boot_img = NULL;

    for (i = 0; i < g_iContainer; i++) {
        fhdr = &imx_header->fhdr[i];
        printf("FHDR%d\n", i);
        printf("\theader : 0x%x\n", *(uint32_t*) &fhdr->header);
        printf("\tdcd_ptr : 0x%lx\n", fhdr->dcd_ptr);
        printf("\tboot_data_ptr : 0x%lx\n", fhdr->boot_data_ptr);
        printf("\tself : 0x%lx\n", fhdr->self);
        printf("\tcsf : 0x%lx\n", fhdr->csf);
        printf("\tscd : 0x%lx\n", fhdr->scd);
    }
    for (i = 0; i < g_iContainer; i++) {
        boot_data = &imx_header->boot_data[i];
        printf("BD%d\n", i);
        printf("\tnum_images : 0x%x\n", boot_data->num_images);
        printf("\tbd_size : 0x%x\n", boot_data->bd_size);
        printf("\tbd_flags : 0x%x\n", boot_data->bd_flags);

        for (j = 0; j < boot_data->num_images; j++) {
            boot_img = &boot_data->img[j];
            printf("\tImage%d\n", j);
            printf("\t\tsrc : 0x%lx\n", boot_img->src);
            printf("\t\tdst : 0x%lx\n", boot_img->dst);
            printf("\t\tentry : 0x%lx\n", boot_img->entry);
            printf("\t\tsize : 0x%x\n", boot_img->size);
            printf("\t\tflags : 0x%x\n", boot_img->flags);
        }

        if (boot_data->scd.size) {
            printf("\tSCD\n");
            printf("\t\tsrc : 0x%lx\n", boot_data->scd.src);
            printf("\t\tdst : 0x%lx\n", boot_data->scd.dst);
            printf("\t\tsize : 0x%x\n", boot_data->scd.size);
        }

        if (boot_data->csf.size) {
            printf("\tCSF\n");
            printf("\t\tsrc : 0x%lx\n", boot_data->csf.src);
            printf("\t\tdst : 0x%lx\n", boot_data->csf.dst);
            printf("\t\tsize : 0x%x\n", boot_data->csf.size);
        }
    }

    return 0;
}

int dump_img_container(void)
{
    int i = 0, j = 0;
    int iCont = g_iContainer;

    for (i = 0; i < iCont; i++) {
        printf("Container%d:\n", i);
        if (NULL != container_opts[i].dcd_file)
            printf("\tDCD: %s\n", container_opts[i].dcd_file);
        if (NULL != container_opts[i].scd_file)
            printf("\tSCD: %s\n", container_opts[i].scd_file);
        if (NULL != container_opts[i].csf_file)
            printf("\tCSF: %s\n", container_opts[i].csf_file);
        for (j = 0; j < container_opts[i].img_num; j++) {
            uint32_t core = container_opts[i].img[j].core;
            printf("\tImage%d\n", j);
            if (NULL != container_opts[i].img[j].img_file)
                printf("\t\tImage_File: %s\n", container_opts[i].img[j].img_file);
            printf("\t\tCore: %s\n",
                   core == CORE_SC              ? "SCU"     :
                   core == CORE_CM4_0           ? "CM4_0"   :
                   core == CORE_CM4_1           ? "CM4_1"   :
                   core == CORE_SECO            ? "SECO"    :
                   core == CORE_CA53            ? "CA53"    :
                   core == CORE_CA72            ? "CA72"    :
                   core == CORE_HDMI_TX_uCPU    ? "HDMI_TX" :
                   core == CORE_HDMI_RX_uCPU    ? "HDMI_RX" :
                   "Invalid Core");
            printf("\t\tEntry: 0x%lx\n", container_opts[i].img[j].entry);
            printf("\t\tTO: 0x%lx\n", container_opts[i].img[j].to);
            printf("\t\tMU: 0x%x\n", container_opts[i].img[j].mu);
			printf("\t\tPART: 0x%x\n", container_opts[i].img[j].part);
			printf("\t\tCPU_ID: 0x%x\n", container_opts[i].img[j].cpu_id);
            printf("\t\tSize: 0x%x\n", container_opts[i].img[j].img_sz);
        }
    }

    return 0;
}

static int split_opt(char **cSplit, char* input_str)
{
	char *c = NULL;
	int l = 0;

	memset(cSplit, 0, sizeof(char *) * SPLIT_NUM_MAX);

	c = input_str;
	cSplit[l++] = c;
	while (*c != '\0') {
		if (*c == ':') {
			if (l < SPLIT_NUM_MAX)
				cSplit[l] = c + 1;
			*c = '\0';
			l++;
		}
		c++;
	}

	return l;
}

void print_help()
{
	printf("Usage:\n"
		   "\t-dcd DCD_FILE\n"
		   "\t\t\tSpecify the DCD script file in DCD_FILE\n\n"
		   "\t-csf CSF_FILE[:to=LOAD_ADDR]\n"
		   "\t\t\tSpecify the CSF binary file in CSF_FILE and its optional LOAD_ADDR from system's view. The default LOAD_ADDR is located after SCU image\n\n"
		   "\t-scd SCD_FILE[:to=LOAD_ADDR]\n"
		   "\t\t\tSpecify the SCD binary file in SCD_FILE and its optional LOAD_ADDR from system's view. The default LOAD_ADDR is located after SCU image\n\n"
		   "\t-img IMG_FILE:CORE_TYPE[:entry=ENTRY_ADDR][:to=LOAD_ADDR][:part=PARTITION_ID][:mu=MU_ID]\n"
		   "\t\t\tSpecify the Image file in IMG_FILE\n"
		   "\t\t\tCORE_TYPE  	The core runs the image. Valid values are: scu/ca53/ca72/cm4_0/cm4_1/seco/hdmi_tx/hdmi_rx\n"
		   "\t\t\tENTRY_ADDR  	The optional entry address for the image, the address is from the CORE's view\n"
		   "\t\t\t\t\tDefault values:\n"
		   "\t\t\t\t\t\tscu: 0x1FFE0000\n"
		   "\t\t\t\t\t\tcm4_0: 0x1FFE0000\n"
		   "\t\t\t\t\t\tcm4_1: 0x1FFE0000\n"
		   "\t\t\t\t\t\tca53/ca72: 0x80000000\n"
		   "\t\t\t\t\t\tseco/hdmi_tx/hdmi_rx: The address must be provided\n"
		   "\t\t\tLOAD_ADDR   	The optional load address for the image, the address is from the system's view\n"
		   "\t\t\t\t\tDefault values:\n"
		   "\t\t\t\t\t\tscu: 0x30FE0000\n"
		   "\t\t\t\t\t\tcm4_0: 0x34FE0000\n"
		   "\t\t\t\t\t\tcm4_1: 0x38FE0000\n"
		   "\t\t\t\t\t\tca53/ca72: 0x80000000\n"
		   "\t\t\t\t\t\tseco/hdmi_tx/hdmi_rx: The address must be provided\n"
		   "\t\t\tPARTITION_ID	This optional value is used to pass resource partition id for this core\n"
		   "\t\t\t\t\t\tFor scu core this parameter is not needed.\n"
		   "\t\t\t\t\t\tFor m4 cores the default value is 0\n"
		   "\t\t\t\t\t\tFor ca53/ca72 cores the default value is 1\n"
		   "\t\t\tMU_ID		This optional value is used to pass MU resource id for the communication between this core and SCU\n\n"
		   "\t-dev DEVICE_TYPE\n"
		   "\t\t\tSpecify the boot device the mkimage generated for\n"
		   "\t\t\tDEVICE_TYPE	Valid values are: nand/sd/sata/flexspi/i2c. sd is default\n\n");
}

int get_options(int argc, char **argv)
{
	int i = 1, j;
	int l = 0, m;
	int nRetCode = 0;
	char *cSplit[SPLIT_NUM_MAX];
	unsigned int lval = 0;
	unsigned int core = 0;
	boot_dev dev = BOOT_DEV_SD;
	int container_id;

	g_iContainer = 0;

	while ((i < argc) && (0 == nRetCode)) {
		container_id = -1;
		if (!strncmp(argv[i], "-img", 4) && (i + 1) < argc) {

			img_opts_t img_tmp;
			memset(&img_tmp, 0, sizeof(img_opts_t));

			i++;
			l = split_opt(cSplit, argv[i]);
			
			img_tmp.img_file = cSplit[0];
			for (m = 1; m < l; m++) {
				if (!strncmp(cSplit[1], "scu", 3)) {
					img_tmp.core = CORE_SC;
					container_id = 0;
					img_tmp.entry = DEFAULT_SCU_ENTRY_ADDR;
					img_tmp.to = DEFAULT_SCU_LOAD_ADDR;
				} else if (!strncmp(cSplit[1], "ca53", 4)) {
					img_tmp.core = CORE_CA53;
					container_id = 1;
					img_tmp.part = PARTITION_ID_AP;
					img_tmp.mu = SC_R_MU_0A;
					img_tmp.cpu_id = SC_R_A53_0;
					img_tmp.entry = DEFAULT_AP_ENTRY_ADDR;
					img_tmp.to = DEFAULT_AP_LOAD_ADDR;
				} else if (!strncmp(cSplit[1], "ca72", 4)) {
					img_tmp.core = CORE_CA72;
					container_id = 1;
					img_tmp.part = PARTITION_ID_AP;
					img_tmp.mu = SC_R_MU_0A;
					img_tmp.cpu_id = SC_R_A72_0;
					img_tmp.entry = DEFAULT_AP_ENTRY_ADDR;
					img_tmp.to = DEFAULT_AP_LOAD_ADDR;
				} else if (!strncmp(cSplit[1], "cm4_0", 5)) {
					img_tmp.core = CORE_CM4_0;
					container_id = 0;
					img_tmp.part = PARTITION_ID_M4;
					img_tmp.mu = SC_R_M4_0_MU_1A;
					img_tmp.cpu_id = SC_R_M4_0_PID0;
					img_tmp.entry = DEFAULT_M4_0_ENTRY_ADDR;
					img_tmp.to = DEFAULT_M4_0_LOAD_ADDR;
				} else if (!strncmp(cSplit[1], "cm4_1", 5)) {
					img_tmp.core = CORE_CM4_1;
					container_id = 0;
					img_tmp.part = PARTITION_ID_M4;
					img_tmp.mu = SC_R_M4_1_MU_1A;
					img_tmp.cpu_id = SC_R_M4_1_PID0;
					img_tmp.entry = DEFAULT_M4_1_ENTRY_ADDR;
					img_tmp.to = DEFAULT_M4_1_LOAD_ADDR;
				} else if (!strncmp(cSplit[1], "seco", 4)) {
					img_tmp.core = CORE_SECO;
					container_id = 0;
				} else if (!strncmp(cSplit[1], "hdmi_tx", 7)) {
					img_tmp.core = CORE_HDMI_TX_uCPU;
					container_id = 0;
				} else if (!strncmp(cSplit[1], "hdmi_rx", 7)) {
					img_tmp.core = CORE_HDMI_RX_uCPU;
					container_id = 0;
				} else {
					fprintf(stderr, "Invalid core type %s\n", cSplit[1]);
					print_help();
					exit(EXIT_FAILURE);
				}
				if (!strncmp(cSplit[m], "entry=", 6)) {
					sscanf(cSplit[m] + 6, "0x%x", &lval);
					img_tmp.entry = lval;
				}
				if (!strncmp(cSplit[m], "to=", 3)) {
					sscanf(cSplit[m] + 3, "0x%x", &lval);
					img_tmp.to = lval;
				}
				if (!strncmp(cSplit[m], "part=", 5)) {
					sscanf(cSplit[m] + 5, "0x%x", &img_tmp.part);
				}
				if (!strncmp(cSplit[m], "mu=", 3)) {
					sscanf(cSplit[m] + 3, "0x%x", &img_tmp.mu);
				}
			}

			container_opts[container_id].img[container_opts[container_id].img_num] = img_tmp;
			container_opts[container_id].img_num++;
			
		} else if (!strncmp(argv[i], "-dcd", 4) && (i + 1) < argc) {	
			i++;
			container_opts[0].dcd_file = argv[i];
			container_id = 0;
		} else if (!strncmp(argv[i], "-scd", 4) && (i + 1) < argc) {
			i++;
			l = split_opt(cSplit, argv[i]);

			container_opts[0].scd_file = cSplit[0];
			container_id = 0;

			if (l > 1 && !strncmp(cSplit[1], "to=", 3)) {
				sscanf(cSplit[1] + 3, "0x%x", &lval);
				container_opts[0].scd_to = lval;
			} else {
				mkimg_debug("Put scd load address after SCU image\n");
				container_opts[0].scd_to = 0;
			}

		} else if (!strncmp(argv[i], "-csf", 4) && (i + 1) < argc) {
			i++;
			l = split_opt(cSplit, argv[i]);

			container_opts[0].csf_file = cSplit[0];
			container_id = 0;

			if (l > 1 && !strncmp(cSplit[1], "to=", 3)) {
				sscanf(cSplit[1] + 3, "0x%x", &lval);
				container_opts[0].csf_to = lval;
			} else {
				mkimg_debug("Put csf load address after SCU image\n");
				container_opts[0].csf_to = 0;
			}

		} else if (!strncmp(argv[i], "-dev", 4) && (i + 1) < argc) {

			i++;
			if (!strncmp(argv[i], "nand", 4)) {
				dev = BOOT_DEV_NAND;
			} else if (!strncmp(argv[i], "sd", 2)) {
				dev = BOOT_DEV_SD;
			} else if (!strncmp(argv[i], "sata", 4)) {
				dev = BOOT_DEV_SATA;
			} else if (!strncmp(argv[i], "flexspi", 7)) {
				dev = BOOT_DEV_FLEXSPI;
			} else if (!strncmp(argv[i], "i2c", 3)) {
				dev = BOOT_DEV_I2C;
			} else {
				fprintf(stderr, "Invalid boot device type, should be sd/nand/sata/flexspi/i2c\n");
				exit(EXIT_FAILURE);
			}
		} else if (!strncmp(argv[i], "-help", 5)) {
			print_help();
			exit(0);
		} else {
			fprintf(stderr, "Invalid command %s\n", argv[i]);
			print_help();
			exit(EXIT_FAILURE);
		}

		i++;

		/* Update g_iContainer */
		if ((container_id + 1) > g_iContainer)
			g_iContainer = (container_id + 1);
	}

	misc_opts = boot_dev_opts[dev];

#ifdef DEBUG
	dump_img_container();
#endif

	/* Checking the validility of options. */
	if ((0 == container_opts[0].img_num)) {
		fprintf(stderr, "Container 0 must has at lease one image\n");
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < g_iContainer; i++) {
		for (j = 0; j < container_opts[i].img_num; j++) {
			core = container_opts[i].img[j].core;
			if (core != CORE_SC && core != CORE_CM4_0 && core != CORE_CM4_1 && core != CORE_SECO &&
				core != CORE_CA53 && core != CORE_CA72 && core != CORE_HDMI_TX_uCPU &&
				core != CORE_HDMI_RX_uCPU) {
				fprintf(stderr, "Invalid core type provisioned, shall be scu/cm4_0/cm4_1/ca53/ca72\n");
				exit(EXIT_FAILURE);
			}
			if (container_opts[i].img[j].img_file == NULL || container_opts[i].img[j].entry == 0 ||
				container_opts[i].img[j].to == 0) {
				fprintf(stderr, "image_file/entry/to must be provisioned for container%d img%d\n", i, j);
				exit(EXIT_FAILURE);
			}
		}
	}

	for (j = 0; j < container_opts[0].img_num; j++) {
		core = container_opts[0].img[j].core;
		if (core == CORE_SC || core == CORE_SECO ||
			core == CORE_HDMI_TX_uCPU || core == CORE_HDMI_RX_uCPU)
		break;
	}
	if (j >= container_opts[0].img_num) {
		fprintf(stderr, "scu or seco image must be provisioned in first container\n");
		exit(EXIT_FAILURE);
	} else {
		if((core == CORE_SECO) && (j != 0)){
			fprintf(stderr, "seco image must be the only one image in the first container\n");
			exit(EXIT_FAILURE);
		}
	}

	return nRetCode;
}

