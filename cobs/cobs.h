/*
 * cobs.h
 *
 *  Created on: May 19, 2025
 *      Author: integer
 */

#ifndef COBS_H_
#define COBS_H_

#include <stdint.h>

uint32_t cobs_encode(const uint8_t *input, uint32_t length, uint8_t *output);
uint32_t cobs_decode(const uint8_t *input, uint8_t *output, int length);

#endif /* COBS_H_ */
