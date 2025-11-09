/*
 * cobs.c
 *
 *  Created on: May 19, 2025
 *      Author: integer
 */


#include <stdint.h>

uint32_t cobs_encode(const uint8_t *input, uint32_t length, uint8_t *output) {
    const uint8_t *end = input + length;
    uint8_t *code_ptr = output++;
    uint8_t code = 1;

    while (input < end) {
        if (*input == 0) {
            *code_ptr = code;
            code_ptr = output++;
            code = 1;
        } else {
            *output++ = *input++;
            code++;
            if (code == 0xFF) {
                *code_ptr = code;
                code_ptr = output++;
                code = 1;
            }
        }
    }

    *code_ptr = code;
    return output - (uint8_t *)code_ptr + (code_ptr - output + length);
}

// COBS decode
uint32_t cobs_decode(const uint8_t *input, uint8_t *output, uint32_t length) {
	uint32_t read_index = 0, write_index = 0;

    while (read_index < length) {
        uint8_t code = input[read_index];
        if (code == 0 || read_index + code > length) return -1; // invalid

        read_index++;
        for (int i = 1; i < code; i++) {
            output[write_index++] = input[read_index++];
        }

        if (code < 0xFF && read_index < length) {
            output[write_index++] = 0;
        }
    }

    return write_index;
}
