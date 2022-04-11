#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errorcode.h"
#include "jpeg_decoder.h"

char prepipe[FILENAME_MAX];

int process(const char *filename)
{
    FILE *f = nullptr;

    if (strlen(filename) > 0)
    {
        f = fopen(filename, "rb");
    }
    else
    {
        fprintf(stderr, "Error usage: <input.jpg>\n");
        return USAGE_ERROR;
    }

    if (!f)
    {
        fprintf(stderr, "Error opening the input file %s.\n", filename);
        return OPEN_ERROR;
    }

    size_t size = 0;
    unsigned char *buf = nullptr;
    fseek(f, 0, SEEK_END);
    size = ftell(f);

    buf = (unsigned char *)malloc(size);
    fseek(f, 0, SEEK_SET);
    size_t read = fread(buf, 1, size, f);
    if (read < size)
    {
        fprintf(stderr, "Error reading the input file %s.\n", filename);
        return READ_ERROR;
    }

    fclose(f);

    Jpeg::Decoder decoder(buf, size);
    if (decoder.GetResult() != Jpeg::Decoder::OK)
    {
        fprintf(stderr, "Error decoding the input file %s.\n", filename);
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

int main(int argc, char *argv[])
{
    int result = SUCCESS;
    char *filename = (argc < 2) ? prepipe : argv[1];
    if (filename == prepipe)
    {
        do
        {
            memset(prepipe, 0, sizeof(prepipe));
            fgets(prepipe, sizeof(prepipe), stdin);

            for (char &p : prepipe)
            {
                // control code
                if (p < 31 || p > 126)
                {
                    p = 0;
                    break;
                }
            }
            if (strlen(prepipe) != 0)
            {
                result = process(filename);
            }

            if (result != SUCCESS)
            {
                break;
            }
        } while (strlen(prepipe) != 0);
    }
    else
    {
        result = process(filename);
    }

    return result;
}
