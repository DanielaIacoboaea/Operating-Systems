#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    //save the filenames from user input in an array
    char filename[256];

    //if no file is provided print an error
    if (argc < 2)
    {
        return 0;
    }else {
        //for each filename, save line by line in a char array and print it 
        for (int i = 1; i < argc; i++)
        {
            strcpy(filename, argv[i]);
            FILE *file = fopen(filename, "r");
            if (file == NULL)
            {
                printf("wcat: cannot open file\n");
                exit(1);
            } 
            char line[256];
            while(fgets(line, sizeof(line), file) != NULL)
            {
                printf("%s", line);
            }
            fclose(file);
        }
    }
    return 0;
}