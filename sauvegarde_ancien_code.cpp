FrameWindow::FrameWindow(void) : pD2DFactory(nullptr), pBlackBrush(nullptr), pD2D1Frame(nullptr), hwnd(nullptr) {
	// Create window
	HRESULT hr = S_OK;
	FLOAT dpiX, dpiY;

	// Initialize factory
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&pD2DFactory);

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

	hwnd = CreateWindow(
		L"FrameWindow",
		L"Display window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
		static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		this
		);

	// Create a Direct2D render target			
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties( hwnd,D2D1::SizeU(800, 600)),&pD2DTarget
		);

	// Create a background brush
	pD2DTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::BlueViolet),&pBlackBrush); 

	hr = hwnd ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(hwnd, SW_SHOWNORMAL);
		UpdateWindow(hwnd);
		RunMessageLoop();
	}

}


FrameWindow::~FrameWindow(void)
{
	//TODO RELEASE EVERY MOTHERFLIPPIN THING !!!!!
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

	return hr;
}

/** Le code vient d'ici : http://blogs.msdn.com/b/devosaure/archive/2011/07/11/comment-d-233-velopper-avec-le-sdk-kinect-en-c-part-i.aspx **/
HRESULT FrameWindow::drawFrame(BYTE* frame, UINT32 pitch,UINT32 width,UINT32 height) {

	HRESULT hr=S_OK;
	D2D1_SIZE_U size;
	size.height =height;
	size.width =width;
	D2D1_BITMAP_PROPERTIES properties=D2D1::BitmapProperties ();

	properties.pixelFormat.format =DXGI_FORMAT_B8G8R8A8_UNORM;
	properties.pixelFormat.alphaMode =D2D1_ALPHA_MODE_PREMULTIPLIED; 

	hr=pD2DTarget->CreateBitmap(size,frame,	pitch, properties,&pD2D1Frame);
	pD2D1Frame->AddRef ();

	if (SUCCEEDED(hr))
	{		

		RECT rc;
		GetClientRect(pD2DTarget->GetHwnd (),&rc);
		D2D1_RECT_F rect;			
		rect.top =(FLOAT)rc.top ;			
		rect.bottom =(FLOAT)rc.bottom ;			
		rect.right =(FLOAT)rc.right; 			
		rect.left =(FLOAT)rc.left;									
		pD2DTarget->DrawBitmap (pD2D1Frame,&rect);					
	}

	SafeRelease(&pD2D1Frame);
	return hr;
}

//
// The window message handler.
//
LRESULT CALLBACK FrameWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	if (message == WM_CREATE) {
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		FrameWindow *pDemoApp = (FrameWindow *)pcs->lpCreateParams;

		::SetWindowLongPtrW(hwnd,GWLP_USERDATA,PtrToUlong(pDemoApp));

		result = 1;
	}
	else {
		FrameWindow *pApp = reinterpret_cast<FrameWindow *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(hwnd,GWLP_USERDATA)));

		bool wasHandled = false;

		if (pApp) {
			switch (message) {
			case WM_SIZE:
				{
					UINT width = LOWORD(lParam);
					UINT height = HIWORD(lParam);
					pApp->onResize(width, height);
				}
				wasHandled = true;
				result = 0;
				break;

			case WM_DISPLAYCHANGE:
				{
					InvalidateRect(hwnd, NULL, FALSE);
				}
				wasHandled = true;
				result = 0;
				break;

			case WM_PAINT:
				{
					pApp->onRender();

					ValidateRect(hwnd, NULL);
				}
				wasHandled = true;
				result = 0;
				break;

			case WM_DESTROY:
				{
					PostQuitMessage(0);
				}
				wasHandled = true;
				result = 1;
				break;
			}
		}

		if (!wasHandled) {
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

HRESULT FrameWindow::onRender() {
	HRESULT hr = S_OK;

	if (!pD2DTarget)
		cout << "Attention, contexte de rendu non defini" << endl;

	pD2DTarget->BeginDraw();
	pD2DTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	hr = pD2DTarget->EndDraw();

	return hr;
}

void FrameWindow::onResize(UINT width, UINT height)
{
	if (pD2DTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		pD2DTarget->Resize(D2D1::SizeU(width, height));
	}
}

void FrameWindow::RunMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
