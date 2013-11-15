#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char **argv)
{
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

    FILE *fpi, *fpo;
    char bm[2];
    char pad[3];
    char pix[3];
    int r, c, m;
    int messLen;
    int cryptLen;
    int cryptIndex;
    int padBytes;
    int bit;
    int bufIndex;
    int bufLen;
    char messBuf[256];

    if (argc != 4 && argc != 5)
    {
        printf("Usage: %s infile.bmp outfile.bmp \"message\" [cryptkey]\n", argv[0]);
        return(1);
    }

    if ((fpi = fopen(argv[1], "rb")) == NULL)
    {
        printf("Unable to open infile: %s\n", argv[1]);
        return(2);
    }

    if ((fpo = fopen(argv[2], "wb")) == NULL)
    {
        printf("Unable to open outfile: %s\n", argv[2]);
        return(3);
    }

    fread(bm, sizeof(char), sizeof(bm), fpi);

    if (bm[0] != 'B' || bm[1] != 'M')
    {
        printf("%s is not a .bmp format file\n", argv[1]);
        return(4);
    }

    fread(&header, sizeof(struct bmp), (size_t) 1, fpi);

    if (header.bpp != 24)
    {
        printf("%s only works on 24 bpp files\n", argv[0]);
        return(5);
    }

    fwrite(bm, sizeof(char), sizeof(bm), fpo);
    fwrite(&header, sizeof(struct bmp), (size_t) 1, fpo);

    padBytes = (4 - (3 * header.width) % 4) % 4;
    messLen = strlen(argv[3]);

    if (argc == 5)
        cryptLen = strlen(argv[4]);
    else
        cryptLen = 0;

    bufLen = messLen + 1;
    messBuf[0] = (char)messLen;
    bit = 7;
    bufIndex = 0;
    cryptIndex = 0;

    for (m = 0; m < messLen; ++m)
    {
        messBuf[m+1] = argv[3][m];

        if (argc == 5)
        {
            messBuf[m+1] = messBuf[m+1] ^ argv[4][cryptIndex];
            cryptIndex = (cryptIndex + 1) % cryptLen;
        }
    }

    for (c = 0; c < header.height; ++c)
    {
        for (r = 0; r < header.width; ++r)
        {
            fread(pix, sizeof(char), sizeof(pix), fpi);

            if (bufIndex < bufLen)
            {
                pix[0] = (pix[0]&0xFE) | ((messBuf[bufIndex]&(1<<bit)) > 0);

                if (bit == 0)
                    ++bufIndex;

                bit = (bit + 7) % 8;
            }

            fwrite(pix, sizeof(char), sizeof(pix), fpo);
        }

        if (padBytes != 0)
        {
            fread(pad, sizeof(char), (size_t) padBytes, fpi);
            fwrite(pad, sizeof(char), (size_t) padBytes, fpo);
        }
    }

    fclose(fpi);
    fclose(fpo);
    return(0);
}
