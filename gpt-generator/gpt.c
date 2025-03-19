#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gpt.h"
#include "crc.h"


const uint8_t EFI_SYSTEM_PARTITION_GUID[GUID_SIZE] =
    { 0xc1, 0x2a, 0x73, 0x28, 0xf8, 0x1f, 0x11, 0xd2, 0xba, 0x4b, 0x0, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b };

const uint8_t PROTECTIVE_MBR_PARTITION_GUID[GUID_SIZE] =
    { 0x2, 0x4d, 0xee, 0x41, 0x33, 0xe7, 0x11, 0xd3, 0x9d, 0x69, 0x0, 0x8, 0xc7, 0x81, 0xf3, 0x9f };


gpt_header_t* generate_gpt_header(ptr_gpt_entry_t entries, uint32_t entries_size,
                                  int is_primary, guid_t disk_guid)
{
    uint32_t entries_num = entries_size / sizeof(gpt_entry_t);
    gpt_header_t *header = calloc(1, sizeof(gpt_header_t));
    ptr_gpt_entry_t first_entry = &entries[0], last_entry = &entries[entries_num-1];
    uint64_t backup_gpt_lba = last_entry->ending_lba + (MINIMAL_GPT_ENTRY_ARRAY_SIZE / SECTOR_SIZE) + 1;

    header->signature = GPT_HEADER_SIGNATURE;
    header->revision = GPT_HEADER_REVISION;
    header->header_size = sizeof(gpt_header_t);
    header->header_crc32 = 0;

    if (is_primary) {
        header->my_lba = GPT_HEADER_LBA;
        header->alternate_lba = backup_gpt_lba;
        header->partition_entry_lba = GPT_HEADER_LBA + 1;
        // primary header + entry array + partitions + entry array + alternate header
    } else {
        header->my_lba = backup_gpt_lba;
        header->alternate_lba = GPT_HEADER_LBA;
        header->partition_entry_lba = header->alternate_lba - 1;
    }

    header->first_usable_lba = first_entry->starting_lba;
    header->last_usable_lba = last_entry->ending_lba;
    memcpy(header->disk_guid, disk_guid, GUID_SIZE);
    header->number_of_partition_entries = entries_num;
    header->size_of_partition_entry = MINIMAL_GPT_ENTRY_ARRAY_SIZE;
    header->partition_entry_array_crc32 = crc32((uint8_t *)entries, entries_num * sizeof(gpt_entry_t));

    header->header_crc32 = crc32((uint8_t *)header, sizeof(gpt_header_t));

    return header;
}

gpt_entry_t* generate_gpt_entry(guid_t type_guid, guid_t unique_guid,
                                uint64_t starting_lba, uint64_t ending_lba,
                                uint64_t attributes, const char *name)
{
    gpt_entry_t *entry = calloc(1, sizeof(gpt_entry_t));

    memcpy(entry->partition_type_guid, type_guid, GUID_SIZE);
    memcpy(entry->unique_partition_guid, unique_guid, GUID_SIZE);
    entry->starting_lba = starting_lba;
    entry->ending_lba = ending_lba;
    entry->attributes = attributes;
    memcpy(entry->partition_name, name, PARTITION_NAME_SIZE);

    return entry;
}

void add_gpt_entry(ptr_gpt_entry_t entries, int index,
                   guid_t type_guid, guid_t unique_guid,
                   uint64_t size,
                   uint64_t attributes, const char *name)
{
    uint64_t starting_lba = 0, ending_lba = 0;
    ptr_gpt_entry_t entry = &entries[index];

    if (index == 0) {
        starting_lba = MINIMAL_GPT_ENTRY_ARRAY_SIZE / SECTOR_SIZE + GPT_HEADER_LBA + 1;
    } else {
        starting_lba = entries[index-1].ending_lba + 1;
    }
    ending_lba = starting_lba + size - 1;

    memcpy(entry->partition_type_guid, type_guid, GUID_SIZE);
    memcpy(entry->unique_partition_guid, unique_guid, GUID_SIZE);
    entry->starting_lba = starting_lba;
    entry->ending_lba = ending_lba;
    entry->attributes = attributes;
    memcpy(entry->partition_name, name, PARTITION_NAME_SIZE);
}

void print_guid(guid_t guid)
{
    int i = 0;
    for (i = 0; i < 4; ++i)
        printf("%.2X", guid[i]);
    for (i = 4; i < 10; i += 2)
        printf("-%.2X%.2X", guid[i], guid[i+1]);
    printf("-");
    for (i = 10; i < 16; ++i)
        printf("%.2X", guid[i]);
}

void print_gpt_entry(ptr_gpt_entry_t entry)
{
    printf("partition type GUID: ");
    print_guid(entry->partition_type_guid);
    printf("\n");
    printf("unique partition GUID: ");
    print_guid(entry->unique_partition_guid);
    printf("\n");

    printf("starting LBA: %lu\n", entry->starting_lba);
    printf("ending LBA: %lu\n", entry->ending_lba);
    printf("attributes: %lu\n", entry->attributes);
    printf("partition name: %s\n", entry->partition_name);
}

void print_gpt_header(ptr_gpt_header_t header)
{
    printf("signature: %lu\n", header->signature);
    printf("revision: %u\n", header->revision);
    printf("header size: %u\n", header->header_size);
    printf("header crc32: %u\n", header->header_crc32);
    printf("lba: %lu\n", header->my_lba);
    printf("alternate lba: %lu\n", header->alternate_lba);
    printf("first usable lba: %lu\n", header->first_usable_lba);
    printf("last usable lba: %lu\n", header->last_usable_lba);

    printf("disk guid: ");
    print_guid(header->disk_guid);
    printf("\n");

    printf("partition entry lba: %lu\n", header->partition_entry_lba);
    printf("number of partition entries: %u\n", header->number_of_partition_entries);
    printf("size of partition entry: %u\n", header->size_of_partition_entry);
    printf("partition entry array crc32: %u\n", header->partition_entry_array_crc32);
}
