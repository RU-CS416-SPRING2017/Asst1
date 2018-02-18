#include "my_pthread_t.h"

// Temporyary test method.
void test(int signum) {
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

    char * testString = malloc(10);
    sprintf(testString, "test %d", signum);

    my_pthread_exit(testString);
}

// Temporary main for testing.
int main(int argc, char ** argv) {

    my_pthread_t thread[5];

	my_pthread_create(&(thread[0]), NULL, test, 0);
    printf("1\n");
    my_pthread_create(&(thread[1]), NULL, test, 1);
    printf("2\n");
    my_pthread_create(&(thread[2]), NULL, test, 2);
    printf("3\n");
    my_pthread_create(&(thread[3]), NULL, test, 3);
    printf("4\n");
    my_pthread_create(&(thread[4]), NULL, test, 4);

	printf("back in main\n");

    char * string[5];
    int i;

    for (i = 0; i < 5; i++) {
        my_pthread_join(thread[i], &(string[i]));
        printf("joined! %s\n", string[i]);
    }
    
	return 0;
}
