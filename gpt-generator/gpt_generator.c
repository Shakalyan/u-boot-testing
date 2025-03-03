#include <stdio.h>

#include "gpt.h"
#include "crc.h"

int main(void)
{

    uint8_t data[] = "123456789";
    printf("ale\n");
    uint32_t crc = crc32(data, sizeof(data)-1);
    printf("crc = 0x%x\n", crc);

    crc = 0xFFFFFFFF - crc;
    uint8_t data2[14] = "1234567890000";
    for (int i = 0; i < 4; ++i) {
        data2[12-i] = crc & 0xff;
        crc >>= 8;
    }

    printf("crc check = 0x%x\n", crc32(data2, sizeof(data2)-1));

    return 0;
}