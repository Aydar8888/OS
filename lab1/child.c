#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Неверное количество параметров");
        return -1;
    }
    
    char *file_name = argv[1];
    
    int file = open(file_name, O_RDONLY);
    if (file == -1) {
        perror("Ошибка открытия файла");
        return -1;
    }
    dup2(file, STDIN_FILENO);
    close(file);
    double first_number, num;

    
    
    if (scanf("%lf", &first_number) == 1) {
        printf("%lf ", first_number);
    }
        while (scanf("%lf", &num) == 1) {
        if (num == 0) {
            exit(0);
        }
        printf("%lf ", num / first_number);
    }
    return 0;
}