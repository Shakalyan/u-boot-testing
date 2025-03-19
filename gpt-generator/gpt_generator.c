#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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


void generate_image(void)
{
    gpt_entry_t gpt_entries[128];
    ptr_gpt_header_t primary_header = NULL, backup_header = NULL;
    uint8_t *zeroes = NULL;
    uint8_t *gpt_image = NULL;
    uint32_t gpt_image_size = 0;

    memset(gpt_entries, 0, sizeof(gpt_entries));

    add_gpt_entry(gpt_entries, 0, PROTECTIVE_MBR_PARTITION_GUID, PARTITION_1_GUID, 1, 0, "protective MBR");
    add_gpt_entry(gpt_entries, 1, EFI_SYSTEM_PARTITION_GUID, PARTITION_2_GUID, 10, 0, "efi system");
    primary_header = generate_gpt_header(gpt_entries, 2, 1, DISK_1_GUID);
    backup_header = generate_gpt_header(gpt_entries, 2, 0, DISK_1_GUID);

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

    gpt_image = make_gpt_image(zeroes, zeroes, primary_header, backup_header, gpt_entries, &gpt_image_size);
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
    //uint32_t image_size = 16777216;
    //int fd = open("disk-image.raw", O_RDONLY);
    uint32_t image_size = 39424;
    int fd = open("gpt.img", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    uint8_t *image = malloc(image_size);
    read(fd, image, image_size);

    ptr_gpt_header_t header = (ptr_gpt_header_t)(image + SECTOR_SIZE);
    print_gpt_header(header);

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
}

