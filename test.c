#include "my_pthread_t.h"

// Temporyary test method.
void test(int signum) {
    int i;
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
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, 9);

    my_pthread_exit(NULL);
}

// Temporary main for testing.
int main(int argc, char ** argv) {

    my_pthread_t thread;

	my_pthread_create(&thread, NULL, test, 1);
    printf("1\n");
    my_pthread_create(&thread, NULL, test, 2);
    printf("2\n");
    my_pthread_create(&thread, NULL, test, 3);
    printf("3\n");
    my_pthread_create(&thread, NULL, test, 4);
    printf("4\n");
    my_pthread_create(&thread, NULL, test, 5);

	printf("back in main\n");

    my_pthread_join(thread, NULL);

    printf("joined!\n");

	return 0;
}
