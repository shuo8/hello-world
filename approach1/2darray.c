#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
 
#define TIMES  8
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

void test(int r, int c) {
    struct timeval begin, middle, end;
    double duration;
    long unsigned num_b = sizeof(int) * r * c;
	int (*arr)[c] = malloc(num_b);
    long unsigned num_kb = num_b / 1024;
    double num_mb = num_kb / 1024.0;
    printf("%ld bytes (equals %ld kb, %.2lf mb) have been allocated\n", num_b,
	num_kb, num_mb);

    double total = 0;
    // Note that arr[i][j] is same as *(*(arr+i)+j)

	int count;
	int i, j;

    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            arr[i][j] = ++count; // OR *(*(arr+i)+j) = ++count

    int temp;
    for (int n = 0; n < TIMES; n++) {  
        gettimeofday(&begin, NULL);
        for (i = 0; i < r; i++)
            for (j = 0; j < c; j++) {
                temp = arr[i][j];
            }

        gettimeofday(&end, NULL);
        duration = CalElapsedTime(&begin, &end);
        total += duration;
        // printf("Time: %f with the execution\n", duration);
    }
    printf("Row based access time: %f\n", total / TIMES);

    total = 0;
    for (int n = 0; n < TIMES; n++) {
        gettimeofday(&begin, NULL);
        for (j = 0; j < c; j++)
            for (i = 0; i < r; i++) {
                temp = arr[i][j];
            }

        gettimeofday(&end, NULL);
        duration = CalElapsedTime(&begin, &end);
        total += duration;
        // printf("Time: %f with the execution\n", duration);
    }
    printf("Column based access time: %f\n\n", total / TIMES);

    /* Code for further processing and free the
       dynamically allocated memory */
 
    free(arr);
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

	for (r = 256; r <= 4096; r += 256) {
		test(r, c);
	}

/*
	for (c = 64; c <= 4096; c += 64) {
		test(1024, c);
	}
*/ 
    return 0;
}
