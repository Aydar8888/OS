#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define PATH_MAX 256
#define SHARED_SIZE 65536

int main() {
    char file_name[PATH_MAX];
    
    printf("Введите имя файла >> ");
    if (scanf("%s", file_name) != 1) {
        perror("Ошибка прочтения имени файла");
        exit(1);
    }

    int file = open(file_name, O_RDONLY);
    if (file == -1) {
        perror("Ошибка открытия файла");
        exit(1);
    }

    char shared_file[] = "temp.txt";

    int fd = open(shared_file, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Ошибка открытия файла"); 
        close(file);
        exit(1);    
    }

    ftruncate(fd, SHARED_SIZE);

    char *shared_mem = mmap(NULL, SHARED_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shared_mem == MAP_FAILED) {
        perror("Ошибка отображения файла в память");
        close(file);
        close(fd);
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0) {
        if (dup2(file, STDIN_FILENO) == -1) {
            perror("dup2");
            close(file);
            exit(1);
        }
        close(file);
        execl("./child", "child", shared_file, NULL);
        perror("execl");
        exit(1);
    } else {
        wait(NULL);
        printf("Результат: %s\n", shared_mem);

        munmap(shared_mem, SHARED_SIZE);
        close(fd);
        close(file);
        unlink(shared_file);
    }
}
