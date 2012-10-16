#pragma once

class FrameWindow
{
	IplImage* img;

public:
	FrameWindow(void);
	~FrameWindow(void);

	void displayImg(IplImage* i, char* title);
	void destroyWindow(char* title);
};

