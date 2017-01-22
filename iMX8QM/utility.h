#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "boot.h"

/* Uncomment this out for debug */
/* #define DEBUG */

#ifdef DEBUG
	#define mkimg_debug printf
#else
	#define mkimg_debug(...)
#endif

#define ALIGN(x,a)		__ALIGN_MASK((x),(__typeof__(x))(a)-1)
#define __ALIGN_MASK(x,mask)	(((x)+(mask))&~(mask))

#define uswap_16(x) \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8))
#define uswap_32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))
#define _uswap_64(x, sfx) \
	((((x) & 0xff00000000000000##sfx) >> 56) | \
	 (((x) & 0x00ff000000000000##sfx) >> 40) | \
	 (((x) & 0x0000ff0000000000##sfx) >> 24) | \
	 (((x) & 0x000000ff00000000##sfx) >>  8) | \
	 (((x) & 0x00000000ff000000##sfx) <<  8) | \
	 (((x) & 0x0000000000ff0000##sfx) << 24) | \
	 (((x) & 0x000000000000ff00##sfx) << 40) | \
	 (((x) & 0x00000000000000ff##sfx) << 56))

#if defined(__GNUC__)
# define uswap_64(x) _uswap_64(x, ull)
#else
#error
# define uswap_64(x) _uswap_64(x, )
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
# define cpu_to_le16(x)		(x)
# define cpu_to_le32(x)		(x)
# define cpu_to_le64(x)		(x)
# define le16_to_cpu(x)		(x)
# define le32_to_cpu(x)		(x)
# define le64_to_cpu(x)		(x)
# define cpu_to_be16(x)		uswap_16(x)
# define cpu_to_be32(x)		uswap_32(x)
# define cpu_to_be64(x)		uswap_64(x)
# define be16_to_cpu(x)		uswap_16(x)
# define be32_to_cpu(x)		uswap_32(x)
# define be64_to_cpu(x)		uswap_64(x)
#else
#error
# define cpu_to_le16(x)		uswap_16(x)
# define cpu_to_le32(x)		uswap_32(x)
# define cpu_to_le64(x)		uswap_64(x)
# define le16_to_cpu(x)		uswap_16(x)
# define le32_to_cpu(x)		uswap_32(x)
# define le64_to_cpu(x)		uswap_64(x)
# define cpu_to_be16(x)		(x)
# define cpu_to_be32(x)		(x)
# define cpu_to_be64(x)		(x)
# define be16_to_cpu(x)		(x)
# define be32_to_cpu(x)		(x)
# define be64_to_cpu(x)		(x)
#endif

#define UNDEFINED 0xFFFFFFFF

typedef struct {
    char *img_file;
	FILE *fp_img;
    unsigned char core;
    uint64_t entry;
    uint64_t to;
    uint32_t mu;
	uint32_t part;
	uint32_t cpu_id;
    uint32_t img_sz;
} img_opts_t;

typedef struct {
    char *dcd_file;
    char *scd_file;
    char *csf_file;
	FILE *fp_dcd;
	FILE *fp_scd;
	FILE *fp_csf;
	uint32_t dcd_sz;
	uint32_t scd_sz;
	uint32_t csf_sz;
    uint64_t scd_to;
    uint64_t csf_to;
    uint32_t img_num;
    img_opts_t img[MAX_NUM_IMGS];
    uint32_t hdr_base;
} img_container_opts_t;

typedef struct{
    uint32_t ivt_off;
    uint32_t image_off;
    uint32_t sector_size;
} img_misc_opts;

int dump_img_container(void);
int get_options(int argc, char **argv);
int dump_imx_header_v3(imx_header_v3_t * imx_header);


#endif

