#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "sys/wait.h"
#include <stdlib.h>


int main() {
    int pipe_fd[2]; // pipe_fd[0] - чтение, pipe_fd[1] - запись 
    char file_name[256];
    double num;

    int err = pipe(pipe_fd);
    if (err == -1) {
        perror("pipe");
        return -1;
    }

    printf("Введите имя файла >> ");
    fgets(file_name, sizeof(file_name), stdin);
    size_t len = strlen(file_name);
    if (len > 0 && file_name[len - 1] == '\n') {
        file_name[len - 1] = '\0';
    }

    pid_t pid = fork();
    
    if (pid == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        execl("./child", "child", file_name, NULL);
        perror("execl");
        exit(0);
    } else {
        close(pipe_fd[1]);
        char buffer[256];
        ssize_t n;
        while ((n = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, n);  
        }
        if (n == -1) {
            perror("read");
        }
        close(pipe_fd[0]);
        wait(NULL);
        printf("\n");
    }
}

   