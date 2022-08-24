#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//the compressed file needs to have
//4-byte integer in binary format followed by the single character in ASCII. 

int main(int argc, char *argv[])
{
    //save the filenames from user input in an array
    char filename_input[256];

    //keep a count for the repeating characters 
    int count = 0;

    //compare each character with the next one 
    char character, next_character;

    //exit if the user does not provide any files 
    if (argc < 2)
    {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }else {
        //loop through all the files and print the bytes output to stdout
        //user input uses > to redirect to a ziped file

        for (int i = 1; i < argc; i++)
        {
            strcpy(filename_input, argv[i]);
            FILE *file_r = fopen(filename_input, "r");
            if (file_r == NULL)
            {
                printf("wzip: cannot open file\n");
                exit(1);
            }

            //when reading the first file character will be the first character encountered in the file 
            if (i == 1)
            {
                fread(&character, sizeof(char), 1, file_r);
                count++;
            }else{

                //from second file will treat the first character from the file as a next character 
                //so that we will compressed all the files in a single output 
                fread(&next_character, sizeof(char), 1, file_r);

                //if the read character is the same increase the count 
                if (character == next_character)
                {
                    count++;
                }else{
                    //otherwise write to output
                    fwrite(&count, sizeof(int), 1, stdout);
                    fwrite(&character, sizeof(char), 1, stdout);
                    fflush(stdout);
                    count = 1;
                    character = next_character;
                }
            }
            
            //treat the cases that are not the first character read from the file 
            while (fread(&next_character, sizeof(char), 1, file_r) == 1)
            {
                if (character == next_character)
                {
                    count++;
                }else{
                    fwrite(&count, sizeof(int), 1, stdout);
                    fwrite(&character, sizeof(char), 1, stdout);
                    fflush(stdout);
                    //reset the count
                    count = 1;
                    //update character variable with the next different character
                    character = next_character;
                }
            }
            //close the file before opening the next one
            fclose(file_r);
        }

        //if the last character from the file is the same as the previous
        //does not enter the else block to print the output
        //make another write for this case
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&character, sizeof(char), 1, stdout);
        fflush(stdout);
    }
    return 0;
}