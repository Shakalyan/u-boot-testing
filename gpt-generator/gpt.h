#ifndef GPT_H
#define GPT_H

#include <stdint.h>

#define SECTOR_SIZE ((512))
#define ENTRY_SIZE  ((0x80))

#define GPT_HEADER_SIGNATURE ((5452415020494645))
#define GPT_HEADER_REVISION  ((0x10000))
#define GPT_HEADER_LBA       ((1))

#define GUID_SIZE ((16))

#define PARTITION_NAME_SIZE ((72))

#define MINIMAL_GPT_ENTRY_ARRAY_SIZE ((16384))

// C12A7328-F81F-11d2-BA4B-00A0C93EC93B
extern const uint8_t EFI_SYSTEM_PARTITION_GUID[GUID_SIZE];

// 024DEE41-33E7-11d3-9D69-0008C781F39F
extern const uint8_t PROTECTIVE_MBR_PARTITION_GUID[GUID_SIZE];

typedef const uint8_t* guid_t;


struct gpt_header {
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
} __attribute__ ((packed));
typedef struct gpt_header gpt_header_t;
typedef gpt_header_t* ptr_gpt_header_t;

struct gpt_entry {
    uint8_t  partition_type_guid[16];
    uint8_t  unique_partition_guid[16];
    uint64_t starting_lba;
    uint64_t ending_lba;
    uint64_t attributes;
    uint8_t  partition_name[PARTITION_NAME_SIZE];
#if ENTRY_SIZE > 0x80
    uint8_t  reserved[ENTRY_SIZE-0x80];
#endif
} __attribute__ ((packed));
typedef struct gpt_entry gpt_entry_t;
typedef gpt_entry_t* ptr_gpt_entry_t;


void add_gpt_entry(ptr_gpt_entry_t entries, int index,
                   guid_t type_guid, guid_t unique_guid,
                   uint64_t size,
                   uint64_t attributes, const char *name);

gpt_entry_t* generate_gpt_entry(guid_t type_guid, guid_t unique_guid,
                                uint64_t starting_lba, uint64_t ending_lba,
                                uint64_t attributes, const char *name);

void print_guid(guid_t guid);

void print_gpt_entry(ptr_gpt_entry_t entry);

void print_gpt_header(ptr_gpt_header_t header);

gpt_header_t* generate_gpt_header(ptr_gpt_entry_t entries, uint32_t entries_size,
                                  int is_primary, guid_t disk_guid);

#endif