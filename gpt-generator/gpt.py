from gpt_image.disk import Disk
from gpt_image.partition import Partition, PartitionType

def generate():
    # create a new, 16 MB disk, size is in bytes
    disk = Disk("disk-image.raw")
    disk.create(512 * 76)

    # create a 2MB Linux partition named "boot"
    boot_part = Partition(
            "boot",
            512*2,#512,#2 * 1024 * 1024,
            PartitionType.EFI_SYSTEM_PARTITION.value
        )
    disk.table.partitions.add(boot_part)

    # create an 8MB Linux partition named "data"
    # data_part = Partition(
    #         "data",
    #         512, #8 * 1024 * 1024,
    #         PartitionType.LINUX_FILE_SYSTEM.value
    #     )
    # disk.table.partitions.add(data_part)

    # commit the change to disk
    disk.commit()

    # dump the current GPT information:

    print(disk)

def read():
    disk = Disk.open("disk-image.raw")
    #disk2 = Disk.open("gpt.img")
    print(disk)
    #print(disk2)

generate()
#read()