#include "crc.h"


static uint32_t* crc32_get_lookup_table()
{
    static uint32_t table[256];
    static char table_is_inited = 0;

    if (table_is_inited)
        return table;

    for (uint32_t byte = 0; byte < 256; ++byte) {
        uint32_t crc = byte<<24;
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x80000000)
                crc = (crc<<1) ^ CRC32_POLY;
            else
                crc <<= 1;
        }
        table[byte] = crc;
    }
    table_is_inited = 1;

    return table;
}

uint32_t crc32(uint8_t *data, uint64_t size)
{
    uint32_t crc = 0;
    uint32_t *table = crc32_get_lookup_table();

    for (uint64_t i = 0; i < size; ++i) {
        crc = (crc<<8) ^ table[(crc ^ data[i]<<24)>>24];
    }

    return crc ^ CRC32_XOR_OUT;
}
