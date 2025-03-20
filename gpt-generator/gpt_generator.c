#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "gpt.h"
#include <zlib.h>

//17c9e43f-5916-49fd-8edb-6afff08164af
const uint8_t PARTITION_1_GUID[GUID_SIZE] =
    { 0x17, 0xc9, 0xe4, 0x3f, 0x59, 0x16, 0x49, 0xfd, 0x8e, 0xdb, 0x6a, 0xff, 0xf0, 0x81, 0x64, 0xaf };

// 99c72ec3-6396-4718-97dc-badb8079571a
const uint8_t PARTITION_2_GUID[GUID_SIZE] =
    { 0xc3, 0x2e, 0xc7, 0x99, 0x96, 0x63, 0x18, 0x47, 0x97, 0xdc, 0xba, 0xdb, 0x80, 0x79, 0x57, 0x1a };

const char PARTITION_1_NAME[] = "protective MBR";
const char PARTITION_2_NAME[] = "boot";

// a2d87104-c3d0-c241-97cd-1bb5212affa7
const uint8_t DISK_1_GUID[GUID_SIZE] =
    { 0xa2, 0xd8, 0x71, 0x4, 0xc3, 0xd0, 0xc2, 0x41, 0x97, 0xcd, 0x1b, 0xb5, 0x21, 0x2a, 0xff, 0xa7 };

uint8_t MBR[SECTOR_SIZE];


void generate_image(void)
{
    gpt_entry_t gpt_entries[128];
    char partition_name[PARTITION_NAME_SIZE];
    ptr_gpt_header_t primary_header = NULL, backup_header = NULL;
    uint8_t *zeroes = NULL;
    uint8_t *gpt_image = NULL;
    uint32_t gpt_image_size = 0;

    memset(gpt_entries, 0, sizeof(gpt_entries));

    MBR[SECTOR_SIZE-2] = 0x55;
    MBR[SECTOR_SIZE-1] = 0xAA;
    

    memset(partition_name, 0, PARTITION_NAME_SIZE);
    memcpy(partition_name, PARTITION_2_NAME, sizeof(PARTITION_2_NAME));
    add_gpt_entry(gpt_entries, 0, EFI_SYSTEM_PARTITION_GUID, PARTITION_2_GUID, 10, 0, partition_name);
    primary_header = generate_gpt_header(gpt_entries, 1, 1, DISK_1_GUID);
    backup_header = generate_gpt_header(gpt_entries, 1, 0, DISK_1_GUID);


    printf("Primary Header:\n");
    print_gpt_header(primary_header);
    printf("\nBackup Header:\n");
    print_gpt_header(backup_header);

    printf("\nPartition 1:\n");
    print_gpt_entry(&gpt_entries[0]);
    printf("\nPartition 2:\n");
    print_gpt_entry(&gpt_entries[1]);


    zeroes = calloc(1, 10 * SECTOR_SIZE);

    int fd = open("gpt.img", O_CREAT | O_RDWR);
    if (fd < 0) {
        perror("open");
        goto Free;
    }

    gpt_image = make_gpt_image(MBR, zeroes, primary_header, backup_header, gpt_entries, &gpt_image_size);
    printf("%u\n", gpt_image_size);

    // for (int i = 0; i < gpt_image_size; ++i) {
    //     if (gpt_image[i] != 0) {
    //         printf("%d: %d\n", i, gpt_image[i]);
    //         break;
    //     }
    // }

    write(fd, gpt_image, gpt_image_size);

    for (int i = 0; i < 8; ++i)
        printf("%c", ((char *)&primary_header->signature)[i]);
    printf("\n");

Free:
    free(zeroes);
    free(primary_header);
    free(backup_header);
    free(gpt_image);
    close(fd);

    return;
}


void read_image(void)
{
    printf("read\n");
    // uint32_t image_size = 16777216;
    // int fd = open("disk-image.raw", O_RDONLY);
    uint32_t image_size = 39424;
    int fd = open("gpt.img", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    uint8_t *image = malloc(image_size);
    read(fd, image, image_size);

    printf("Primary Header:\n");
    ptr_gpt_header_t header = (ptr_gpt_header_t)(image + SECTOR_SIZE);
    print_gpt_header(header);

    printf("\nBackup Header:\n");
    ptr_gpt_header_t backup_header = (ptr_gpt_header_t)(image + header->alternate_lba * SECTOR_SIZE);
    print_gpt_header(backup_header);

    ptr_gpt_entry_t entries = (ptr_gpt_entry_t)(image + header->partition_entry_lba * SECTOR_SIZE);
    printf("\nPartitions:\n");
    for (int i = 0; i < header->number_of_partition_entries; ++i) {
        printf("Partition %d:\n", i);
        print_gpt_entry(&entries[i]);
        if (entries[i].partition_type_guid[0] == 0)
            break;
    }

    // for (int i = 0; i < image_size; ++i) {
    //     if (image[i] != 0)
    //         printf("%d: %d\n", i, image[i]);
    // }
    printf("end\n");

Free:
    free(image);
    close(fd);
}


int main(void)
{
    generate_image();
    read_image();
    //char data[] = "hello-world1234567890";
    //printf("%u\n", crc32(data, sizeof(data)));
}

