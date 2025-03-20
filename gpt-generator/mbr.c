#include <string.h>

#include "mbr.h"


int generate_protective_mbr(void *mbr_buffer, uint64_t disk_size, uint64_t sector_size)
{
    memset(mbr_buffer, 0, sizeof(mbr_t));

    ptr_mbr_t mbr = (ptr_mbr_t)mbr_buffer;
    mbr->signature = MBR_SIGNATURE;

    ptr_mbr_partition_record_t pr = &mbr->partition_records[0];
    pr->starting_chs[2] = 0x02;
    pr->os_type = 0xEE;

    pr->ending_chs[0] = 0xFF;
    pr->ending_chs[1] = 0xFF;
    pr->ending_chs[2] = 0xFF;

    pr->starting_lba = 1;
    pr->size_in_lba = 0xFFFFFFFF; //disk_size / sector_size;

    return 0;
}