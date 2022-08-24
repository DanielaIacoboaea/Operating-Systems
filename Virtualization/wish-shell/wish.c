#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>


// built-in commands 
typedef struct path{
    char name[256];
}path;

path paths[10];


void fork_process(char *command_line, char *test_path)
{
    const char delimiter[] = "& >";
    char error_message[30] = "An error has occurred\n";
    char *command = NULL;
    char *next_command = NULL;
    char *p_input_command = command_line;
    int size = 1;
    int rc = fork();
    if (rc < 0)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }else if(rc == 0)
    {
        char *exec_args[50];
            while ((command = strsep(&p_input_command, delimiter)) != NULL)
            {
                size++;
                strcpy(exec_args[size-1], command);
            }
            size++;
            exec_args[size-1] = NULL;
        
        if (execv(test_path, exec_args))
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
    }else{
        int rc_wait = wait(NULL);
    }
}

void check_if_redirected_stdout(char *command_line){
    char *command = NULL;
    char *p_input_command = command_line;
    const char delimiter[] = "& >";
    char error_message[30] = "An error has occurred\n";
    int size_paths = 1;

    command = strsep(&p_input_command, delimiter);
    if (strcmp(command, "exit") == 0)
    {
        if ((command = strsep(&p_input_command, delimiter)) == NULL)
        {
           exit(0);
        }else{
            printf("%s\n", error_message);
        }
    }else if (strcmp(command, "path") == 0){
       
        if((command = strsep(&p_input_command, delimiter)) == NULL)
        {
            strcpy(paths[0].name, "\0");
            size_paths = 0;
        }else{
            strcpy(paths[0].name, command);
            while ((command = strsep(&p_input_command, delimiter)) != NULL)
            {
                size_paths++;
                strcpy(paths[size_paths-1].name, command);
            }
        }
      
    }else{
        bool found = false;
        if (size_paths == 0)
        {
          
            char save_command[256];
            if (strcmp(command, "cd") == 0)
            {
                if((command = strsep(&p_input_command, delimiter)) == NULL)
                {
                    printf("%s\n", error_message);
                    strcpy(save_command, command);
                }else if ((command = strsep(&p_input_command, delimiter)) == NULL){
                    chdir(save_command);
                }else{
                    printf("%s\n", error_message);
                }
            }
        }else{
          
            for (int i = 0; i < size_paths; i++)
            {
                char test_path[256];
                strcat(paths[i].name, command);
                strcpy(test_path, paths[i].name);
                if(access(test_path, X_OK) == 0)
                {
                    found = true;
                    fork_process(command_line, test_path); //changed p_input_command
                    break;
                }
            }
            if (!found)
            {
                printf("%s\n", error_message);
            }
        }
    }
}

void parse_command(char *command_line, char *redirect_file)
{
    strcpy(paths[0].name, "/bin/");
    int size_paths = 1;
    int fd;
    int stdout_copy;

    if (redirect_file != NULL)
    {
        stdout_copy = dup(1);
        close(1);
        fd = open(redirect_file, O_CREAT|O_WRONLY|O_TRUNC,  S_IRWXU);
        check_if_redirected_stdout(command_line);
        close(fd);
        dup2(stdout_copy, 1);

    }else{
        check_if_redirected_stdout(command_line);
    }
}

void split_in_processes(char *p_input){
 
    char *command = NULL;
    char *redirect_file = NULL;
    char *redirect_check = NULL;
    char *p_input_command = p_input;
    const char redirect_delimiter[] = ">";
    char error_message[30] = "An error has occurred\n";

    // 4 Cases:
    // 1.Multiple processes
    // 2. stdout redirection to file 
    // 3. change path for binary files
    // 4. exit 
    if (strstr(p_input, "&") == NULL)
    {
        // we have only one process to run
        // parse the input line to run the process 

        // check if we have to redirect the output
        if (strstr(p_input, ">") != NULL)
        {
            command = strsep(&p_input_command, redirect_delimiter);
            redirect_file = strsep(&p_input_command, redirect_delimiter);
            
            if ((redirect_check = strsep(&p_input_command, redirect_delimiter)) != NULL)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }else{
                parse_command(command, redirect_file);
            }
        }else{
            
            parse_command(p_input, redirect_file);
        }
    }else{
        printf("else multiple processes\n");
        // run multiple processes at the same time
    }
}

int main(int argc, char *argv[])
{
    char input_command[64];
    char *p_input = input_command;
    size_t input_size = 64;
    size_t chars;

    char file[256];

    char error_message[30] = "An error has occurred\n";

    // 2 types of input for the wish shell
    // standard input and txt file input
    if (argc == 1){

        //read from user input
        do{
            printf("wish> ");
            chars = getline(&p_input, &input_size, stdin);
            input_command[strlen(p_input)-1] = 0;
            split_in_processes(p_input);
        }while(strcmp(p_input, "exit") != 0);
        exit(0);
    }else if (argc == 2){

        //read from file
        strcpy(file, argv[1]);
        FILE* f = fopen(file, "r");
        if (f == NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        while (getline(&p_input, &input_size, f) != -1)
        {
            input_command[strlen(p_input)-1] = 0;
            split_in_processes(p_input);
        }
        fclose(f);
        exit(0);
    }else{
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    return 0;
}