// maths.c
#include <stdio.h>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int addNums(int a, int b) {
	return a + b;
}

EMSCRIPTEN_KEEPALIVE
int main() {
	printf("Sum: %d\n", addNums(10, 13));
	return 0;
}
