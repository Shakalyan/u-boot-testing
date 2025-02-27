#ifndef GPT_H
#define GPT_H

#include <stdint.h>

#define SECTOR_SIZE ((512))
#define ENTRY_SIZE  ((0x80))

typedef struct GPT_Header {
    uint64_t signature;
    uint32_t revision;
    uint32_t header_size;
    uint32_t header_crc32;
    uint32_t reserved1;
    uint64_t my_lba;
    uint64_t alternate_lba;
    uint64_t first_usable_lba;
    uint64_t last_usable_lba;
    uint8_t  disk_guid[16];
    uint64_t partition_entry_lba;
    uint32_t number_of_partition_entries;
    uint32_t size_of_partition_entry;
    uint32_t partition_entry_array_crc32;
    uint8_t  reserved2[SECTOR_SIZE-0x5c];
} GPT_Header;

typedef struct GPT_Entry {
    uint8_t  partition_type_guid[16];
    uint8_t  unique_partition_guid[16];
    uint64_t starting_lba;
    uint64_t ending_lba;
    uint64_t attributes;
    uint8_t  partition_name[72];
#if ENTRY_SIZE > 0x80
    uint8_t  reserved[ENTRY_SIZE-0x80];
#endif
} GPT_Entry;

#endif