#include <stdio.h>
#include <pthread.h>
#include <ncurses.h>

#define EXIT_KEY 113

int lastkey;
bool isrunning;

void* handlekeys() {
	while (isrunning) {
		lastkey = getch();
		if (lastkey == EXIT_KEY) {
			isrunning = false;
			break;
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	// Init window
	WINDOW* w = initscr();
	noecho();
	int mode = curs_set(0);
	int height = getmaxy(w);
	int width = getmaxx(w);
	clear();

	// Set up key handler
	pthread_t keyhandler;
	if (pthread_create(&keyhandler, NULL, &handlekeys, NULL)) {
		curs_set(mode);
		return 1;
	}

	// Set variables
	isrunning = true;
	long double pi = 0.0;
	unsigned long i = 0;

	while (isrunning) {
		if (i % 2 == 0) {
			pi += 1.0 / (1.0 + i * 2.0);
		} else {
			pi -= 1.0 / (1.0 + i * 2.0);
		}
		mvprintw(height / 2, width / 2 - 10, "%15.19Lf", 4.0 * pi);
		refresh();
		i++;
	}

	// Close key handler
	if (pthread_cancel(keyhandler)) {
		curs_set(mode);
		return 1;
	}

	// End window
	curs_set(mode);
	endwin();
	return 0;
}
