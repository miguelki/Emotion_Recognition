#include "StdAfx.h"
#include "FrameWindow.h"
#include <Strsafe.h>

using namespace std;

FrameWindow::FrameWindow(void) : pD2DFactory(nullptr), pD2DTarget(nullptr),  pD2D1Frame(nullptr), pBlackBrush(nullptr), hwnd(0) {
}

FrameWindow::~FrameWindow(void) {
	SafeRelease(&pD2DFactory);
	SafeRelease(&pD2DTarget);  
	SafeRelease(&pD2D1Frame); 
	SafeRelease(&pBlackBrush);
}


HRESULT FrameWindow::createWdw() {
	// Create window
	HRESULT hr = S_OK;
	FLOAT dpiX, dpiY;

	pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	// Register the window class.
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = FrameWindow::WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = sizeof(LONG_PTR);
	wcex.hInstance     = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName  = NULL;
	wcex.hCursor       = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"FrameWindow";

	RegisterClassEx(&wcex);

	hwnd = CreateWindow(L"FrameWindow", 
		L"Display window", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		static_cast<UINT>(ceil(W_WDH.f * dpiX / 96.f)),	
		static_cast<UINT>(ceil(W_HGT.f * dpiY / 96.f)), 
		NULL, 
		NULL, 
		HINST_THISCOMPONENT, 
		this);

	if (hwnd == NULL)
		hr = E_FAIL;

	return hr;
}

HRESULT FrameWindow::createFactory() {
	HRESULT hr = S_OK;

	// Initialize factory
	if (pD2DFactory == nullptr) {

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&pD2DFactory);
	}

	return hr;
}

HRESULT FrameWindow::createRenderTarget(HWND hw) {
	HRESULT hr = S_OK;

	if(pD2DFactory == nullptr)
		return E_FAIL;

	// Create a Direct2D render target			
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties( hw,D2D1::SizeU(W_WDH, W_HGT)),&pD2DTarget);

	// Create a background brush
	pD2DTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::BlueViolet),&pBlackBrush);

	return hr;
}

HRESULT FrameWindow::init(HWND hw) {
	HRESULT hr;

	// Creer factory
	hr=createFactory();

	if (SUCCEEDED(hr))
	{
		// Si ca a fonctionne, creer fenetre & render target
		hr = createWdw();

		if (SUCCEEDED(hr))
			hr = createRenderTarget(hwnd);
	}

	return hr;
}

/** Le code vient d'ici : http://blogs.msdn.com/b/devosaure/archive/2011/07/11/comment-d-233-velopper-avec-le-sdk-kinect-en-c-part-i.aspx **/
HRESULT FrameWindow::drawWindow() {
	HRESULT hr=S_OK;
	pD2DTarget->BeginDraw();

	// dessiner arriere plan
	RECT rc;
	GetClientRect(pD2DTarget->GetHwnd(),&rc);
	D2D1_RECT_F rect;			
	rect.top =(FLOAT)rc.top ;

	rect.bottom =(FLOAT)rc.bottom ;

	rect.right =(FLOAT)rc.right; 			
	rect.left =(FLOAT)rc.left;

	pD2DTarget->FillRectangle  (&rect,pBlackBrush);
	
	pD2DTarget->EndDraw();

	return hr;
}

/** Le code vient d'ici : http://blogs.msdn.com/b/devosaure/archive/2011/07/11/comment-d-233-velopper-avec-le-sdk-kinect-en-c-part-i.aspx **/
HRESULT FrameWindow::drawFrame(BYTE* frame, UINT32 pitch,UINT32 width,UINT32 height) {
	HRESULT hr = S_OK;

	HRESULT hr=S_OK;
	D2D1_SIZE_U size;
	size.height =height;
	size.width =width;
	D2D1_BITMAP_PROPERTIES properties=D2D1::BitmapProperties ();


	properties.pixelFormat.format =DXGI_FORMAT_B8G8R8A8_UNORM     ;
	properties.pixelFormat.alphaMode =D2D1_ALPHA_MODE_PREMULTIPLIED; 

	hr=pD2DTarget->CreateBitmap(size,
		frame,
		pitch,
		properties,&pD2D1Frame);
	pD2D1Frame->AddRef ();

	if (SUCCEEDED(hr))
	{		

		RECT rc;
		GetClientRect(pD2DTarget->GetHwnd(),&rc);
		D2D1_RECT_F rect;			
		rect.top =(FLOAT)rc.top ;			
		rect.bottom =(FLOAT)rc.bottom ;			
		rect.right =(FLOAT)rc.right; 			
		rect.left =(FLOAT)rc.left;									
		pD2DTarget->DrawBitmap (pD2D1Frame,&rect);					
	}	

	SafeRelease (&pD2D1Frame);

	return hr;

}
