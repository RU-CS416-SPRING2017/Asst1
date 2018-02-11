#include "my_pthread_t.h"

// Temporyary test method.
void test(int signum) {
	printf("in test %d\n", signum);
}

// Temporary main for testing.
int main(int argc, char ** argv) {

	my_pthread_create(NULL, NULL, test, 1);
    my_pthread_create(NULL, NULL, test, 2);
    my_pthread_create(NULL, NULL, test, 3);
    my_pthread_create(NULL, NULL, test, 4);
    my_pthread_create(NULL, NULL, test, 5);

	printf("back in main\n");

    while(1);

	return 0;
}
