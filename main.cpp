#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void usage()
{
    printf("Please provide a PPM file for processing\n");
}

void processFile(char *filename)
{
    char ppmVersion[128] = {0};
    int width = 0;
    int height = 0;
    int colors = 0;
    unsigned char *colorTripler = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    char *line = NULL;
    char partial_line[128] = {0};

    // Support P1, which is space/new line separated
    FILE *stream = fopen(filename, "r");

    if (NULL == stream)
    {
        printf("Failed to open: %s\n", filename);
        return;
    }

    // This includes the PPM version
    getline(&line, &len, stream);
    // printf("line: %s", line);
    strcpy(ppmVersion, line);
    printf("ppmVersion: %s", ppmVersion);

    // Width and Height
    getline(&line, &len, stream);
    char *space_loc = strstr(line, " ");
    if (NULL == space_loc)
    {
        printf("Malformed PPM\n");
        return;
    }

    memcpy(partial_line, line, space_loc - line);

    width = atol(partial_line);
    printf("width: %d\n", width);

    memcpy(partial_line, space_loc, strlen(line) - (space_loc - line));
    height = atol(partial_line);
    printf("height: %d\n", height);

    getline(&line, &len, stream);
    // printf("line: %s", line);

    colors = atol(line);
    printf("colors: %d\n", colors);

    size_t alloc_mem = sizeof(unsigned char) * 3 * (width * height);
    printf("alloc_mem: %zd\n", alloc_mem);
    colorTripler = (unsigned char *)malloc(alloc_mem);
    nread = getline(&line, &len, stream); // Should be a number with a space, and another number and a space, etc..
    char *end_line = strstr(line, "\n");
    if (NULL != end_line)
    {
        *end_line = 0;
    }
    // printf("nread: %zd, line: [%s]\n", nread, line);

    char *current_pos = line;
    int location = 0;
    while (NULL != current_pos)
    {
        char digit[128] = {0};
        char *space = strstr(current_pos, " ");
        if (NULL != space)
        {
            // Copy everything until space
            strncpy(digit, current_pos, space - current_pos);

            current_pos = space + strlen(" ");
        }
        else
        {
            strcpy(digit, current_pos);
        }

        // printf("digit: [%s]\n", digit);
        if (0 == strlen(digit))
        { // Reached the end
            nread = getline(&line, &len, stream);
            if (nread == -1)
            {
                break;
            }

            char *end_line = strstr(line, "\n");
            if (NULL != end_line)
            {
                *end_line = 0;
            }
            // printf("nread: %zd, line: [%s]\n", nread, line);
            current_pos = line;
            continue;
        }

        // printf("location: %d, digit: [%s]\n", location, digit);
        colorTripler[location++] = atoi(digit);
    }

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            printf("%x ", colorTripler[x + y * width]);
        }
        printf("\n\n");
    }

    printf("Read: %d pixels, into: %zd\n", location, alloc_mem);

    free(colorTripler);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage();
        return (0);
    }

    processFile(argv[1]);
}