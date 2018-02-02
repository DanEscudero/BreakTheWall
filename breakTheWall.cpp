#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <math.h>

using namespace std;

#ifdef _WIN32

#elif __linux__

struct termios orig_termios;

/* Returns to original terminal */
void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

/* Sets special terminal */
void set_conio_terminal_mode()
{
	struct termios new_termios;

	/* take two copies - one for now, one for later */
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	/* register cleanup handler, and set the new terminal mode */
	atexit(reset_terminal_mode);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

/* Ckecks if keyboard was hit */
int kbhit()
{
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

/* Gets char from keyboard */
int getch()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) {
		return r;
	} else {
		return c;
	}
}

#endif

#define INIT_DELAY 0.15
#define HEIGHT 15
#define WIDTH 52
#define PI 3.1415

#include "brick.cpp"

/* Return time */
double time_now()
{
	return double (clock())/CLOCKS_PER_SEC;
}

/* Prints a line of '#' */
void printLine ()
{
	for (int x = 0; x <= WIDTH+1; x++) cout << '#';
	cout << endl;
}
/* Checks if the pair [i, j] belongs to the array of size n */
bool belongs (int i, int j, Brick *v, int n)
{
	for (int k = 0; k < n; k++)
		if (v[k].x == i && v[k].y == j && v[k].on) return true;
	return false;
}

/* Converts x to radians */
double rad (double x)
{
	return x*PI/180.0;
}

/* Return a random number between min and max */
int rand_range (int min, int max)
{
	srand(clock());
	return rand()*1.0/(RAND_MAX-1) * (max+1-min) + min;
}

/* Return the amount of digits in n */
int digits (int n)
{
	int ans = 0;

	while (n) {
		n /= 10;
		ans ++;
	}
	return ans;
}

int score = 0;
int maxScore;
int lifes = 5;

/* Prints ending text, with correct spacing */
void printEndText ()
{
	int len = 0;
	if (lifes == -1) {
		len = 28 + digits(score);
		for (int i = 0; i < (WIDTH - len)/2; i++) cout << ' ';
		cout << "YOU LOST         Your Score: " << score;
		for (int i = 0; i < (WIDTH - len)/2; i++) cout << ' ';
	}
	else if (score == maxScore) {
		len = 27 + digits(score);
		for (int i = 0; i < (WIDTH - len)/2; i++) cout << ' ';
		cout << "YOU WON         Your Score: " << score;
		for (int i = 0; i < (WIDTH - len)/2; i++) cout << ' ';
	}
	else {
		for (int i = 0; i < WIDTH; i++) cout << ' ';
	}
}

#include "paddle.cpp"
#include "ball.cpp"

int main () {
	bool gameOver = false;
	
	double t1 = time_now();
	double t2 = t1;
	double delay = INIT_DELAY;
	
	char input = ' ';
	
	int n_bricks = WIDTH/4;
	int n_lines = 3;
	
	maxScore = n_bricks * n_lines * 10;

	Paddle pad (WIDTH/2);
	Ball ball (WIDTH/2, HEIGHT/2);
	ball.setSpeed (rand_range(210, 330));
	Brick *all_bricks = new Brick [n_bricks * n_lines];
	
	/* Initializes all bricks in the correct positions */
	for (int i = 0, h = -1; i < n_bricks * n_lines; i++) {
		if (i%n_bricks == 0) h++;
		all_bricks[i].x = (i%n_bricks);
		all_bricks[i].y = h;
		all_bricks[i].on = true;
	}

	while (!gameOver) {
		/* SETUP TERMINAL */
		set_conio_terminal_mode();
		
		while (!kbhit() &&  t1-t2 < delay) t1 = time_now();
			
		if (t1-t2 < delay) input = getch();
		t2 = t1;
		
		reset_terminal_mode();
		system ("clear");
		/*----------------*/
	
		/* INPUT */
		switch (input) {
			case 'a':
			case 'A':
			case 'd':
			case 'D':
				pad.move(input);
				break;

			case 'q':
			case 'Q':
				gameOver = true;
				break;
		}
		input = ' ';
		/*-------*/
		
		/* LOGIC */
		ball.checkPaddle(pad);
		ball.checkRoof();
		ball.checkPass();
		ball.checkBrick(all_bricks, n_lines*n_bricks);
		ball.checkWall();
		ball.move();
		/*-------*/

		/* DRAW */
		printLine();
		for (int j = 0; j < HEIGHT; j++) {
			cout << "#";
			for (int i = 0; i < WIDTH; i++) {
				if (i >= pad.start && i <= pad.end && j == HEIGHT-1) cout << '=';
				
				else if (i == floor(ball.x) && j == floor(ball.y)) cout << 'O';
				
				else if (belongs(i/4, j, all_bricks, n_bricks*n_lines)) {
					if (i%4 == 0) cout << '[';
					if (i%4 == 1) cout << '+';
					if (i%4 == 2) cout << '+';
					if (i%4 == 3) cout << ']';
				}
				
				else cout << ' ';
			}
			cout << '#' << endl;
		}
		printLine();

		cout << "score: " << score << endl;
		cout << "lifes: " << lifes << endl;
		if (score == n_bricks*n_lines*10 || lifes == -1) gameOver = true;
		/*------*/
	}
	
	/* Ending text */
	system ("clear");
	printLine();
	for (int j = 0; j < HEIGHT; j++) {
		cout << "#";
		if (j == HEIGHT/2) printEndText();
		for (int i = 0; i < WIDTH; i++) {
			if (belongs(i/4, j, all_bricks, n_bricks*n_lines)) {
				if (i%4 == 0) cout << '[';
				if (i%4 == 1) cout << '+';
				if (i%4 == 2) cout << '+';
				if (i%4 == 3) cout << ']';
			}
			
			else if (j != HEIGHT/2) cout << ' ';
		}
		cout << '#' << endl;
	}
	printLine();

	return 0;
}
