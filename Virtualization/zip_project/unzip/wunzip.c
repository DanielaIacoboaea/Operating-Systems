#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //save the filenames from user input in an array
    char filename_unzip[256];

    //read in the count variable how many identical characters we need to print
    int count;

    //read the chracters from the files
    char character;

    //if no file is provided, exit
    if (argc < 2)
    {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }else {
        for (int i = 1; i < argc; i++)
        {
            //read from all the input files and write to stdout the unziped output
            strcpy(filename_unzip, argv[i]);
            FILE *file_r = fopen(filename_unzip, "rb");
            if (file_r == NULL)
            {
                printf("wunzip: cannot open file\n");
                exit(1);
            }
            
            //the zipped file has the format: count (how many of the next letter) and letter
            //for each count, read the next value (letter) 
            while (fread(&count, sizeof(int), 1, file_r) == 1)
            {
                fread(&character, sizeof(char), 1, file_r);
                //print as many letters as the count indicates
                for (int i = 0; i < count; i++)
                {
                    printf("%c", character);
                }
            }
            //close the file before opening the next one
            fclose(file_r);
        }
    }
    return 0;
}