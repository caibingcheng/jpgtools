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
    if (!decoder.IsColor())
    {
        fprintf(stderr, "Error only support color image\n");
        return FORMAT_ERROR;
    }

    int count = 0;
    double r = 0.0, g = 0.0, b = 0.0;

    unsigned char *rgb = decoder.GetImage();
    size_t start = 0, end = decoder.GetImageSize();
    constexpr size_t step = 17 * 3;

    while (start < end)
    {
        count++;
        r += rgb[start + 0];
        g += rgb[start + 1];
        b += rgb[start + 2];
        start += step;
    }
    r /= count;
    g /= count;
    b /= count;

    fprintf(stdout, "( r g b )=( %lf %lf %lf )\n", r, g, b);

    return SUCCESS;
}
