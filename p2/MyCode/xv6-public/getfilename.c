
#include "types.h"
#include "param.h"
#include "stat.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"
#include "user.h"

int main(int argc, char * argv[])
{

    if (argc < 2)
        printf(1, "No argument provided.\n");

    char * output = malloc(256);
    int fd = open(argv[1], O_RDWR);

    if (fd == 0)
        printf(1, "File open error\n");

    int return_code = getfilename(fd, output, 256);

    if (return_code < 0)
        printf(1, "Get file name error\n");

    printf(1, "XV6_TEST_OUTPUT Open filename: %s\n", output);
    exit();

}