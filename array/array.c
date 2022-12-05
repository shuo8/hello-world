#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
 
#define TIMES   8
#define SIZE1   256 * 1024
#define SIZE2   3 * 1024 * 1024   
/*return the duration in seconds */
double
CalElapsedTime(struct timeval* tv_begin, struct timeval* tv_end)
{
    double ela_time;
    long ela_secs, ela_usecs;

    if(tv_end->tv_usec >= tv_begin->tv_usec)
    {
	ela_secs  = tv_end->tv_sec  - tv_begin->tv_sec;
        ela_usecs = tv_end->tv_usec - tv_begin->tv_usec;
    } else {
        ela_secs  = tv_end->tv_sec  - tv_begin->tv_sec - 1;
        ela_usecs = tv_end->tv_usec - tv_begin->tv_usec + 1000000;
    }

    ela_usecs += ela_secs * 1000000;
    ela_time = (double)ela_usecs / 1000000;

    return ela_time;
}

void print_usage(char **argv)
{
    printf("Usage:  %s -r length -c width\n", *argv);
}

void clear_cache() {
    int temp;
    int *arr = malloc(sizeof(int) * 4 * 1024 * 1024); // allocate 16 MB

    for (int i = 0; i < 4194304; i++)
        arr[i] = i;

    for (int i = 0; i < 4194304; i++)
        temp = arr[i];

    free(arr);
}

void test(int flag) {
    struct timeval begin, middle, end;
    double duration;
    int temp;

	int *arr1 = malloc(sizeof(int) * SIZE1); // 1 MB
	int *arr2 = malloc(sizeof(int) * SIZE2); // 12 MB

	int count = 0;
    for (int i = 0; i < SIZE1; i++)
        arr1[i] = ++count;

    gettimeofday(&begin, NULL);
    for (int n = 0; n < 120; n++) {  
        for (int i = 0; i < SIZE1; i++)
            //temp = arr1[i];
            arr1[i]++;
    }
    gettimeofday(&end, NULL);
    duration = CalElapsedTime(&begin, &end);
    printf("access time: %lf\n", duration);

	count = 0;
    for (int i = 0; i < SIZE2; i++)
        arr2[i] = ++count;

    gettimeofday(&begin, NULL);
    for (int n = 0; n < 10; n++) {  
        for (int i = 0; i < SIZE2; i++)
            //temp = arr2[i];
            arr2[i]++;
    }
    gettimeofday(&end, NULL);
    duration = CalElapsedTime(&begin, &end);
    printf("access time: %lf\n", duration);

    free(arr1);
    free(arr2);

}

int main(int argc, char *argv[])
{
    int opt;
    int r = 64, c = 1024;
 
	while ((opt = getopt (argc, argv, "r:c:")) != -1)
		switch (opt) {
			case 'r':
			  r = atol(optarg);
			  break;
			case 'c':
			  c = atol(optarg);
			  break;
			default:
			  print_usage(argv);
			  abort ();
		}

    test(0);
    return 0;
}
