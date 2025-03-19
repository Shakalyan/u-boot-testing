#include <stdio.h>
#include <stdlib.h>

#include "gpt.h"
#include "crc.h"

//17c9e43f-5916-49fd-8edb-6afff08164af
const uint8_t PARTITION_1_GUID[GUID_SIZE] =
    { 0x17, 0xc9, 0xe4, 0x3f, 0x59, 0x16, 0x49, 0xfd, 0x8e, 0xdb, 0x6a, 0xff, 0xf0, 0x81, 0x64, 0xaf };

// 960d0f9c-2642-4b22-8ec4-edbaadc86be7
const uint8_t PARTITION_2_GUID[GUID_SIZE] =
    { 0x96, 0x0d, 0x0f, 0x9c, 0x26, 0x42, 0x4b, 0x22, 0x8e, 0xc4, 0xed, 0xba, 0xad, 0xc8, 0x6b, 0xe7 };

// d77ba9a1-5eac-4bad-a935-271346610728
const uint8_t DISK_1_GUID[GUID_SIZE] =
    { 0xd7, 0x7b, 0xa9, 0xa1, 0x5e, 0xac, 0x4b, 0xad, 0xa9, 0x35, 0x27, 0x13, 0x46, 0x61, 0x7, 0x28 };

int main(void)
{
    gpt_entry_t gpt_entries[2];
    ptr_gpt_header_t primary_header = NULL, backup_header = NULL;

    add_gpt_entry(gpt_entries, 0, PROTECTIVE_MBR_PARTITION_GUID, PARTITION_1_GUID, 1, 0, "protective MBR");
    add_gpt_entry(gpt_entries, 1, EFI_SYSTEM_PARTITION_GUID, PARTITION_2_GUID, 1, 0, "efi system");
    primary_header = generate_gpt_header(gpt_entries, sizeof(gpt_entries), 1, DISK_1_GUID);
    backup_header = generate_gpt_header(gpt_entries, sizeof(gpt_entries), 0, DISK_1_GUID);

    printf("Primary Header:\n");
    print_gpt_header(primary_header);

    printf("\nPartition 1:\n");
    print_gpt_entry(&gpt_entries[0]);
    printf("\nPartition 2:\n");
    print_gpt_entry(&gpt_entries[1]);

    //print_guid(PARTITION_1_GUID);

Free:
    free(primary_header);
    free(backup_header);

    return 0;
}