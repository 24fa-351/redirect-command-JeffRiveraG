#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <inp> <cmd> <out>\n", argv[0]);
        return 1;
    }

    int inp_fd = -1;
    if (strcmp(argv[1], "-") != 0) {
        inp_fd = open(argv[1], O_RDONLY);
        if (inp_fd < 0) {
            perror("Error opening input file");
            return 1;
        }
    }

    int out_fd = -1;
    if (strcmp(argv[3], "-") != 0) {
        out_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd < 0) {
            perror("Error opening output file");
            return 1;
        }
    }

    char *cmd = strtok(argv[2], " ");
    char *cmd_args[10];  
    int i = 0;

    while (cmd != NULL) {
        cmd_args[i++] = cmd;
        cmd = strtok(NULL, " ");
    }
    cmd_args[i] = NULL;  

    pid_t pid = fork();
    if (pid == 0) {

        if (inp_fd != -1) {
            dup2(inp_fd, STDIN_FILENO);
            close(inp_fd);
        }
        
        if (out_fd != -1) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        
        execvp(cmd_args[0], cmd_args);
        perror("Error executing command");
        exit(1);
    } else if (pid > 0) {
        
        wait(NULL);  
    } else {
        perror("Error forking process");
        return 1;
    }

    if (inp_fd != -1) close(inp_fd);
    if (out_fd != -1) close(out_fd);

    return 0;
}
