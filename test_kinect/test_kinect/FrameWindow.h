#pragma once

#include <WinUser.h>

/** UI **/

class FrameWindow
{

private :
	HWND hwnd; // return value after window creation
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget *pD2DTarget;
	ID2D1SolidColorBrush* pBlackBrush;
	ID2D1Bitmap *pD2D1Frame;

	static LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
	);

public:
	FrameWindow(void);
	~FrameWindow(void);
	HRESULT drawWindow(); // draw window frame & background
	HRESULT drawFrame(BYTE* frame, UINT32 pitch,UINT32 width,UINT32 height); // draw input frames	
	void RunMessageLoop();

	/** Getters and setters **/

	HWND getHandle() {return hwnd;}
	void setHandle(HWND wh) {hwnd = wh;}

	/** Events handlers **/
	    HRESULT onRender();

    void onResize(UINT width,UINT height);
};
