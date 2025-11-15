#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define SHARED_SIZE 65536

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Неверное количество аргументов\n");
        exit(1);
    }

    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("Ошиюбка открытия файла");
        exit(1);
    }

    char *shared_mem = mmap(NULL, SHARED_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("Ошибка отображения файла в память");
        exit(1);
    }

    double first_number, num; 
    int pos = 0;
    
    if (scanf("%lf", &first_number) == 1) {
        pos += sprintf(shared_mem + pos, "%lf ", first_number);
    }
    
    while (scanf("%lf", &num) == 1) {
        if (num == 0) {
            break;
        }
        pos += sprintf(shared_mem + pos, "%lf ", num / first_number);
    }
    
    munmap(shared_mem, SHARED_SIZE);
    close(fd);
    return 0;
}