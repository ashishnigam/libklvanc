/*
 * Copyright (c) 2016 Kernel Labs Inc. All Rights Reserved
 *
 * Address: Kernel Labs Inc., PO Box 745, St James, NY. 11780
 * Contact: sales@kernellabs.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <libklvanc/vanc.h>

/* Normally we don't use a global, but we know our test harness will never be
   multi-threaded, and this is a really easy way to get the results out of the
   callback for comparison */
static uint16_t vancResult[16384];
static size_t vancResultCount;
static int passCount = 0;
static int failCount = 0;

#define SHOW_DETAIL 1

/* CALLBACKS for message notification */
static int cb_SCTE_104(void *callback_context, struct vanc_context_s *ctx, struct packet_scte_104_s *pkt)
{
	int ret = -1;

#ifdef SHOW_DETAIL
	/* Have the library display some debug */
	printf("Asking libklvanc to dump a struct\n");
	ret = dump_SCTE_104(ctx, pkt);
	if (ret != 0) {
		fprintf(stderr, "Error dumping SCTE 104 packet!\n");
		return -1;
	}
#endif

	uint16_t *words;
	uint16_t wordCount;
	ret = convert_SCTE_104_to_words(pkt, &words, &wordCount);
	if (ret != 0) {
		fprintf(stderr, "Failed to convert 104 to words: %d\n", ret);
		return -1;
	}

	memcpy(vancResult, words, wordCount * sizeof(uint16_t));
	vancResultCount = wordCount;
	free(words);

	return 0;
}

static struct vanc_callbacks_s callbacks = 
{
	.scte_104		= cb_SCTE_104,
};
/* END - CALLBACKS for message notification */

static unsigned char __0_vancentry[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x01, 0x52,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x01, 0x51, 0x02, 0x00,
	0x02, 0x00, 0x01, 0x52, 0x02, 0x00, 0x02, 0x05, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x06, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x01, 0x02,
	0x01, 0x40, 0x02, 0x00, 0x02, 0x00, 0x01, 0x52, 0x02, 0x00, 0x01, 0x64,
	0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x90, 0x02, 0x03, 0x01, 0x01,
	0x02, 0x00, 0x01, 0x01, 0x01, 0x04, 0x02, 0x00, 0x01, 0x02, 0x02, 0x00,
	0x02, 0x00, 0x01, 0x01, 0x02, 0x09, 0x02, 0x00, 0x02, 0x03, 0x02, 0x00,
	0x01, 0x01, 0x02, 0x30, 0x01, 0x01, 0x01, 0x0b, 0x02, 0x00, 0x02, 0x12,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0x40, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x02, 0x03,
	0x01, 0x01, 0x01, 0x02, 0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x08,
	0x02, 0x00, 0x01, 0x08, 0x01, 0x01, 0x01, 0x0b, 0x02, 0x05, 0x01, 0x54,
	0x02, 0x56, 0x02, 0x4e, 0x01, 0x54, 0x02, 0x00, 0x02, 0x06
};

static unsigned char test1[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x01, 0x25,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x01, 0x02,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x12, 0x01, 0x34, 0x01, 0x45, 0x01, 0x67,
	0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x01, 0x01, 0x02,
	0x01, 0x01, 0x01, 0x01, 0x02, 0x0f, 0x02, 0x00, 0x01, 0x02, 0x02, 0x12,
	0x01, 0x34, 0x01, 0x02
};

static unsigned char test2[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x02, 0x28,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x01, 0x02,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x12, 0x01, 0x34, 0x01, 0x45, 0x01, 0x67,
	0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x01, 0x01, 0x02,
	0x01, 0x01, 0x01, 0x01, 0x02, 0x09, 0x02, 0x00, 0x02, 0x05, 0x01, 0xf4,
	0x02, 0x03, 0x02, 0x30, 0x01, 0x34, 0x01, 0x32, 0x02, 0x49
};

