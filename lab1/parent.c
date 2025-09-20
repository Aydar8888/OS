#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "sys/wait.h"

int main() {
    int pipe_fd[2];
    char file_name[256];
    int number;

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
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        execl("./child", "child", file_name, NULL);
        perror("execl failed");
        return -1;
    } else {
        close(pipe_fd[0]); // pipe_fd[0] - чтение, pipe_fd[1] - запись    
        printf("Введите числа через пробел (Ctrl+D для завершения):\n");
        while (scanf("%d", &number) == 1) {
            write(pipe_fd[1], &number, sizeof(number));
        }
        close(pipe_fd[1]); 
        wait(NULL);      
    }
}




