#pragma once

#include <WinUser.h>
#define W_WDH 800
#define W_HGT 600


/** UI **/

class FrameWindow
{

private :
	HWND hwnd; // return value after window creation

	ID2D1Factory*pD2DFactory;
	ID2D1HwndRenderTarget* pD2DTarget;
	ID2D1Bitmap* pD2D1Frame;
	ID2D1SolidColorBrush* pBlackBrush;

	static LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
	);

public:
	FrameWindow(void);
	~FrameWindow(void);
	HRESULT createWdw();
	HRESULT createFactory();
	HRESULT createRenderTarget(HWND hw);
	HRESULT init(HWND hw);
	HRESULT drawWindow(); // draw window frame & background
	HRESULT drawFrame(BYTE* frame, UINT32 pitch,UINT32 width,UINT32 height); // draw input frames	

	/** Getters and setters **/

	HWND getHandle() {return hwnd;}
	void setHandle(HWND wh) {hwnd = wh;}
	ID2D1HwndRenderTarget* getRenderTarget() {return pD2DTarget;}
	
};
