
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

#include "utility.h"
#include "boot.h"

#define CSF_DATA_SIZE       (0x4000)
#define INITIAL_LOAD_ADDR_SCU_ROM 0x3100e000
#define INITIAL_LOAD_ADDR_AP_ROM 0x00110000
#define IMG_AUTO_ALIGN 0x10

imx_header_v3_t imx_header;
img_container_opts_t container_opts[MAX_NUM_OF_CONTAINER];
img_misc_opts    misc_opts;
unsigned int g_iContainer;

enum imximage_fld_types {
	CFG_INVALID = -1,
	CFG_COMMAND,
	CFG_REG_SIZE,
	CFG_REG_ADDRESS,
	CFG_REG_VALUE
};

enum imximage_cmd {
	CMD_INVALID,
	CMD_WRITE_DATA,
	CMD_WRITE_CLR_BIT,
	CMD_CHECK_BITS_SET,
	CMD_CHECK_BITS_CLR,
};

typedef struct table_entry {
	int	id;
	char *sname;		/* short (input) name to find table entry */
	char *lname;		/* long (output) name to print for messages */
} table_entry_t;

/*
 * Supported commands for configuration file
 */
static table_entry_t imximage_cmds[] = {
	{CMD_WRITE_DATA,        "DATA",                 "Reg Write Data", },
	{CMD_WRITE_CLR_BIT,     "CLR_BIT",              "Reg clear bit",  },
	{CMD_CHECK_BITS_SET,    "CHECK_BITS_SET",   "Reg Check bits set", },
	{CMD_CHECK_BITS_CLR,    "CHECK_BITS_CLR",   "Reg Check bits clr", },
	{-1,                    "",                     "",	          },
};

static struct dcd_v2_cmd *gd_last_cmd;

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

