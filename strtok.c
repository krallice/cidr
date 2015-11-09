#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	char *split;
	split = strtok(argv[1],"/");
	while (split != NULL) {
		printf("%s\n", split);
		split = strtok(NULL, "/");
	}
	return 0;
}
