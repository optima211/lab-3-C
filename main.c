#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

struct person {
    char name[20];
    char sex;
    int year;
    int height;
};

int save(char *filename, struct person *st, int n);

int load(char *filename);

int main(void) {
    char *filename = "people.dat";
    struct person people[] = {"Tom", 'M', 1993, 188, "Alice", 'F', 1999, 170, "Bob", 'M', 1997, 183, "Kate", 'F', 1998,
                              184};
    int n = sizeof(people) / sizeof(people[0]);

    save(filename, people, n);
    load(filename);
    return 0;
}

// запись в файл массива структур
int save(char *filename, struct person *st, int n) {
    FILE *fp;
    char *c;

    // число записываемых байтов
    int size = n * sizeof(struct person);

    if ((fp = fopen(filename, "wb")) == NULL) {
        perror("Error occured while opening file");
        return 1;
    }
    // записываем количество структур
    c = (char *) &n;
    for (int i = 0; i < sizeof(int); i++) {
        putc(*c++, fp);
    }

    // посимвольно записываем в файл все структуры
    c = (char *) st;
    for (int i = 0; i < size; i++) {
        putc(*c, fp);
        c++;
    }
    fclose(fp);
    return 0;
}

// загрузка из файла массива структур
int load(char *filename) {
    FILE *fp;
    char *c;
    int m = sizeof(int);
    int n, i;


    // выделяем память для количества данных
    int *pti = (int *) malloc(m);

    if ((fp = fopen(filename, "r")) == NULL) {
        perror("Error occured while opening file");
        return 1;
    }
    // считываем количество структур
    c = (char *) pti;
    while (m > 0) {
        i = getc(fp);
        if (i == EOF) break;
        *c = i;
        c++;
        m--;
    }
    //получаем число элементов
    n = *pti;

    // выделяем память для считанного массива структур
    struct person *ptr = (struct person *) malloc(n * sizeof(struct person));
    c = (char *) ptr;
    // после записи считываем посимвольно из файла
    while ((i = getc(fp)) != EOF) {
        *c = i;
        c++;
    }
    // перебор загруженных элементов и вывод на консоль
    printf("\n%d people in the file stored\n\n", n);
    printf("#, name,      sex,      year_of_birth,      height\n");
    int max[4], index, min;
    for (int k = 0; k < n; k++) {
        printf("%-2d %-6s %5c %12d %18d\n", k + 1, (ptr + k)->name, (ptr + k)->sex, (ptr + k)->year, (ptr + k)->height);
        max[k] = (ptr + k)->year;
    }
    min = 9999;
    for (int k = 0; k < n; k++) {
        if (min >= max[k]) {
            index = k;
            min = max[k];
        }
    }
    printf("very old is %d and her number %d", min, index + 1);
    free(pti);
    free(ptr);
    fclose(fp);
    return 0;
}