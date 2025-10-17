#include <stdio.h>
#include <stdlib.h>

int main() {
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


