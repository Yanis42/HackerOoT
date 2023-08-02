/*
 * Copyright (c) 2022, 2023 glank
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef HB_H

#include <ultra64/ultratypes.h>

#define hb_regs			(*(volatile hb_regs_t *) 0xA8050000)

#define HB_STATUS_RESET		(1  << 9)
#define HB_STATUS_ERROR		(15 << 5)
#define HB_STATUS_SD_INIT	(1  << 4)
#define HB_STATUS_SD_HC		(1  << 3)
#define HB_STATUS_SD_INSERTED	(1  << 2)
#define HB_STATUS_SD_BUSY	(1  << 1)
#define HB_STATUS_SD_READY	(1  << 0)

#define HB_FIFO_STA_PWR		(1 << 0)
#define HB_FIFO_STA_RXF		(1 << 1)
#define HB_FIFO_STA_TXE		(1 << 2)

#define HB_ERROR_SUCCESS	0
#define HB_ERROR_INVAL		1
#define HB_ERROR_QUEUEFULL	2
#define HB_ERROR_NOMEM		3
#define HB_ERROR_NOBUFFER	4
#define HB_ERROR_OTHER		5

#define HB_TIMEBASE_FREQ	60750000

typedef struct
{
	u32	key;		/* 0x0000 */
	u32	sd_dram_addr;	/* 0x0004 */
	u32	sd_write_lba;	/* 0x0008 */
	u32	sd_read_lba;	/* 0x000C */
	u32	sd_n_blocks;	/* 0x0010 */
	u32	status;		/* 0x0014 */
	u32	dram_save_addr;	/* 0x0018 */
	u32	dram_save_len;	/* 0x001C */
	u32	dram_save_key;	/* 0x0020 */
	u32	timebase_hi;	/* 0x0024 */
	u32	timebase_lo;	/* 0x0028 */
	u32	fifo_status;	/* 0x002C */
	u32	fifo_dram_addr;	/* 0x0030 */
	u32	fifo_rd_len;	/* 0x0034 */
	u32	fifo_wr_len;	/* 0x0038 */
				/* 0x003C */
} hb_regs_t;

#endif
