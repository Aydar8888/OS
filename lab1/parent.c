#include <stdio.h>
#include <unistd.h>
#include "sys/wait.h"
#include <stdlib.h>
#include <fcntl.h>
#define PATH_MAX 256


int main() {
    int pipe_fd[2]; // pipe_fd[0] - чтение, pipe_fd[1] - запись 
    char file_name[PATH_MAX];

    printf("Введите имя файла >> ");
    if (scanf("%s", file_name) != 1) {
        fprintf(stderr, "Ошибка прочтения имени файла\n");
        return -1;
    }
    int file = open(file_name, O_RDONLY);

    if (file == -1) {
        perror("Ошибка открытия файла");
        return -1;
    }

    int err = pipe(pipe_fd);
    if (err == -1) {
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();

    if (pid == 0) {
        if (dup2(file, STDIN_FILENO) == -1) {
            perror("dup2");
            close(file);
            exit(1);
        }
        close(file);

        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            close(pipe_fd[1]);
            exit(1);
        }
        close(pipe_fd[1]);
        execl("./child", "child", NULL);
        perror("execl");
        exit(1);
    } else {
        close(pipe_fd[1]);        
        char buffer[256];
        int n;
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

   