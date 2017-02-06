/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 * derived from u-boot's mkimage utility
 *
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

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

#define MAX_NUM_IMGS 3
#define MAX_HW_CFG_SIZE_V2 359
struct dcd_v2_cmd {
	write_dcd_command_t write_dcd_command; /*4*/
	dcd_addr_data_t addr_data[MAX_HW_CFG_SIZE_V2]; /*2872*/
} __attribute__((packed));

typedef struct {
	ivt_header_t header;   /*4*/
	struct dcd_v2_cmd dcd_cmd; /*2876*/
} __attribute__((packed)) dcd_v2_t;			/*2880*/

typedef struct {
	uint64_t src;		/*8*/
	uint64_t dst;		/*8*/
	uint64_t entry;		/*8*/
	uint32_t size;		/*4*/
	uint32_t flags;		/*4*/
} __attribute__((packed)) boot_img_t;; /*32*/

#define CORE_SC 	1
#define CORE_CM4_0 	2
#define CORE_CM4_1 	3
#define CORE_CA53 	4
#define CORE_CA72 	5

#define BOOT_IMG_FLAGS_CORE_MASK	0xF
#define BOOT_IMG_FLAGS_CPU_RID_MASK	0x3FF0
#define BOOT_IMG_FLAGS_CPU_RID_SHIFT	4
#define BOOT_IMG_FLAGS_MU_RID_MASK	0xFFC000
#define BOOT_IMG_FLAGS_MU_RID_SHIFT	14
#define BOOT_IMG_FLAGS_PARTITION_ID_MASK	0x1F000000
#define BOOT_IMG_FLAGS_PARTITION_ID_SHIFT	24

#define SC_R_A53_0	1
#define SC_R_A72_0	6
#define SC_R_MU_0A	213
#define SC_R_M4_0_PID0	278
#define SC_R_M4_0_MU_1A	297
#define SC_R_M4_1_PID0	298
#define SC_R_M4_1_MU_1A 317
#define PARTITION_ID_M4	0
#define PARTITION_ID_AP	1

typedef struct {
	uint32_t num_images;	/*4*/
	uint32_t bd_size;	/*4*/
	uint32_t bd_flags;	/*4*/
	uint32_t reserved;	/*4*/
	boot_img_t img[MAX_NUM_IMGS];	/*96*/
	uint32_t padding[4];    /* end up on an 8-byte boundary */
}  __attribute__((packed)) boot_data_v3_t;		/*128*/

typedef struct {
	ivt_header_t header;	/*4*/
	uint32_t reserved1;	/*4*/
	uint64_t dcd_ptr;	/*8*/
	uint64_t boot_data_ptr;	/*8*/
	uint64_t self;		/*8*/
	uint64_t csf;		/*8*/
	uint64_t scd;		/*8*/
	uint64_t reserved2;	/*8*/
	uint64_t reserved3;	/*8*/
}  __attribute__((packed)) flash_header_v3_t;		/*64*/

typedef struct {
	flash_header_v3_t fhdr;	/*64*/
	boot_data_v3_t boot_data; /*128*/
	dcd_v2_t dcd_table; /*2880*/
}  __attribute__((packed)) imx_header_v3_t; /*3072*/

/* Command tags and parameters */
#define IVT_HEADER_TAG			0xD1
#define IVT_VERSION			0x43
#define DCD_HEADER_TAG			0xD2
#define DCD_VERSION			0x43
#define DCD_WRITE_DATA_COMMAND_TAG	0xCC
#define DCD_WRITE_DATA_PARAM		0x4
#define DCD_WRITE_CLR_BIT_PARAM		0xC
#define DCD_CHECK_DATA_COMMAND_TAG	0xCF
#define DCD_CHECK_BITS_SET_PARAM	0x14
#define DCD_CHECK_BITS_CLR_PARAM	0x04

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

