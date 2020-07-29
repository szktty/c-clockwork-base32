#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "base32.h"

static char encode_symbols[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K',
	'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X',
	'Y', 'Z',
};

static int8_t decode_symbols[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 0-9 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 10-19 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 20-29 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 30-39 */
	-1, -1, -1, -1, -1, -1, -1, -1, 0, 1, /* 40-49 */
	2, 3, 4, 5, 6, 7, 8, 9, 0, -1, /* 50-59 */
	-1, -1, -1, -1, -1, 10, 11, 12, 13, 14, /* 60-69 */
	15, 16, 17, 1, 18, 19, 1, 20, 21, 0, /* 70-79 */
	22, 23, 24, 25, 26, -2, 27, 28, 29, 30, /* 80-89 */
	31, -1, -1, -1, -1, -1, -1, 10, 11, 12, /* 90-99 */
	13, 14, 15, 16, 17, 1, 18, 19, 1, 20, /* 100-109 */
	21, 0, 22, 23, 24, 25, 26, -1, 27, 28, /* 110-119 */
	29, 30, 31, -1, -1, -1, -1, -1, -1, -1, /* 120-129 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 130-109 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 140-109 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 150-109 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 160-109 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 170-109 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 180-109 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 190-109 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 200-209 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 210-209 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 220-209 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 230-209 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 240-209 */
	-1, -1, -1, -1, -1, -1, /* 250-256 */
};

static int get_buf_len(int base, int unit) {
    int buf_len = (base / unit) * unit;
    if (base % unit > 0) {
        buf_len = buf_len + unit;
    }
    return buf_len;
}

int base32_encode(const uint8_t *input, const int input_len,
                  uint8_t *output, const int output_len) {
    int input_buf_len = get_buf_len(input_len, 5);
    uint8_t *input_buf = malloc(sizeof(uint8_t) * input_buf_len);
    memset(input_buf, 0, input_buf_len);
    memcpy(input_buf, input, input_len);

    int output_buf_len = input_buf_len * 5 / 8;
    uint8_t *output_buf = malloc(sizeof(uint8_t) * output_buf_len);
    memset(output_buf, 0, output_buf_len);

    for (int i = 0; i <= (input_buf_len / 5); i++) {
        int j = i * 8;
        int k = i * 5;
        output_buf[j] = encode_symbols[input_buf[k] >> 3 & 0x1F];
        output_buf[j+1] = encode_symbols[((input_buf[k] & 0x07) << 2) |
            (input_buf[k+1] >> 6 & 0x03)];
        output_buf[j+2] = encode_symbols[input_buf[k+1] >> 1 & 0x1F];
        output_buf[j+3] = encode_symbols[((input_buf[k+1] & 0x01) << 4) |
            (input_buf[k+2] >> 4 & 0x0F)];
        output_buf[j+4] = encode_symbols[((input_buf[k+2] & 0x0F) << 1) |
            (input_buf[k+3] >> 7 & 0x01)];
        output_buf[j+5] = encode_symbols[input_buf[k+3] >> 2 & 0x1F];
        output_buf[j+6] = encode_symbols[((input_buf[k+3] & 0x03) << 3) |
            (input_buf[k+4] >> 5 & 0x07)];
        output_buf[j+7] = encode_symbols[input_buf[k+4] & 0x1F];
    }

    int write_len = input_len * 8 / 5;
    if (input_len * 8 % 5 > 0) {
        write_len++;
    }
    if (write_len > output_len) {
        write_len = output_len;
    }

    memcpy(output, output_buf, write_len);
    free(input_buf);
    free(output_buf);
    return write_len;
}

int base32_decode(const uint8_t *input, const int input_len,
                  uint8_t *output, const int output_len) {
    int input_buf_len = get_buf_len(input_len, 8);
    uint8_t *input_buf = malloc(sizeof(uint8_t) * input_buf_len);
    memset(input_buf, '0', input_buf_len);
    memcpy(input_buf, input, input_len);

    int output_buf_len = input_buf_len * 8 / 5;
    uint8_t *output_buf = malloc(sizeof(uint8_t) * output_buf_len);
    memset(output_buf, 0, output_buf_len);

    for (int i = 0; i < (input_buf_len / 8); i++) {
        int j = i * 5;
        int k = i * 8;
        uint8_t d0 = decode_symbols[input_buf[k]];
        uint8_t d1 = decode_symbols[input_buf[k+1]];
        uint8_t d2 = decode_symbols[input_buf[k+2]];
        uint8_t d3 = decode_symbols[input_buf[k+3]];
        uint8_t d4 = decode_symbols[input_buf[k+4]];
        uint8_t d5 = decode_symbols[input_buf[k+5]];
        uint8_t d6 = decode_symbols[input_buf[k+6]];
        uint8_t d7 = decode_symbols[input_buf[k+7]];
        if (d0 < 0 || d1 < 0 || d2 < 0 || d3 < 0 || d4 < 0 || d5 < 0 || d6 < 0 || d7 < 0) {
            printf("error: invalid symbol\n");
            goto error;
        }
        output_buf[j] = d0 << 3 | (d1 >> 2 & 0x07);
        output_buf[j+1] = (d1 & 0x03) << 6 | (d2 << 1) | (d3 >> 4 & 0x01);
        output_buf[j+2] = (d3 & 0x0F) << 4 | (d4 >> 1 & 0x0F);
        output_buf[j+3] = ((d4 & 0x01) << 7) | (d5 << 2) | (d6 >> 3 & 0x03);
        output_buf[j+4] = ((d6 & 0x07) << 5) | d7;
    }

    int write_len = input_len * 5 / 8;
    if (write_len > output_len) {
        write_len = output_len;
    }

    memcpy(output, output_buf, write_len);
    free(input_buf);
    return write_len;

error:
    free(input_buf);
    free(output_buf);
    return -1;
}

