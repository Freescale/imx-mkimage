#ifndef _BOOT_H_
#define _BOOT_H_

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t addr;
    uint32_t value;
} dcd_addr_data_t;

typedef struct {
    uint8_t tag;
    uint16_t length;
    uint8_t version;
} __attribute__((packed)) ivt_header_t;

typedef struct {
    uint8_t tag;
    uint16_t length;
    uint8_t param;
} __attribute__((packed)) write_dcd_command_t;

#define MAX_NUM_IMGS 4
#define MAX_HW_CFG_SIZE_V2 359 //231

struct dcd_v2_cmd {
    write_dcd_command_t write_dcd_command; /*4*/
    dcd_addr_data_t addr_data[MAX_HW_CFG_SIZE_V2]; /*2872*/
} __attribute__((packed));

typedef struct {
    ivt_header_t header;   /*4*/
    struct dcd_v2_cmd dcd_cmd; /*2876*/
} __attribute__((packed)) dcd_v2_t;            /*2880*/

typedef struct {
    uint64_t src;        /*8*/
    uint64_t dst;        /*8*/
    uint64_t entry;        /*8*/
    uint32_t size;        /*4*/
    uint32_t flags;        /*4*/
} __attribute__((packed)) boot_img_t;; /*32*/

#define CORE_SC             (1)
#define CORE_CM4_0          (2)
#define CORE_CM4_1          (3)
#define CORE_CA53           (4)
#define CORE_CA72           (5)
#define CORE_SECO	        (6)
#define CORE_HDMI_TX_uCPU   (7)
#define CORE_HDMI_RX_uCPU   (8)


#define BOOT_IMG_FLAGS_CORE_MASK            (0xF)
#define BOOT_IMG_FLAGS_CPU_RID_MASK         (0x3FF0)
#define BOOT_IMG_FLAGS_CPU_RID_SHIFT        (4)
#define BOOT_IMG_FLAGS_MU_RID_MASK          (0xFFC000)
#define BOOT_IMG_FLAGS_MU_RID_SHIFT         (14)
#define BOOT_IMG_FLAGS_PARTITION_ID_MASK    (0x1F000000)
#define BOOT_IMG_FLAGS_PARTITION_ID_SHIFT   (24)


typedef enum sc_rsrc_e {
    SC_R_A53_0              = 1,
    SC_R_A72_0              = 6,

    SC_R_MU_0A              = 213,
    SC_R_MU_1A              = 214,
    SC_R_MU_2A              = 215,
    SC_R_MU_3A              = 216,
    SC_R_MU_4A              = 217,

    SC_R_M4_0_PID0          = 278,
    SC_R_M4_0_MU_1A         = 297,
    SC_R_M4_1_PID0          = 298,
    SC_R_M4_1_MU_1A         = 317,

	SC_R_SECO               = 499,
	SC_R_SECO_MU_2          = 503,
    SC_R_SECO_MU_3          = 504,
    SC_R_SECO_MU_4          = 505,
} sc_rsrc_t;

#define PARTITION_ID_M4    0
#define PARTITION_ID_AP    1

typedef struct {
    uint32_t num_images;            /*4*/
    uint32_t bd_size;               /*4*/
    uint32_t bd_flags;              /*4*/
    uint32_t reserved;              /*4*/
    boot_img_t img[MAX_NUM_IMGS];   /*4*32=128*/
    boot_img_t scd;                 /*32*/
    boot_img_t csf;                 /*32*/
    boot_img_t img_reserved;        /* Reserved for future, 32 */
}  __attribute__((packed)) boot_data_v3_t;

typedef struct {
    ivt_header_t header;    /*4*/
    uint32_t reserved1;    /*4*/
    uint64_t dcd_ptr;    /*8*/
    uint64_t boot_data_ptr;    /*8*/
    uint64_t self;        /*8*/
    uint64_t csf;        /*8*/
    uint64_t scd;        /*8*/
    uint64_t reserved2;    /*8*/
    uint64_t reserved3;    /*8*/
}  __attribute__((packed)) flash_header_v3_t;        /*64*/

#define MAX_NUM_OF_CONTAINER    2

typedef struct {
    flash_header_v3_t fhdr[MAX_NUM_OF_CONTAINER];    /*64*/
    boot_data_v3_t boot_data[MAX_NUM_OF_CONTAINER]; /*128*/
    dcd_v2_t dcd_table; /*2880*/
}  __attribute__((packed)) imx_header_v3_t; /*3072*/

/* Command tags and parameters */
#define IVT_HEADER_TAG            0xD1
#define IVT_VERSION            0x43
#define DCD_HEADER_TAG            0xD2
#define DCD_VERSION            0x43
#define DCD_WRITE_DATA_COMMAND_TAG    0xCC
#define DCD_WRITE_DATA_PARAM        0x4
#define DCD_WRITE_CLR_BIT_PARAM        0xC
#define DCD_CHECK_DATA_COMMAND_TAG    0xCF
#define DCD_CHECK_BITS_SET_PARAM    0x14
#define DCD_CHECK_BITS_CLR_PARAM    0x04

#define INITIAL_LOAD_IMG_SZ        0x2000

#endif
