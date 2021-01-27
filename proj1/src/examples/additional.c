#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int main(int argc, char **argv){
	int fibo_res, max_res;

	fibo_res = fibonacci(atoi(argv[1]));
	max_res = max_of_four_int(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	

	printf("%d %d\n", fibo_res, max_res);

	return EXIT_SUCCESS;
}
