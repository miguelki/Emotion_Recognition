#pragma once

#include <WinUser.h>

/** UI **/

class FrameWindow
{
private :
	HWND windowHandle; // return value after window creation

public:
	FrameWindow(void);
	~FrameWindow(void);

	/** Getters and setters **/

	HWND getHandle() {return windowHandle;}
	void setHandle(HWND wh) {windowHandle = wh;}
};

