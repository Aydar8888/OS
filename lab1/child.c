#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Ошибка: неверное количество параметров\n");
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }
    
    char *file_name = argv[1];
    printf("Полученное имя файла: %s\n", file_name);
    int number, sum = 0;
    while (read(STDIN_FILENO, &number, sizeof(number)) > 0) {
        sum += number;
    }
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        perror("Ошибка открытия файла!");
        return 1;
    }
    printf("%d\n", sum);
    fprintf(file, "%d", sum);
    fclose(file);

}