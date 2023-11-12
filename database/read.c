#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

char *read_last_line(FILE *fp)
{
    // Seek to the end of the file
    fseek(fp, 0, SEEK_END);

    // Get the file size
    long size = ftell(fp);

    // Allocate a buffer to store the last line
    char *line = malloc(size + 1);
    if (line == NULL)
    {
        return NULL;
    }

    // Seek to the beginning of the last line
    fseek(fp, -size, SEEK_END);

    // Read the last line
    fread(line, sizeof(char), size, fp);

    // Add a null terminator to the end of the line
    line[size] = '\0';

    // Return the last line
    return line;
}

char *read_from_file()
{
    char *FILE_NAME = "./database/file.txt";
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
    {
        // Handle error
        printf("Error opening %s\n", FILE_NAME);
        return NULL;
    }

    char *last_line = read_last_line(fp);
    fclose(fp);
    if (last_line == NULL)
    {
        // Handle error
        printf("Error last line = NULL\n");
        return NULL;
    }
    printf("%s\n", last_line);
}
