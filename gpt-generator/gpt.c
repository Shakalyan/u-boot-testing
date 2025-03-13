#include <stdlib.h>
#include <string.h>

#include "gpt.h"


GPT_Header* generate_gpt_header()
{
    GPT_Header *header = malloc(sizeof(GPT_Header));
    memset(header, 0, sizeof(GPT_Header));

    header->signature = GPT_HEADER_SIGNATURE;
    header->revision = GPT_HEADER_REVISION;
    header->header_size = sizeof(GPT_Header);
    header->header_crc32 = 0;
    header->my_lba = GPT_HEADER_LBA;
    header->alternate_lba = 0; // ?????????????????????????/

    return header;
}

GPT_Entry* generate_gpt_entry()
{
    GPT_Entry *entry = malloc(sizeof(GPT_Entry));
    memset(entry, 0, sizeof(GPT_Entry));

    memcpy(entry->partition_type_guid, EFI_SYSTEM_PARTITION_GUID, GUID_SIZE);

    return entry;
}
