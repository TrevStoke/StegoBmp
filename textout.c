#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fpi;
    char bm[2];
    char pad[3];
    char pix[3];
    int r, c, m;
    int messLen;
    int cryptLen;
    int cryptIndex;
    int padBytes;
    int bit;
    int bitMask;
    int bufIndex;
    char messBuf[256];

    struct bmp
    {
        int size;
        short resv1;
        short resv2;
        int imageOffset;
        int infoHeaderSize;
        int width;
        int height;
        short planes;
        short bpp;
        int compress;
        int paddedSize;
        int hres;
        int vres;
        int col1;
        int col2;
    } header;

    if (argc != 2 && argc != 3)
    {
        printf("Usage: %s infile.bmp [cryptkey]\n", argv[0]);
        return(1);
    }

    if ((fpi = fopen(argv[1], "rb")) == NULL)
    {
        printf("Unable to open infile: %s\n", argv[1]);
        return(2);
    }

    fread(bm, sizeof(char), sizeof(bm), fpi);

    if (bm[0] != 'B' || bm[1] != 'M')
    {
        printf("%s is not a .bmp format file\n", argv[1]);
        return(4);
    }

    fread(&header, sizeof(struct bmp), (size_t) 1, fpi);
    padBytes = (4 - (3 * header.width) % 4) % 4;
    messLen = 255;

    bit = 7;
    bufIndex = 0;
    cryptIndex = 0;

    for (c = 0; c < header.height; ++c)
    {
        for (r = 0; r < header.width; ++r)
        {
            fread(pix, sizeof(char), sizeof(pix), fpi);

            if (bufIndex <= messLen)
            {
                bitMask = 1 << bit;
                messBuf[bufIndex] = 
                messBuf[bufIndex] & (0xFF - bitMask) | ((pix[0]&1)<<bit);

                if (bit == 0 && bufIndex == 0)
                    messLen = (int)messBuf[0];

                if (bit == 0)
                    ++bufIndex;

                bit = (bit + 7) % 8;
            }
        }

        if (padBytes != 0)
            fread(pad, sizeof(char), (size_t) padBytes, fpi);
    }

    fclose(fpi);

    if (argc == 3)
    {
        cryptLen = strlen(argv[2]);

        for (m = 0; m < messLen; ++m)
        {
            messBuf[m+1] = messBuf[m+1] ^ argv[2][cryptIndex];
            cryptIndex = (cryptIndex + 1) % cryptLen;
        }
    }

    messBuf[messLen+1] = '\0';
    printf("%s\n", &messBuf[1]);
    return(0);
}