static void set_dcd_param_v2(dcd_v2_t *dcd_v2, uint32_t dcd_len,
		int32_t cmd)
{
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

static void set_dcd_val_v2(char *name, int lineno,
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

static void set_dcd_rst_v2(dcd_v2_t *dcd_v2, uint32_t dcd_len,
						char *name, int lineno)
{
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

static void parse_cfg_cmd(dcd_v2_t *dcd_v2, int32_t cmd, char *token,
				char *name, int lineno, int fld, int dcd_len)
{
	int value;
	static int cmd_ver_first = ~0;

	switch (cmd) {
	case CMD_WRITE_DATA:
	case CMD_WRITE_CLR_BIT:
	case CMD_CHECK_BITS_SET:
	case CMD_CHECK_BITS_CLR:
		value = get_cfg_value(token, name, lineno);
		set_dcd_param_v2(dcd_v2, dcd_len, cmd);
		set_dcd_val_v2(name, lineno, fld, value, dcd_len);
		if (cmd_ver_first != 1)
			cmd_ver_first = 0;
		break;
	}
}

static void parse_cfg_fld(dcd_v2_t *dcd_v2, int32_t *cmd,
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
		parse_cfg_cmd(dcd_v2, *cmd, token, name, lineno, fld, *dcd_len);
		break;
	case CFG_REG_ADDRESS:
	case CFG_REG_VALUE:
		switch(*cmd) {
		case CMD_WRITE_DATA:
		case CMD_WRITE_CLR_BIT:
		case CMD_CHECK_BITS_SET:
		case CMD_CHECK_BITS_CLR:

			value = get_cfg_value(token, name, lineno);
			set_dcd_param_v2(dcd_v2, *dcd_len, *cmd);
			set_dcd_val_v2(name, lineno, fld, value,
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

#define OPEN_FILE_GET_SZ(name, fp, size, rw)    \
	do {        \
		uint32_t local_sz = 0;    \
		if(NULL != name){    \
			fp = fopen(name, rw);    \
			if(NULL == fp) { printf("Failed to open %s\n", name); return -2;}    \
			fseek(fp, 0, SEEK_END);    local_sz = ftell(fp);    fseek(fp, 0, SEEK_SET);    \
			size = local_sz;    \
		}    \
	} while(0)

static uint32_t parse_cfg_file(dcd_v2_t *dcd_v2, char *name)
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

			parse_cfg_fld(dcd_v2, &cmd, token, name,
					lineno, fld, &dcd_len);
		}

	}

	set_dcd_rst_v2(dcd_v2, dcd_len, name, lineno);
	fclose(fd);

	return dcd_len;
}

int main(int argc, char **argv)
{
	int i = 0, j = 0;
	char *out_buf = NULL;
	uint32_t out_buf_sz, pos;

	flash_header_v3_t *fhdr_v3 = NULL;
	boot_data_v3_t *boot_data = NULL;
	uint32_t hdr_base = 0, dcd_len;
	size_t file_sz;

	memset((char*)&imx_header, 0, sizeof(imx_header_v3_t));
	memset((char*)&container_opts, 0, sizeof(container_opts));

	if (0 != get_options(argc, argv)){
		return -1;
	}

	container_opts[0].hdr_base = INITIAL_LOAD_ADDR_SCU_ROM + misc_opts.ivt_off;
	container_opts[1].hdr_base = INITIAL_LOAD_ADDR_AP_ROM + misc_opts.ivt_off;

	/* The sub imgs needs to align with sector size */
	out_buf_sz = ALIGN(sizeof(imx_header_v3_t) + misc_opts.ivt_off, misc_opts.sector_size);
	pos = out_buf_sz;

	if ((container_opts[0].img[0].core == CORE_SECO) ||
		(container_opts[0].img[0].core == CORE_HDMI_TX_uCPU) ||
		(container_opts[0].img[0].core == CORE_HDMI_RX_uCPU)) {
		out_buf_sz = 0x200;
		pos = 0x200;
		misc_opts.ivt_off = 0;
		misc_opts.sector_size = 0x4;
	}

	mkimg_debug("out buf size: 0x%x, sector_size 0x%x\n", out_buf_sz, misc_opts.sector_size);

	for (i = 0; i < g_iContainer; i++){
		OPEN_FILE_GET_SZ(container_opts[i].scd_file, container_opts[i].fp_scd, container_opts[i].scd_sz, "rb");
		if (container_opts[i].scd_sz) {
			out_buf_sz += ALIGN(container_opts[i].scd_sz, misc_opts.sector_size);

			mkimg_debug("scd size: 0x%x\n", container_opts[i].scd_sz);
			mkimg_debug("out buf size involved scd: 0x%x\n", out_buf_sz);
		}

		/* Will append the CSF at the end, to avoid CSF size aligned with sector size */
		OPEN_FILE_GET_SZ(container_opts[i].csf_file, container_opts[i].fp_csf, container_opts[i].csf_sz, "rb");
		if (container_opts[i].csf_sz) {

			out_buf_sz += CSF_DATA_SIZE;
			
			mkimg_debug("csf size: 0x%x\n", CSF_DATA_SIZE);
			mkimg_debug("out buf size involved csf: 0x%x\n", out_buf_sz);
		}
		
		for(j = 0; j < container_opts[i].img_num; j++){
			OPEN_FILE_GET_SZ(container_opts[i].img[j].img_file, container_opts[i].img[j].fp_img, container_opts[i].img[j].img_sz, "rb");
			out_buf_sz += ALIGN(container_opts[i].img[j].img_sz, misc_opts.sector_size);
			
			mkimg_debug("container%d img%d size: 0x%x\n", i, j, container_opts[i].img[j].img_sz);
			mkimg_debug("out buf size involved container%d boot img%d: 0x%x\n", i, j, out_buf_sz);

			if (container_opts[i].img[j].core == CORE_SC) {
				uint64_t tmp_to = ALIGN((container_opts[i].img[j].to + container_opts[i].img[j].img_sz), 4);

				/* When scd_to is 0, put the SCD load address after SCU image */
				if (container_opts[i].scd_sz && !container_opts[i].scd_to) {
					container_opts[i].scd_to = tmp_to;
					tmp_to = ALIGN((container_opts[i].scd_to + container_opts[i].scd_sz), IMG_AUTO_ALIGN);
				}

				/* When csf_to is 0, put the CSF load address after SCU image */
				if (container_opts[i].csf_sz && !container_opts[i].csf_to) {
					container_opts[i].csf_to = tmp_to;
					tmp_to = ALIGN((container_opts[i].csf_to + container_opts[i].csf_sz), IMG_AUTO_ALIGN);
				}
			}
		}
	}

#ifdef DEBUG
	dump_img_container();
#endif

	out_buf = (char*)calloc(1, out_buf_sz);
	if (NULL == out_buf) {
		fprintf(stderr, "Failed to calloc memory.\n ");
		exit(EXIT_FAILURE);
	}

	mkimg_debug("out buf addr: 0x%x\n", (uint32_t)out_buf);

	for (i = 0; i < g_iContainer; i++) {
		mkimg_debug("container_opts[%d].hdr_base=0x%x\n", i, container_opts[i].hdr_base);

		hdr_base = container_opts[i].hdr_base;
		fhdr_v3 = &imx_header.fhdr[i];
		boot_data =  &imx_header.boot_data[i];

		fhdr_v3->header.tag = IVT_HEADER_TAG; /* 0xD1 */
		fhdr_v3->header.length = cpu_to_be16(sizeof(flash_header_v3_t));
		fhdr_v3->header.version = IVT_VERSION; /* 0x43 */
		fhdr_v3->reserved1 = fhdr_v3->reserved2 = fhdr_v3->reserved3 = 0;
		fhdr_v3->self = (hdr_base + (i * sizeof(flash_header_v3_t)));

		/* Only  container 0 supports DCD */
		if (i == 0 && container_opts[i].dcd_file){
			dcd_len = parse_cfg_file(&imx_header.dcd_table, container_opts[i].dcd_file);
			if (dcd_len)
				fhdr_v3->dcd_ptr = hdr_base + offsetof(imx_header_v3_t, dcd_table);
			else
				fhdr_v3->dcd_ptr = 0;
			mkimg_debug("dcd len = %d\n", dcd_len);
		} else {
			fhdr_v3->dcd_ptr = 0;
	        mkimg_debug("No dcd\n");
		}

		fhdr_v3->boot_data_ptr = hdr_base + offsetof(imx_header_v3_t, boot_data) + sizeof(boot_data_v3_t)*i;

		for (j = 0; j < container_opts[i].img_num; j++){
			boot_data->num_images = container_opts[i].img_num;
			boot_data->bd_size = sizeof(boot_data_v3_t);

			/* The src is a offset from image_offset, not the absolut offset from 0 */
			boot_data->img[j].src = ALIGN(pos, misc_opts.sector_size);
			boot_data->img[j].dst = container_opts[i].img[j].to;
			boot_data->img[j].entry = container_opts[i].img[j].entry;
			boot_data->img[j].size = container_opts[i].img[j].img_sz;
			boot_data->img[j].flags = 0;
			boot_data->img[j].flags |= (container_opts[i].img[j].mu << BOOT_IMG_FLAGS_MU_RID_SHIFT) & BOOT_IMG_FLAGS_MU_RID_MASK;
			boot_data->img[j].flags |= (container_opts[i].img[j].part << BOOT_IMG_FLAGS_PARTITION_ID_SHIFT) & BOOT_IMG_FLAGS_PARTITION_ID_MASK;
			boot_data->img[j].flags |= container_opts[i].img[j].core & BOOT_IMG_FLAGS_CORE_MASK;
			boot_data->img[j].flags |= (container_opts[i].img[j].cpu_id << BOOT_IMG_FLAGS_CPU_RID_SHIFT) & BOOT_IMG_FLAGS_CPU_RID_MASK;

			file_sz = fread(out_buf + pos, 1, container_opts[i].img[j].img_sz, container_opts[i].img[j].fp_img);
			fclose(container_opts[i].img[j].fp_img);

			if (file_sz != container_opts[i].img[j].img_sz) {
				fprintf(stderr, "File %s reading error\n", container_opts[i].img[j].img_file);
				exit(EXIT_FAILURE);
			}

			mkimg_debug("container%d img%d: off:0x%x, dst: 0x%x, entry: 0x%x, size: 0x%x, pos: 0x%x\n",
				i, j,
				(uint32_t)(boot_data->img[j].src),
				(uint32_t)(boot_data->img[j].dst),
				(uint32_t)(boot_data->img[j].entry),
				boot_data->img[j].size,
				pos);

			mkimg_debug("container%d img%d buf addr : 0x%x\n", i, j, (unsigned int)(out_buf + pos));
			
			pos += ALIGN(container_opts[i].img[j].img_sz, misc_opts.sector_size);
		}

	    if (container_opts[i].scd_sz){
			fhdr_v3->scd = container_opts[i].scd_to;
			boot_data->scd.src = ALIGN(pos, misc_opts.sector_size);
			boot_data->scd.dst = container_opts[i].scd_to;
			boot_data->scd.size = container_opts[i].scd_sz;
			
			file_sz = fread(out_buf + pos, 1, container_opts[i].scd_sz, container_opts[i].fp_scd);
			fclose(container_opts[i].fp_scd);

			if (file_sz != container_opts[i].scd_sz) {
				fprintf(stderr, "File %s reading error\n", container_opts[i].scd_file);
				exit(EXIT_FAILURE);
			}
			
			mkimg_debug("scd off: 0x%x, dst: 0x%x, size: 0x%x, pos: 0x%x\n",
				(uint32_t)(boot_data->scd.src),
				(uint32_t)(boot_data->scd.dst),
				boot_data->scd.size,
				pos);
			
			pos += ALIGN(container_opts[i].scd_sz, misc_opts.sector_size);
	    } else {
			fhdr_v3->scd = 0;
			mkimg_debug("No scd\n");
	    }

	    if (container_opts[i].csf_sz){
			fhdr_v3->csf = container_opts[i].csf_to;
			boot_data->csf.src = ALIGN(pos, misc_opts.sector_size);
			boot_data->csf.dst = container_opts[i].csf_to;
			boot_data->csf.size = CSF_DATA_SIZE;
			
			file_sz = fread(out_buf + pos, 1, container_opts[i].csf_sz, container_opts[i].fp_csf);
			fclose(container_opts[i].fp_csf);

			if (file_sz != container_opts[i].csf_sz) {
				fprintf(stderr, "File %s reading error\n", container_opts[i].csf_file);
				exit(EXIT_FAILURE);
			}

			mkimg_debug("csf off: 0x%x, dst: 0x%x, size: 0x%x, pos: 0x%x\n",
				(uint32_t)(boot_data->csf.src),
				(uint32_t)(boot_data->csf.dst),
				boot_data->csf.size,
				pos);
			pos += CSF_DATA_SIZE;
	    } else {
			fhdr_v3->csf = 0;
			mkimg_debug("No csf\n");
	    }
	}

	dump_imx_header_v3(&imx_header);

	if ((container_opts[0].img[0].core == CORE_SECO) ||
	    (container_opts[0].img[0].core == CORE_HDMI_TX_uCPU) ||
	    (container_opts[0].img[0].core == CORE_HDMI_RX_uCPU))
		memcpy(out_buf + misc_opts.ivt_off, (char *)&imx_header, 0x200);
	else
		memcpy(out_buf + misc_opts.ivt_off, (char *)&imx_header, sizeof(imx_header));

	FILE *fp_out = fopen("flash.bin", "wb");
	fwrite(out_buf, out_buf_sz, 1, fp_out);
	fclose(fp_out);

    return 0;
}
