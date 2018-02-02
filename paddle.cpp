class Paddle
{
public:
	int len = 5;
	int start;
	int end;

	Paddle (int st)
	{
		this->start = st;
		this->end = this->start + this->len;
		
	}

	~Paddle ()
	{

	}

	/* Moves paddle based on char t */
	void move (char t)
	{
		if ((t == 'A' || t == 'a') && this->start > 0)
			this->start--;
		else if ((t == 'D' || t == 'd') && this->end < WIDTH - 1)
			this->start++;
		
		this->end = this->start + this->len;
	}
};
