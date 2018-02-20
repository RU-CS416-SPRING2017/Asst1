#include "my_pthread_t.h"
#include <stdio.h>
#include<stdlib.h>

my_pthread_mutex_t m; my_pthread_mutex_t n; my_pthread_mutex_t b;
my_pthread_mutex_t total; my_pthread_mutex_t print;

int sum = 0;
void tester(int signum) {

    my_pthread_mutex_lock(&m);my_pthread_mutex_lock(&n);my_pthread_mutex_lock(&b);

    int i;
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
		sum = sum + l;
    }
    printf("in test %d, %d\n", signum, 0);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
	printf("in test %d, %d\n", signum, 1);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 2);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 3);

    my_pthread_mutex_unlock(&b);
	my_pthread_mutex_unlock(&n); my_pthread_mutex_unlock(&m);
    
	my_pthread_mutex_lock(&m);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 4);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", sum);
    }
    printf("in test %d, %d\n", signum, 8);
    
    my_pthread_mutex_unlock(&m);

    char * testString = malloc(10);
	fflush(stdin);
	printf("test %d sum %d \n", signum, sum);
	fflush(stdin);
	sprintf(testString, "test %d sum %d", signum, sum);

    my_pthread_exit(testString);
}


// Temporyary test method.
void test(int signum) {

    my_pthread_mutex_lock(&m);

    int i;
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 0);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
	printf("in test %d, %d\n", signum, 1);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 2);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 3);

    my_pthread_mutex_unlock(&m);

    my_pthread_mutex_lock(&m);
    
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 4);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 5);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 6);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 7);
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 8);
    
    my_pthread_mutex_unlock(&m);

    char * testString = malloc(10);
    fflush(stdin);
	printf("test %d sum %d", signum, sum);
	fflush(stdin);
	sprintf(testString, "test %d", signum);

    my_pthread_exit(testString);
}

// Temporary main for testing.
int main(int argc, char ** argv) {

    my_pthread_mutex_init(&m, NULL);
	my_pthread_mutex_init(&n, NULL);
	my_pthread_mutex_init(&b, NULL);


    my_pthread_t thread[6];

	my_pthread_create(&(thread[0]), NULL, test, 0);
    printf("1\n");
    my_pthread_create(&(thread[1]), NULL, tester, 1);
    printf("2\n");
    my_pthread_create(&(thread[2]), NULL, test, 2);
    printf("3\n");
    my_pthread_create(&(thread[3]), NULL, tester, 3);
    printf("4\n");
    my_pthread_create(&(thread[4]), NULL, test, 4);
	printf("5\n");
    my_pthread_create(&(thread[5]), NULL, test, 5);

	printf("back in main\n");

    char * string[6];
    int i;

    for (i = 0; i < 6; i++) {
        my_pthread_join(thread[i], &(string[i]));
        printf("joined! %s\n", string[i]);
    }
    
	return 0;
}
