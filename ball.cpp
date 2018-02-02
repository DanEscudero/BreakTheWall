class Ball
{
public:
	double x;
	double y;

	double xspeed;
	double yspeed;

	int ang;

	Ball(int x, int y)
	{
		this->x = x;
		this->y = y;
		this->xspeed += 1;
		this->yspeed += 1;
	}

	~Ball()
	{

	}
	
	/* Moves Ball */
	void move ()
	{
		this->x = this->x + this->xspeed;
		this->y = this->y + this->yspeed;
	}
	
	/* Sets balls speed based on angle a */
	void setSpeed (int a)
	{
		double ang_rad = rad(a);		
		xspeed = cos(ang_rad);
		yspeed = sin(ang_rad);
	}
	
	/* Checks if ball hits paddle and hits back */
	void checkPaddle (Paddle p)
	{
		if (floor(this->x) >= p.start 
		 && floor(this->x) <= p.end 
		 && floor(this->y) == HEIGHT-2) {
			hitBack(p);
		}
	}

	/* Hits ball up, with angle based on paddles position */
	void hitBack (Paddle p)
	{
		this->yspeed *= -1;
		switch (int(p.end - this->x - p.len/2)) {
			case -2:
				this->setSpeed(rand_range(300, 315));
				break;
			case -1:
				this->setSpeed(rand_range(285, 300));
				break;
			case 0:
				this->setSpeed(rand_range(270, 285));
				break;
			case 1:
				this->setSpeed(rand_range(255, 270));
				break;
			case 2:
				this->setSpeed(rand_range(240, 255));
				break;
			case -3:
				this->setSpeed(rand_range(225, 240));
				break;
		}
	}

	/* Checks if ball hits roof and hits back down */
	void checkRoof ()
	{
		if (floor(this->y) == -1) this->yspeed *= -1;
	}
	
	/* Checks if ball hits wall and hits back */
	void checkWall ()
	{
		if (floor(this->x) == 0
		 || floor(this->x) == WIDTH-1) this->xspeed *= -1;
	}
	
	/* Checks if ball hits a brick, hits back and increases score */
	void checkBrick (Brick *v, int n)
	{
		for (int i = 0; i < n; i++) {
			if (v[i].on && v[i].x == floor(this->x/4)
			 && v[i].y == floor(this->y)) {
				v[i].on = false;
				this->yspeed *= -1;
				score += 10;
			}
		}
	}
	
	/* Checks if ball passes paddle, resets and decreases life */
	bool checkPass ()
	{
		if (floor(this->y) == HEIGHT + 2) {
			this->x = WIDTH/2;
			this->y = HEIGHT/2;
			setSpeed(rand_range(210, 330));
			lifes--;
		}
	}
		
};
