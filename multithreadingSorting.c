#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

// Número de elementos do array
#define MAX 50000000
// Número de threads
#define THREAD_MAX 1

// Array a ser preenchido
int *a;

// Parâmetros para controle das threads
struct task 
{
    int task_no;
    int task_low;
    int task_high;
};

void
merge(int low, int mid, int high)
{
    int n1 = mid - low + 1, 
        n2 = high - mid, k, i, j, 
        *left = malloc(n1 * sizeof(int)), 
        *right = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        left[i] = a[i + low];
    for (i = 0; i < n2; i++)
        right[i] = a[i + mid + 1];
    
    k = low; 
    i = j = 0;

    while (i < n1 && j < n2) 
    {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }
    while (i < n1)
        a[k++] = left[i++];
    while (j < n2)
        a[k++] = right[j++];

    free(left); 
    free(right);
}
 
// Função Merge_Sort
void
merge_sort(int low, int high)
{
    int mid = low + (high - low) / 2;
    if (low < high) 
    {
        // Primeira metade
        merge_sort(low, mid);
        // Segunda metade
        merge_sort(mid + 1, high);
        // Junta as duas metades ordenadas
        merge(low, mid, high);
    }
}

// Função para multi-threading
void *
merge_sort123(void *arg)
{
    int low, high;
    struct task *task = arg;

    low = task->task_low;
    high = task->task_high;
    int mid = low + (high - low) / 2;

    if (low < high) 
    {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }

    return 0;
}
int
main(int argc, char **argv)
{
    int len = MAX / THREAD_MAX;
    int low = 0;
    pthread_t threads[THREAD_MAX];
    struct task *task;
    struct task tasklist[THREAD_MAX];
    time_t secondsIni, secondsEnd;

    // Aloca o array[MAX]
    a = malloc(sizeof(int) * MAX);

    // Preenche o array[MAX] com valores aleatórios
    for (int i = 0; i < MAX; i++) 
        a[i] = rand() % 100;

    for (int i = 0; i < THREAD_MAX; i++, low += len) 
    {
        task = &tasklist[i];
        task->task_no = i;
        task->task_low = low;
        task->task_high = low + len - 1;
        if (i == (THREAD_MAX - 1))
            task->task_high = MAX - 1;
        else 
        {
            task->task_low = i * (MAX / THREAD_MAX);
            task->task_high = (i + 1) * (MAX / THREAD_MAX) - 1;
        }
    }

    secondsIni = time(NULL);
    
    // cria as THREAD_MAX threads
    for (int i = 0; i < THREAD_MAX; i++) 
    {
        task = &tasklist[i];
        pthread_create(&threads[i], NULL, merge_sort123, task);
    }
    
    // join para todas as THREAD_MAX threads
    for (int i = 0; i < THREAD_MAX; i++)
        pthread_join(threads[i], NULL);

    struct task *task_m = &tasklist[0];
    for (int i = 1; i < THREAD_MAX; i++) {
        struct task *task = &tasklist[i];
        merge(task_m->task_low, task->task_low - 1, task->task_high);
    }
    
    secondsEnd = time(NULL);
    printf("Time Elapsed: %lds", secondsEnd - secondsIni);
    
    // Apresenta o array ordenado em tela (não aconselho kk)

    // printf("\n\nSorted array:");
    // for (int i = 0; i < MAX; i++)
    //     printf(" %d", a[i]);
    // printf("\n");
    printf("\n\nNumber of threads:");
    printf(" %d\n", THREAD_MAX);
    printf("\n\nArray size:");
    printf(" %d\n", MAX);
    printf("\n");

    return 0;
}