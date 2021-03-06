#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

unsigned char key[] = {
    0x54, 0x67, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x6e, 0x20, 0x61,
    0x6e, 0x63, 0x69, 0x65, 0x6e, 0x74, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6e,
    0x67, 0x2c, 0x20, 0x69, 0x74, 0x20, 0x72, 0x65, 0x70, 0x72, 0x65, 0x73,
    0x65, 0x6e, 0x74, 0x73, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6f, 0x72, 0x69,
    0x67, 0x69, 0x6e, 0x20, 0x6f, 0x66, 0x20, 0x61, 0x6c, 0x6c, 0x20, 0x62,
    0x69, 0x6e, 0x61, 0x72, 0x79, 0x20, 0x63, 0x68, 0x61, 0x72, 0x61, 0x63,
    0x74, 0x65, 0x72, 0x73, 0x2c, 0x20, 0x69, 0x73, 0x6e, 0x27, 0x74, 0x20,
    0x69, 0x74, 0x2e, 0x20, 0x4c, 0x65, 0x74, 0x20, 0x6d, 0x65, 0x20, 0x73,
    0x65, 0x65, 0x2c, 0x20, 0x69, 0x74, 0x20, 0x73, 0x61, 0x79, 0x73, 0x20,
    0x30, 0x2c, 0x31, 0x2c, 0x32, 0x2c, 0x33, 0x2c, 0x34, 0x2c, 0x35, 0x2c,
    0x36, 0x2c, 0x37, 0x2e, 0x2e, 0x2e, 0x67, 0xf3, 0xa3, 0xca, 0x23, 0x58,
    0xa3, 0xd1, 0xf8, 0xc1, 0x96, 0xe3, 0xd7, 0x85, 0x85, 0xfe, 0xbe, 0x7b,
    0xd2, 0x82, 0x59, 0xf4, 0xd8, 0xf0, 0x5f, 0xf5, 0xe2, 0x55, 0xe5, 0x2c,
    0x14, 0xdc, 0xd6, 0xf4, 0x60, 0xf9, 0x89, 0x84, 0x0c, 0x70, 0x50, 0xb8,
    0xf5, 0xde, 0x7f, 0xff, 0x5a, 0xc8, 0x8d, 0x61, 0xf0, 0x02};
unsigned int key_len = 178;

unsigned char fixed[0x1000];

unsigned char printable[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";
#define LEN (sizeof(printable) - 1)

inline static int same(unsigned char *a, unsigned char *b)
{
    int i;
    for (i = 0; i < key_len; i++)
    {
        if (*a == *b)
        {
            a++;
            b++;
        }
        else
        {
            break;
        }
    }
    return i - 131;
}

int main()
{
    int fd, i, ii, iii, offset, temp;
    unsigned char output[0x800], input[0x800] = "This is an ancient string, it represents the origin of all binary characters, isn't it."
                                                " Let me see, it says 0,1,2,3,4,5,6,7..."
                                                "d3ctf{w0W_sEems_u_bRe4k_uP_tHe_H1DdeN_s7R_By_Ae1tH_c0De}";

    fd = open("bin", O_RDONLY);
    mmap((void *)0x400000, 32768, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_DENYWRITE, fd, 0);
    close(fd);

    *(size_t *)0x407080 = (size_t)malloc;

    mprotect((void *)0x400000, 32768, PROT_EXEC | PROT_READ);

    ((void (*)(unsigned char *, int))0x402110)(fixed, 16);
    ((void (*)(unsigned char *))0x402B60)(fixed);

    memset(output, 0, sizeof(output));
    offset = 178;
    printf("%c%c\n\n", input[offset - 2], input[offset - 1]);

    for (i = 0; i < LEN; i++)
    {
        for (ii = 0; ii < LEN; ii++)
        {
            for (iii = 0; iii < LEN; iii++)
            {
                input[offset] = printable[i];
                input[offset + 1] = printable[ii];
                input[offset + 2] = printable[iii];

                ((void (*)(unsigned char *, int))0x402110)(fixed, 16);
                ((void (*)(unsigned char *))0x402B60)(fixed);
                ((void (*)(unsigned char *, unsigned char *, int, unsigned char *, int, int))0x403500)(output, input, 184, fixed, 126, 0);
                temp = same(key, output);
                if (44 <= temp)
                {
                    printf("%d: %c%c%c\n", temp, printable[i], printable[ii], printable[iii]);
                    // exit(EXIT_SUCCESS);
                }
            }
        }
    }

    return 0;
}
