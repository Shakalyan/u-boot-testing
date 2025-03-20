#ifndef MBR_H
#define MBR_H

#include <stdint.h>

#define MBR_SIGNATURE ((0xAA55))

struct mbr_partition_record {
    uint8_t  boot_indicator;
    uint8_t  starting_chs[3];
    uint8_t  os_type;
    uint8_t  ending_chs[3];
    uint32_t starting_lba;
    uint32_t size_in_lba;
} __attribute__((packed));
typedef struct mbr_partition_record mbr_partition_record_t;
typedef mbr_partition_record_t* ptr_mbr_partition_record_t;

struct mbr {
    uint8_t  boot_code[440];
    uint32_t disk_signature;
    uint16_t unknown;
    mbr_partition_record_t partition_records[4];
    uint16_t signature;
} __attribute__((packed));
typedef struct mbr mbr_t;
typedef mbr_t* ptr_mbr_t;


int generate_protective_mbr(void *mbr_buffer, uint64_t disk_size, uint64_t sector_size);


#endif // MBR_H