static unsigned char test3[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x02, 0x28,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x01, 0x02,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x12, 0x01, 0x34, 0x01, 0x45, 0x01, 0x67,
	0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x01, 0x01, 0x02,
	0x01, 0x01, 0x01, 0x01, 0x02, 0x0a, 0x02, 0x00, 0x02, 0x05, 0x01, 0x01,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x03, 0x02, 0xe7, 0x01, 0xa8
};

static unsigned char test4[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x01, 0x38,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x01, 0x02,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x12, 0x01, 0x34, 0x01, 0x45, 0x01, 0x67,
	0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x01, 0x01, 0x02,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x0b, 0x02, 0x00, 0x01, 0x15, 0x02, 0x00,
	0x02, 0x00, 0x01, 0x04, 0x02, 0xd2, 0x02, 0x00, 0x01, 0x01, 0x01, 0x5e,
	0x01, 0x01, 0x02, 0x03, 0x01, 0x01, 0x01, 0x02, 0x02, 0x03, 0x02, 0x03,
	0x01, 0x01, 0x01, 0x02, 0x01, 0x3b, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00,
	0x01, 0x01, 0x02, 0x00, 0x02, 0x61
};

static unsigned char test5[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x01, 0x2f,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x01, 0x02,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x12, 0x01, 0x34, 0x01, 0x45, 0x01, 0x67,
	0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x01, 0x01, 0x02,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x10, 0x02, 0x00, 0x02, 0x0c, 0x02, 0xee,
	0x01, 0xc4, 0x01, 0x58, 0x02, 0xdb, 0x02, 0xee, 0x01, 0xc4, 0x02, 0x1d,
	0x01, 0xcd, 0x02, 0x65, 0x02, 0x00, 0x01, 0x01, 0x01, 0xf4, 0x02, 0xac
};

static unsigned char test6[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x02, 0x21,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 0x02, 0x00, 0x02, 0x00, 0x01, 0x01,
	0x01, 0x10, 0x02, 0x00, 0x02, 0x0c, 0x02, 0xee, 0x01, 0xc4, 0x01, 0x58,
	0x02, 0xdb, 0x02, 0xee, 0x01, 0xc4, 0x02, 0x1d, 0x01, 0xcd, 0x02, 0x65,
	0x02, 0x00, 0x01, 0x01, 0x01, 0xf4, 0x02, 0x6c
};

static unsigned char test7[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x02, 0x1d,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 0x02, 0x00, 0x02, 0x00, 0x01, 0x01,
	0x01, 0x08, 0x02, 0x00, 0x01, 0x08, 0x01, 0x01, 0x01, 0x0b, 0x02, 0x05,
	0x01, 0x54, 0x02, 0x56, 0x01, 0x43, 0x01, 0x54, 0x02, 0x11, 0x02, 0xe4
};

static unsigned char test8[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x02, 0x21,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 0x02, 0x00, 0x02, 0x00, 0x01, 0x01,
	0x02, 0x0c, 0x02, 0x00, 0x02, 0x0c, 0x02, 0x55, 0x02, 0x22, 0x02, 0xaa,
	0x02, 0x11, 0x02, 0x22, 0x01, 0x0b, 0x02, 0x05, 0x01, 0x54, 0x02, 0x56,
	0x01, 0x43, 0x01, 0x54, 0x02, 0x11, 0x01, 0x43
};

static unsigned char test9[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x01, 0x25,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x01, 0x04,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x12, 0x01, 0x34, 0x01, 0x45, 0x01, 0x67,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x02,
	0x02, 0x00, 0x01, 0x01, 0x02, 0x0f, 0x02, 0x00, 0x01, 0x02, 0x02, 0x12,
	0x01, 0x34, 0x01, 0xd6
};

