#ifndef CRC_H
#define CRC_H

#include <stdint.h>

#define CRC32_POLY ((0x04C11DB7))
#define CRC32_XOR_OUT ((0xFFFFFFFF))


uint32_t crc32(uint8_t *data, uint64_t size);


#endif
