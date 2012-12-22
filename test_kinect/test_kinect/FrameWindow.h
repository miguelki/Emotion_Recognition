#pragma once



class FrameWindow
{
	IplImage* img; // image to display

public:

	// Constructor and destructor
	FrameWindow(void);
	~FrameWindow(void);

	void displayImg(IplImage* i, char* title);
	void destroyWindow(char* title);
	void createWindow(char* title);
	

};