static void
copy_file (int ifd, const char *datafile, int pad, int offset)
{
	int dfd;
	struct stat sbuf;
	unsigned char *ptr;
	int tail;
	int zero = 0;
	uint8_t zeros[4096];
	int size;

	memset(zeros, 0, sizeof(zeros));

	if ((dfd = open(datafile, O_RDONLY|O_BINARY)) < 0) {
		fprintf (stderr, "Can't open %s: %s\n",
			datafile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	if (fstat(dfd, &sbuf) < 0) {
		fprintf (stderr, "Can't stat %s: %s\n",
			datafile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	ptr = mmap(0, sbuf.st_size, PROT_READ, MAP_SHARED, dfd, 0);
	if (ptr == MAP_FAILED) {
		fprintf (stderr, "Can't read %s: %s\n",
			datafile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	size = sbuf.st_size;
	lseek(ifd, offset, SEEK_SET);
	if (write(ifd, ptr, size) != size) {
		fprintf (stderr, "Write error %s\n",
			strerror(errno));
		exit (EXIT_FAILURE);
	}

	tail = size % 4;
	pad = pad - size;
	if ((pad == 1) && (tail != 0)) {

		if (write(ifd, (char *)&zero, 4-tail) != 4-tail) {
			fprintf (stderr, "Write error on %s\n",
				strerror(errno));
			exit (EXIT_FAILURE);
		}
	} else if (pad > 1) {
		while (pad > 0) {
			int todo = sizeof(zeros);

			if (todo > pad)
				todo = pad;
			if (write(ifd, (char *)&zeros, todo) != todo) {
				fprintf(stderr, "Write error: %s\n",
					strerror(errno));
				exit(EXIT_FAILURE);
			}
			pad -= todo;
		}
	}

	(void) munmap((void *)ptr, sbuf.st_size);
	(void) close (dfd);
}

enum imximage_fld_types {
	CFG_INVALID = -1,
	CFG_COMMAND,
	CFG_REG_SIZE,
	CFG_REG_ADDRESS,
	CFG_REG_VALUE
};

enum imximage_cmd {
	CMD_INVALID,
	CMD_IMAGE_VERSION,
	CMD_BOOT_FROM,
	CMD_BOOT_OFFSET,
	CMD_WRITE_DATA,
	CMD_WRITE_CLR_BIT,
	CMD_CHECK_BITS_SET,
	CMD_CHECK_BITS_CLR,
	CMD_CSF,
	CMD_PLUGIN,
};

typedef struct table_entry {
	int	id;
	char	*sname;		/* short (input) name to find table entry */
	char	*lname;		/* long (output) name to print for messages */
} table_entry_t;

/*
 * Supported commands for configuration file
 */
static table_entry_t imximage_cmds[] = {
	{CMD_BOOT_FROM,         "BOOT_FROM",            "boot command",	  },
	{CMD_BOOT_OFFSET,       "BOOT_OFFSET",          "Boot offset",	  },
	{CMD_WRITE_DATA,        "DATA",                 "Reg Write Data", },
	{CMD_WRITE_CLR_BIT,     "CLR_BIT",              "Reg clear bit",  },
	{CMD_CHECK_BITS_SET,    "CHECK_BITS_SET",   "Reg Check bits set", },
	{CMD_CHECK_BITS_CLR,    "CHECK_BITS_CLR",   "Reg Check bits clr", },
	{CMD_CSF,               "CSF",           "Command Sequence File", },
	{CMD_IMAGE_VERSION,     "IMAGE_VERSION",        "image version",  },
	{-1,                    "",                     "",	          },
};

static uint32_t imximage_version;
static struct dcd_v2_cmd *gd_last_cmd;
static uint32_t imximage_ivt_offset = UNDEFINED;
static uint32_t imximage_csf_size = UNDEFINED;

int get_table_entry_id(const table_entry_t *table,
		const char *table_name, const char *name)
{
	const table_entry_t *t;

	for (t = table; t->id >= 0; ++t) {
		if (t->sname && strcasecmp(t->sname, name) == 0)
			return (t->id);
	}

	return -1;
}

static uint32_t get_cfg_value(char *token, char *name,  int linenr)
{
	char *endptr;
	uint32_t value;

	errno = 0;
	value = strtoul(token, &endptr, 16);
	if (errno || (token == endptr)) {
		fprintf(stderr, "Error: %s[%d] - Invalid hex data(%s)\n",
			name,  linenr, token);
		exit(EXIT_FAILURE);
	}
	return value;
}

static void set_imx_hdr_v3(imx_header_v3_t *imxhdr, uint32_t dcd_len,
		uint32_t flash_offset)
{
	flash_header_v3_t *fhdr_v3 = &imxhdr->fhdr;
	uint32_t hdr_base = 0;

	/* Set magic number */
	fhdr_v3->header.tag = IVT_HEADER_TAG; /* 0xD1 */
	fhdr_v3->header.length = cpu_to_be16(sizeof(flash_header_v3_t));
	fhdr_v3->header.version = IVT_VERSION; /* 0x40 */

	fhdr_v3->reserved1 = fhdr_v3->reserved2 = fhdr_v3->reserved3 = 0;

	fhdr_v3->self = hdr_base;
	if (dcd_len > 0)
		fhdr_v3->dcd_ptr = hdr_base +
			offsetof(imx_header_v3_t, dcd_table);
	else
		fhdr_v3->dcd_ptr = 0;
	fhdr_v3->boot_data_ptr = hdr_base
			+ offsetof(imx_header_v3_t, boot_data);

	fhdr_v3->csf = 0;
}

static void set_dcd_param_v2(imx_header_v3_t *imxhdr, uint32_t dcd_len,
		int32_t cmd)
{
	dcd_v2_t *dcd_v2 = &imxhdr->dcd_table;
	struct dcd_v2_cmd *d = gd_last_cmd;
	struct dcd_v2_cmd *d2;
	int len;

	if (!d)
		d = &dcd_v2->dcd_cmd;
	d2 = d;
	len = be16_to_cpu(d->write_dcd_command.length);
	if (len > 4)
		d2 = (struct dcd_v2_cmd *)(((char *)d) + len);

	switch (cmd) {
	case CMD_WRITE_DATA:
		if ((d->write_dcd_command.tag == DCD_WRITE_DATA_COMMAND_TAG) &&
		    (d->write_dcd_command.param == DCD_WRITE_DATA_PARAM))
			break;
		d = d2;
		d->write_dcd_command.tag = DCD_WRITE_DATA_COMMAND_TAG;
		d->write_dcd_command.length = cpu_to_be16(4);
		d->write_dcd_command.param = DCD_WRITE_DATA_PARAM;
		break;
	case CMD_WRITE_CLR_BIT:
		if ((d->write_dcd_command.tag == DCD_WRITE_DATA_COMMAND_TAG) &&
		    (d->write_dcd_command.param == DCD_WRITE_CLR_BIT_PARAM))
			break;
		d = d2;
		d->write_dcd_command.tag = DCD_WRITE_DATA_COMMAND_TAG;
		d->write_dcd_command.length = cpu_to_be16(4);
		d->write_dcd_command.param = DCD_WRITE_CLR_BIT_PARAM;
		break;
	/*
	 * Check data command only supports one entry,
	 */
	case CMD_CHECK_BITS_SET:
		d = d2;
		d->write_dcd_command.tag = DCD_CHECK_DATA_COMMAND_TAG;
		d->write_dcd_command.length = cpu_to_be16(4);
		d->write_dcd_command.param = DCD_CHECK_BITS_SET_PARAM;
		break;
	case CMD_CHECK_BITS_CLR:
		d = d2;
		d->write_dcd_command.tag = DCD_CHECK_DATA_COMMAND_TAG;
		d->write_dcd_command.length = cpu_to_be16(4);
		d->write_dcd_command.param = DCD_CHECK_BITS_CLR_PARAM;
		break;
	default:
		break;
	}
	gd_last_cmd = d;
}

static void set_dcd_val_v2(imx_header_v3_t *imxhdr, char *name, int lineno,
					int fld, uint32_t value, uint32_t off)
{
	struct dcd_v2_cmd *d = gd_last_cmd;
	int len;

	len = be16_to_cpu(d->write_dcd_command.length);
	off = (len - 4) >> 3;

	switch (fld) {
	case CFG_REG_ADDRESS:
		d->addr_data[off].addr = cpu_to_be32(value);
		break;
	case CFG_REG_VALUE:
		d->addr_data[off].value = cpu_to_be32(value);
		off++;
		d->write_dcd_command.length = cpu_to_be16((off << 3) + 4);
		break;
	default:
		break;

	}
}

static void set_dcd_rst_v2(imx_header_v3_t *imxhdr, uint32_t dcd_len,
						char *name, int lineno)
{
	dcd_v2_t *dcd_v2 = &imxhdr->dcd_table;
	struct dcd_v2_cmd *d = gd_last_cmd;
	int len;

	if (!d)
		d = &dcd_v2->dcd_cmd;
	len = be16_to_cpu(d->write_dcd_command.length);
	if (len > 4)
		d = (struct dcd_v2_cmd *)(((char *)d) + len);

	len = (char *)d - (char *)&dcd_v2->header;

	dcd_v2->header.tag = DCD_HEADER_TAG;
	dcd_v2->header.length = cpu_to_be16(len);
	dcd_v2->header.version = DCD_VERSION;
}

static void parse_cfg_cmd(imx_header_v3_t *imxhdr, int32_t cmd, char *token,
				char *name, int lineno, int fld, int dcd_len)
{
	int value;
	static int cmd_ver_first = ~0;

	switch (cmd) {
	case CMD_IMAGE_VERSION:
		imximage_version = get_cfg_value(token, name, lineno);
		if (cmd_ver_first == 0) {
			fprintf(stderr, "Error: %s[%d] - IMAGE_VERSION "
				"command need be the first before other "
				"valid command in the file\n", name, lineno);
			exit(EXIT_FAILURE);
		}
		cmd_ver_first = 1;
		break;
	case CMD_BOOT_OFFSET:
		imximage_ivt_offset = get_cfg_value(token, name, lineno);
		if (cmd_ver_first != 1)
			cmd_ver_first = 0;
		break;
	case CMD_WRITE_DATA:
	case CMD_WRITE_CLR_BIT:
	case CMD_CHECK_BITS_SET:
	case CMD_CHECK_BITS_CLR:
		value = get_cfg_value(token, name, lineno);
		set_dcd_param_v2(imxhdr, dcd_len, cmd);
		set_dcd_val_v2(imxhdr, name, lineno, fld, value, dcd_len);
		if (cmd_ver_first != 1)
			cmd_ver_first = 0;
		break;
	case CMD_CSF:
		if (imximage_version != 2) {
			fprintf(stderr,
				"Error: %s[%d] - CSF only supported for VERSION 2(%s)\n",
				name, lineno, token);
			exit(EXIT_FAILURE);
		}
		imximage_csf_size = get_cfg_value(token, name, lineno);
		if (cmd_ver_first != 1)
			cmd_ver_first = 0;
		break;
	}
}

static void parse_cfg_fld(imx_header_v3_t *imxhdr, int32_t *cmd,
		char *token, char *name, int lineno, int fld, int *dcd_len)
{
	int value;

	switch (fld) {
	case CFG_COMMAND:
		*cmd = get_table_entry_id(imximage_cmds,
			"imximage commands", token);
		if (*cmd < 0) {
			fprintf(stderr, "Error: %s[%d] - Invalid command"
			"(%s)\n", name, lineno, token);
			exit(EXIT_FAILURE);
		}
		break;
	case CFG_REG_SIZE:
		parse_cfg_cmd(imxhdr, *cmd, token, name, lineno, fld, *dcd_len);
		break;
	case CFG_REG_ADDRESS:
	case CFG_REG_VALUE:
		switch(*cmd) {
		case CMD_WRITE_DATA:
		case CMD_WRITE_CLR_BIT:
		case CMD_CHECK_BITS_SET:
		case CMD_CHECK_BITS_CLR:

			value = get_cfg_value(token, name, lineno);
			set_dcd_param_v2(imxhdr, *dcd_len, *cmd);
			set_dcd_val_v2(imxhdr, name, lineno, fld, value,
					*dcd_len);

			if (fld == CFG_REG_VALUE) {
				(*dcd_len)++;
				if (*dcd_len > MAX_HW_CFG_SIZE_V2) {
					fprintf(stderr, "Error: %s[%d] -"
						"DCD table exceeds maximum size(%d)\n",
						name, lineno, MAX_HW_CFG_SIZE_V2);
					exit(EXIT_FAILURE);
				}
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

static uint32_t parse_cfg_file(imx_header_v3_t *imxhdr, char *name)
{
	FILE *fd = NULL;
	char *line = NULL;
	char *token, *saveptr1, *saveptr2;
	int lineno = 0;
	int fld;
	size_t len;
	int dcd_len = 0;
	int32_t cmd;

	fd = fopen(name, "r");
	if (fd == 0) {
		fprintf(stderr, "Error: %s - Can't open DCD file\n", name);
		exit(EXIT_FAILURE);
	}

	/*
	 * Very simple parsing, line starting with # are comments
	 * and are dropped
	 */
	while ((getline(&line, &len, fd)) > 0) {
		lineno++;

		token = strtok_r(line, "\r\n", &saveptr1);
		if (token == NULL)
			continue;

		/* Check inside the single line */
		for (fld = CFG_COMMAND, cmd = CMD_INVALID,
				line = token; ; line = NULL, fld++) {
			token = strtok_r(line, " \t", &saveptr2);
			if (token == NULL)
				break;

			/* Drop all text starting with '#' as comments */
			if (token[0] == '#')
				break;

			parse_cfg_fld(imxhdr, &cmd, token, name,
					lineno, fld, &dcd_len);
		}

	}

	set_dcd_rst_v2(imxhdr, dcd_len, name, lineno);
	fclose(fd);

	/* Exit if there is no BOOT_FROM field specifying the flash_offset */
	if (imximage_ivt_offset == UNDEFINED) {
		fprintf(stderr, "Error: No BOOT_FROM tag in %s\n", name);
		exit(EXIT_FAILURE);
	}
	return dcd_len;
}

int main(int argc, char **argv)
{
	int c, scfw_file_size, cm4_file_size = 0, scfw_fd = -1, cm4_fd = -1, ap_fd = -1, ofd = -1;
	unsigned int dcd_len = 0, cm4_core = 0, cm4_start_addr = 0, ap_start_addr = 0, ap_core = 0;
	char *ofname=NULL, *scfw_img = NULL, *dcd_img = NULL, *cm4_img = NULL, *ap_img = NULL;
    uint32_t flags = 0;
	static imx_header_v3_t imx_header;
	struct stat sbuf;

	static struct option long_options[] =
	{
		{"scfw", required_argument, NULL, 's'},
		{"m4", required_argument, NULL, 'm'},
		{"ap", required_argument, NULL, 'a'},
		{"dcd", required_argument, NULL, 'd'},
		{"out", required_argument, NULL, 'o'},
		{"flags", required_argument, NULL, 'f'},
		{NULL, 0, NULL, 0}
	};


	//fprintf(stderr, "imx_header_v3_t size %lu boot_data_v3_t = %lu flash_header_v3_t = %lu\n",
	//	sizeof(imx_header_v3_t), sizeof(boot_data_v3_t), sizeof(flash_header_v3_t));

	while(1)
    	{
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long_only (argc, argv, ":s:d:m:a:o:f:",
			long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
		break;

		switch (c)
		{
			case 0:
				fprintf(stderr, "option %s", long_options[option_index].name);
				if (optarg)
					fprintf(stderr, " with arg %s", optarg);
				fprintf(stderr, "\n");
				break;
			case 's':
				fprintf(stderr, "SCFW:\t%s\n", optarg);
				scfw_img = optarg;
				break;
			case 'd':
				fprintf(stderr, "DCD:\t%s\n", optarg);
				dcd_img = optarg;
				break;
			case 'm':
				fprintf(stderr, "CM4:\t%s", optarg);
				cm4_img = optarg;
				if ((optind < argc && *argv[optind] != '-') && (optind+1 < argc &&*argv[optind+1] != '-' )) {
					cm4_core = strtol(argv[optind++], NULL, 0);
					cm4_start_addr = (uint32_t) strtoll(argv[optind++], NULL, 0);
					fprintf(stderr, "\tcore: %d", cm4_core);
					fprintf(stderr, " addr: 0x%08x\n", cm4_start_addr);
				} else {
					fprintf(stderr, "\n-m4 option require THREE arguments: filename, core: 0/1, start address in hex\n\n");
					exit(1);
				}
				break;
			case 'a':
				fprintf(stderr, "AP:\t%s", optarg);
				ap_img = optarg;
				if ((optind < argc && *argv[optind] != '-') && (optind+1 < argc &&*argv[optind+1] != '-' )) {
					if(!strncmp(argv[optind++], "a53", 3))
						ap_core = CORE_CA53;
					else
						ap_core = CORE_CA72;

					ap_start_addr = (uint32_t) strtoll(argv[optind++], NULL, 0);

					fprintf(stderr, "\tcore: %s",   (ap_core == CORE_CA53)? "a53":"a72");
					fprintf(stderr, " addr: 0x%08x\n", ap_start_addr);
				} else {
					fprintf(stderr, "\n-ap option require THREE arguments: filename, a53/a72, start address in hex\n\n");
					exit(1);
				}
				break;
			case 'f':
				fprintf(stderr, "FLAG:\t%s\n", optarg);
    			flags = (uint32_t) strtoll(optarg, NULL, 0);
				break;
			case 'o':
				fprintf(stderr, "Output:\t%s\n", optarg);
				ofname = optarg;
				break;
			case ':':
				fprintf(stderr, "option %c missing arguments\n", optopt);
				break;
			case '?':
			default:
				/* invalid option */
				fprintf(stderr, "option '%c' is invalid: ignored\n",
					optopt);
				exit(1);
		}
	}

	if((scfw_img == NULL) || (ofname == NULL))
	{
		fprintf(stderr, "mandatory args scfw and output file name missing! abort\n");
		exit(1);
	}

	scfw_fd = open(scfw_img, O_RDONLY | O_BINARY);
	if (scfw_fd < 0) {
		fprintf(stderr, "%s: Can't open: %s\n",
                                scfw_img, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (fstat(scfw_fd, &sbuf) < 0) {
		fprintf(stderr, "%s: Can't stat: %s\n",
				scfw_img, strerror(errno));
		exit(EXIT_FAILURE);
	}
	close(scfw_fd);

	if(dcd_img) {
		dcd_len = parse_cfg_file(&imx_header, dcd_img);
	        fprintf(stderr, "dcd len = %d\n", dcd_len);
	}

	set_imx_hdr_v3(&imx_header, dcd_len, 0);

	scfw_file_size = sbuf.st_size;
	fprintf(stderr, "scfw size = %d\n", scfw_file_size);

	imx_header.boot_data.img[0].src = ALIGN(sizeof(imx_header_v3_t), 512);
	imx_header.boot_data.img[0].dst = 0x30fe0000;
	imx_header.boot_data.img[0].entry = 0x1ffe0000;
	imx_header.boot_data.img[0].size = scfw_file_size;
	imx_header.boot_data.img[0].flags = 0;
	imx_header.boot_data.img[0].flags = (CORE_SC & BOOT_IMG_FLAGS_CORE_MASK);
	imx_header.boot_data.num_images++;
	imx_header.boot_data.bd_size = sizeof(boot_data_v3_t);
	imx_header.boot_data.bd_flags = flags;

	if(cm4_img) {
		cm4_fd = open(cm4_img, O_RDONLY | O_BINARY);
		if (cm4_fd < 0) {
			fprintf(stderr, "%s: Can't open: %s\n",
                                cm4_img, strerror(errno));
			exit(EXIT_FAILURE);
		}

		if (fstat(cm4_fd, &sbuf) < 0) {
			fprintf(stderr, "%s: Can't stat: %s\n",
				cm4_img, strerror(errno));
			exit(EXIT_FAILURE);
		}
		close(cm4_fd);

        	cm4_file_size = sbuf.st_size;
		//fprintf(stderr, "cm4 size = %d\n", (int)sbuf.st_size);

		imx_header.boot_data.img[1].src = imx_header.boot_data.img[0].src + ALIGN(scfw_file_size, 512);
		imx_header.boot_data.img[1].dst = cm4_start_addr;
		imx_header.boot_data.img[1].entry = cm4_start_addr;
		imx_header.boot_data.img[1].size = sbuf.st_size;
		imx_header.boot_data.num_images++;	

		if(cm4_core == 0) {
			if(cm4_start_addr == 0x38fe0000) {
				fprintf(stderr, "! Invalid CM4_0 start address\n");
				exit(EXIT_FAILURE);
			}
			imx_header.boot_data.img[1].flags = (CORE_CM4_0 & BOOT_IMG_FLAGS_CORE_MASK);
			imx_header.boot_data.img[1].flags |= (SC_R_M4_0_PID0 << BOOT_IMG_FLAGS_CPU_RID_SHIFT);
			imx_header.boot_data.img[1].flags |= (SC_R_M4_0_MU_1A << BOOT_IMG_FLAGS_MU_RID_SHIFT);
			imx_header.boot_data.img[1].flags |= (PARTITION_ID_M4 << BOOT_IMG_FLAGS_PARTITION_ID_SHIFT);
		}
		else {
			if(cm4_start_addr == 0x34fe0000) {
				fprintf(stderr, "! Invalid CM4_1 start address\n");
				exit(EXIT_FAILURE);
			}
			imx_header.boot_data.img[1].flags = (CORE_CM4_1 & BOOT_IMG_FLAGS_CORE_MASK);
			imx_header.boot_data.img[1].flags |= (SC_R_M4_1_PID0 << BOOT_IMG_FLAGS_CPU_RID_SHIFT);
			imx_header.boot_data.img[1].flags |= (SC_R_M4_1_MU_1A << BOOT_IMG_FLAGS_MU_RID_SHIFT);
			imx_header.boot_data.img[1].flags |= (PARTITION_ID_M4 << BOOT_IMG_FLAGS_PARTITION_ID_SHIFT);
		}
	}

	if((ap_core == CORE_CA72) || (ap_core == CORE_CA53))
	{
		//printf("Note: Ignoring CM4_0 and CA53 images\n");
		//printf("Special case for DDR stress test tool\n");

		if(strncmp(ap_img, "null", 4)) {
			ap_fd = open(ap_img, O_RDONLY | O_BINARY);
			if (ap_fd < 0) {
				fprintf(stderr, "%s: Can't open: %s\n",
                                	ap_img, strerror(errno));
				exit(EXIT_FAILURE);
			}
			if (fstat(ap_fd, &sbuf) < 0) {
				fprintf(stderr, "%s: Can't stat: %s\n",
					ap_img, strerror(errno));
				exit(EXIT_FAILURE);
			}
			close(ap_fd);

			imx_header.boot_data.img[2].src = imx_header.boot_data.img[0].src + ALIGN(scfw_file_size, 512) + ALIGN(cm4_file_size, 512);
			imx_header.boot_data.img[2].dst = ap_start_addr;
			imx_header.boot_data.img[2].size = sbuf.st_size;
			imx_header.boot_data.img[2].entry = ap_start_addr;
			imx_header.boot_data.num_images++;	
			//fprintf(stderr, "ap img size = %d\n", (int)sbuf.st_size);
		}
		else {
			imx_header.boot_data.img[2].src = 0;
			imx_header.boot_data.img[2].dst = 0;
			imx_header.boot_data.img[2].size = 0;
			imx_header.boot_data.img[2].entry = 0;
		}

		imx_header.boot_data.img[2].flags = (ap_core & BOOT_IMG_FLAGS_CORE_MASK);
		if (ap_core == CORE_CA53)
			imx_header.boot_data.img[2].flags |= (SC_R_A53_0 << BOOT_IMG_FLAGS_CPU_RID_SHIFT);
		else
			imx_header.boot_data.img[2].flags |= (SC_R_A72_0 << BOOT_IMG_FLAGS_CPU_RID_SHIFT);

		imx_header.boot_data.img[2].flags |= (SC_R_MU_0A << BOOT_IMG_FLAGS_MU_RID_SHIFT);
		imx_header.boot_data.img[2].flags |= (PARTITION_ID_AP << BOOT_IMG_FLAGS_PARTITION_ID_SHIFT);
	}

	/* Open output file */
	ofd = open (ofname, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, 0666);
	if (ofd < 0) {
		fprintf(stderr, "%s: Can't open: %s\n",
                                ofname, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Write image header */
	if (write(ofd, &imx_header, sizeof(imx_header_v3_t)) != sizeof(imx_header_v3_t)) {
		fprintf(stderr, "error writing image hdr\n");
		exit(1);
	}

	/* Write SCFW after header */
	copy_file(ofd, scfw_img, 0, imx_header.boot_data.img[0].src);

	/* Write CM4 image after SCFW */
	if(cm4_img) {
		copy_file(ofd, cm4_img, 0, imx_header.boot_data.img[1].src);
	}

	/* Write AP image (if present) after CM4 */
	if(ap_img && strncmp(ap_img, "null", 4)) {
		copy_file(ofd, ap_img, 0, imx_header.boot_data.img[2].src);
	}

	/* Close output file */
	close(ofd);

	fprintf(stderr, "done.\n");

	return 0;
}

