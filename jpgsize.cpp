#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errorcode.h"
#include "jpeg_decoder.h"

int main(int argc, char *argv[])
{
    size_t size;
    unsigned char *buf;
    FILE *f;

    if (argc < 2)
    {
        fprintf(stderr, "Error usage: %s <input.jpg>\n", argv[0]);
        return USAGE_ERROR;
    }
    f = fopen(argv[1], "rb");
    if (!f)
    {
        fprintf(stderr, "Error opening the input file.\n");
        return OPEN_ERROR;
    }

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    buf = (unsigned char *)malloc(size);
    fseek(f, 0, SEEK_SET);
    size_t read = fread(buf, 1, size, f);
    fclose(f);

    Jpeg::Decoder decoder(buf, size);
    if (decoder.GetResult() != Jpeg::Decoder::OK)
    {
        fprintf(stderr, "Error decoding the input file\n");
        return DECODE_ERROR;
    }

    const auto width = decoder.GetWidth();
    const auto height = decoder.GetHeight();
    const auto pixels = width * height;
    fprintf(stdout, "( width height pixels size )=( %d %d %d %zu )\n", width, height, pixels, read);

    return SUCCESS;
}
