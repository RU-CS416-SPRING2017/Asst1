#include "my_pthread_t.h"

// Temporyary test method.
void test(int signum) {
	printf("in test %d, %d\n", signum, signum * 1);
    printf("in test %d, %d\n", signum, signum * 2);
    printf("in test %d, %d\n", signum, signum * 3);
    printf("in test %d, %d\n", signum, signum * 4);
    int i;
    for (i = 0; i < 999999; i++) {
        int l = (((i+1) *32)/65)*(((i+1) *32)/65);
        // printf("%d ", l);
    }
    printf("in test %d, %d\n", signum, signum * 5);
    printf("in test %d, %d\n", signum, signum * 6);
    printf("in test %d, %d\n", signum, signum * 7);
    printf("in test %d, %d\n", signum, signum * 8);
    printf("in test %d, %d\n", signum, signum * 9);
}

// Temporary main for testing.
int main(int argc, char ** argv) {

	my_pthread_create(NULL, NULL, test, 1);
    printf("1\n");
    my_pthread_create(NULL, NULL, test, 2);
    printf("2\n");
    my_pthread_create(NULL, NULL, test, 3);
    printf("3\n");
    my_pthread_create(NULL, NULL, test, 4);
    printf("4\n");
    my_pthread_create(NULL, NULL, test, 5);

	printf("back in main\n");

    while(1);

	return 0;
}
