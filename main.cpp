#include <iostream>

#include "shell.h"

/*
	TODO

	- copia incolla
	- movimenti con frecce
	- shortcuts
*/

using namespace std;

int main(int argc, char* argv[]) {

	Shell shell;
	if (argv[1]) shell.setCurrentPath(argv[1]);

	shell.loadConfig();

	shell.run();

    return 0;
}