static unsigned char test10[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x01, 0x25,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x01, 0x01,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x12, 0x01, 0x34, 0x01, 0x45, 0x01, 0x67,
	0x01, 0x13, 0x02, 0x88, 0x01, 0x01, 0x02, 0x18, 0x01, 0x01, 0x01, 0x02,
	0x01, 0x01, 0x01, 0x01, 0x02, 0x0f, 0x02, 0x00, 0x01, 0x02, 0x02, 0x12,
	0x01, 0x34, 0x01, 0x88
};

static unsigned char test11[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x01, 0x25,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x02, 0x03,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x12, 0x01, 0x34, 0x01, 0x45, 0x01, 0x67,
	0x01, 0x13, 0x02, 0x88, 0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x02,
	0x02, 0x00, 0x01, 0x01, 0x02, 0x0f, 0x02, 0x00, 0x01, 0x02, 0x02, 0x12,
	0x01, 0x34, 0x02, 0x70
};

static unsigned char test12[] = {
	0x00, 0x00, 0x03, 0xff, 0x03, 0xff, 0x02, 0x41, 0x01, 0x07, 0x01, 0x25,
	0x01, 0x08, 0x02, 0xff, 0x02, 0xff, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
	0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x0e, 0x02, 0x05,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x12, 0x01, 0x34, 0x01, 0x45, 0x01, 0x67,
	0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x02,
	0x02, 0x00, 0x01, 0x01, 0x02, 0x0f, 0x02, 0x00, 0x01, 0x02, 0x02, 0x12,
	0x01, 0x34, 0x02, 0xd7
};

static int test_scte_104(struct vanc_context_s *ctx, const uint8_t *buf, size_t bufSize)
{
	int numWords = bufSize / 2;
	int mismatch = 0;

	printf("\nParsing a new SCTE104 VANC packet......\n");
	uint16_t *arr = malloc(bufSize);
	if (arr == NULL)
		return -1;

	for (int i = 0; i < numWords; i++) {
		arr[i] = buf[i * 2] << 8 | buf[i * 2 + 1];
	}

	printf("Original Input\n");
	for (int i = 0; i < numWords; i++) {
		printf("%04x ", arr[i]);
	}
	printf("\n");

	int ret = vanc_packet_parse(ctx, 13, arr, numWords);

	printf("Final output\n");
	for (int i = 0; i < vancResultCount; i++) {
		printf("%04x ", vancResult[i]);
	}
	printf("\n");

	for (int i = 0; i < vancResultCount; i++) {
		if (arr[i] != vancResult[i]) {
			fprintf(stderr, "Mismatch starting at offset 0x%02x\n", i);
			mismatch = 1;
			break;
		}
	}
	
	free(arr);

	if (mismatch) {
		printf("Printing mismatched structure:\n");
		failCount++;
		ret = vanc_packet_parse(ctx, 13, vancResult, vancResultCount);
	} else {
		printf("Original and generated versions match!\n");
		passCount++;
	}

	return ret;
}

int scte104_main(int argc, char *argv[])
{
	struct vanc_context_s *ctx;
	int ret;

	if (vanc_context_create(&ctx) < 0) {
		fprintf(stderr, "Error initializing library context\n");
		exit(1);
	}
#ifdef SHOW_DETAIL
	ctx->verbose = 1;
#endif
	ctx->callbacks = &callbacks;
	printf("Library initialized.\n");

	ret = test_scte_104(ctx, __0_vancentry, sizeof(__0_vancentry));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test1, sizeof(test1));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test2, sizeof(test2));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test3, sizeof(test3));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test4, sizeof(test4));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test5, sizeof(test5));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test6, sizeof(test6));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test7, sizeof(test7));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test8, sizeof(test8));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test9, sizeof(test9));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test10, sizeof(test10));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test11, sizeof(test11));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	ret = test_scte_104(ctx, test12, sizeof(test12));
	if (ret < 0)
		fprintf(stderr, "SCTE-104 failed to parse\n");

	vanc_context_destroy(ctx);
	printf("Library destroyed.\n");

	printf("Final result: PASS: %d/%d, Failures: %d\n",
	       passCount, passCount + failCount, failCount);
	return 0;
}
