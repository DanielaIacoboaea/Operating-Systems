#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    char filename[256];
    char word[1000];
    int line_length;

    if (argc < 2)
    {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }else if (argc == 2)
    {
        strcpy(word, argv[1]);
        if (strcmp(word, " ") == 0)
        {
            return 0;
        }
        char input_line[256];
        while (fgets(input_line, sizeof(input_line), stdin))
        {
            if (strstr(input_line, word) != NULL)
            {
                printf("%s", input_line);
            }
        }
    }else {
        for (int i = 2; i < argc; i++)
        {
            strcpy(filename, argv[i]);
            strcpy(word, argv[1]);
            if (strcmp(word, " ") == 0)
            {
                return 0;
            }
            FILE *file = fopen(filename, "r");
            if (file == NULL)
            {
                printf("wgrep: cannot open file\n");
                exit(1);
            } 
            char line[1000];
            while(fgets(line, sizeof(line), file) != NULL)
            {
                if (strstr(line, word) != NULL)
                {
                    printf("%s", line);
                }
            }
            fclose(file);
        }
    }
    return 0;
}