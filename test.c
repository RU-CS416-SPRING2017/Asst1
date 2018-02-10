#include "my_pthread_t.h"

// Temporyary test method.
void test(int signum) {
	printf("in test\n");
}

// Temporary main for testing.
int main(int argc, char ** argv) {

	my_pthread_create(NULL, NULL, test, 123);

	printf("back in main\n");

    while(1);

	return 0;
}
