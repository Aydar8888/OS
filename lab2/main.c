#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h> 

typedef struct {
    double x;
    double y;
    double z;
} point;

typedef struct {
    int x;
    int y;
    int z;
} point_index;

typedef struct {
    point* points;
    int n; 
} data;

typedef struct {
    data* input_data;
    int start;
    int end;
} thread_arg;

point_index* triangles = NULL;
int total_triangles = 0;
double max_area = 0.0;
pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;

data* generate_points() {
    int n = 1000;
    
    point* points = malloc(sizeof(point) * n);
    if (points == NULL) {
        fprintf(stderr, "Ошибка выделения памяти!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        points[i].x = (double)(i);
        points[i].y = (double)(i % 20);     
        points[i].z = (double)(i % 10);   
    }

    data* result = malloc(sizeof(data));
    if (result == NULL) {
        fprintf(stderr, "Ошибка выделения памяти!\n");
        free(points);
        exit(1);
    }

    result->n = n;
    result->points = points;
    
    return result;
}


data* input_points() {
    return generate_points();
    // int n = 0;
    // printf("Введите количество точек >> ");
    // if (scanf("%d", &n) != 1) {
    //     fprintf(stderr, "Ошибка! Не удалось прочитать количество точек!\n");
    //     exit(1);
    // }

    // if (n < 3) {
    //     fprintf(stderr, "Чтобы задать треугольник необходимо как минимум 3 точки!\n");
    //     exit(1);
    // }

    // point* points = malloc(sizeof(point) * n);
    // if (points == NULL) {
    //     fprintf(stderr, "Ошибка! Не удалось выделить память!\n");
    //     exit(1);
    // }

    // for (int i = 0; i < n; i++) {
    //     printf("Введите координаты точки № %d в виде x y z через пробел >> ", i);
    //     if (scanf("%lf %lf %lf", &points[i].x, &points[i].y, &points[i].z) != 3) {
    //         fprintf(stderr, "Ошибка! Не удалось прочитать координаты точки!\n");
    //         free(points);
    //         exit(1);
    //     }
    // }

    // data* result = malloc(sizeof(data));
    // if (result == NULL) {
    //     fprintf(stderr, "Ошибка выделения памяти для данных!\n");
    //     free(points);
    //     exit(1);
    // }

    // result->n = n;
    // result->points = points;
    // return result; 
}


point_index* create_triangle_index(data* data) {
    int n = data->n;
    long long count_triangles = (long long)(n * (n - 1) * (n - 2) / 6);
    
    point_index* result = malloc(sizeof(point_index) * count_triangles);
    if (result == NULL) {
        fprintf(stderr, "Ошибка выделения памяти!\n");
        exit(1);
    }

    int index = 0;
    for (int i = 0; i < n; i++) {          
        for (int j = i + 1; j < n; j++) {  
            for (int k = j + 1; k < n; k++) { 
                result[index].x = i;
                result[index].y = j;
                result[index].z = k;
                index++;
            }
        }
    }

    total_triangles = index; 
    return result;
}

double triangle_area(point a, point b, point c) {
    double abx = b.x - a.x;
    double aby = b.y - a.y;
    double abz = b.z - a.z;

    double acx = c.x - a.x;
    double acy = c.y - a.y;
    double acz = c.z - a.z;

    double cx = aby * acz - abz * acy; 
    double cy = abz * acx - abx * acz;  
    double cz = abx * acy - aby * acx;  

    double Sabcd = sqrt(cx*cx + cy*cy + cz*cz);
    return 0.5 * Sabcd;
}

void* calculation_area(void* arg) {
    thread_arg* t = (thread_arg*)arg;
    point* points = t->input_data->points;

    double local_max = 0.0;
    for (int i = t->start; i < t->end; i++) {
        point_index tri = triangles[i];
        double area = triangle_area(
            points[tri.x],
            points[tri.y],
            points[tri.z]
        );
        if (area > local_max) {
            local_max = area;
        }
    }

    pthread_mutex_lock(&result_mutex);
    if (local_max > max_area) {
        max_area = local_max;
    }
    pthread_mutex_unlock(&result_mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }
    
    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Ошибка! Количество потоков должно быть положительным!\n");
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start); 

    data* input_data = input_points();
    triangles = create_triangle_index(input_data);

    pthread_t* threads = malloc(sizeof(pthread_t) * num_threads);
    thread_arg* args = malloc(sizeof(thread_arg) * num_threads);

    int step = total_triangles / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].input_data = input_data;
        args[i].start = i * step;
        args[i].end = (i == num_threads - 1) ? total_triangles : (i + 1) * step;

        if (pthread_create(&threads[i], NULL, calculation_area, &args[i]) != 0) {
            fprintf(stderr, "Ошибка создания потока %d!\n", i);
            free(triangles);
            free(input_data->points);
            free(input_data);
            free(threads);
            free(args);
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end); 

    double t = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Максимальная площадь: %lf\n", max_area);
    printf("Время выполнения: %lf секунд\n", t);

    free(triangles);
    free(input_data->points);
    free(input_data);
    free(threads);
    free(args);

    return 0;
}


