 //
//
//				Class KinectAccess, model of the MVC pattern used
//				Manage Kienct Acess and process face detection
//
//
//
//
//
//
#include "StdAfx.h"
#include "KinectAccess.h"
#include <comdef.h>

// Constructor which init some of the elements from the class
KinectAccess::KinectAccess(DWORD f) : flags(f), pBuffer(NULL), pTexture(NULL) {
}

// Destructor - NOT USED -
KinectAccess::~KinectAccess(void)
{
}

void KinectAccess::init() {
	// Init Kinect
	NuiInitialize(flags);

	//Creation of a manual reset event in order to wait for the availability of the next frame
	_videoNextFrame=CreateEvent( NULL, TRUE, FALSE, NULL ); // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682396(v=vs.85).aspx

	// Parametres : type d'image - resolution - pre-processing de l’image - nb de frames a bufferiser - handle 1 - handle 2
	NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, _videoNextFrame, &_videoStreamHandle);
}

HRESULT KinectAccess::fetchImg() {

	pBuffer = NULL; //init buffer
	const NUI_IMAGE_FRAME* pImageFrame = NULL;

	// Wait for an element coming from the sensor in the _videoNextFrame handle during an "infinite" time
	WaitForSingleObject (_videoNextFrame,INFINITE); 

	// Get a frame from the video stream handle and stock it into pImageFrame which is a NUI_IMAGE_FRAME structure
	HRESULT hr = NuiImageStreamGetNextFrame(_videoStreamHandle, 0, &pImageFrame );

	// Tests on the result of the previous functions
	if (hr == E_POINTER)
		cout << "pointer problem" << endl;
	if (hr == E_NUI_FRAME_NO_DATA)
		cout << "nui frame no data" << endl;
	if (hr == E_INVALIDARG)
		cout << "invalid argument" << endl;

	// Trame -> Texture -> Structure de données -> données & longueur de la ligne

	if(SUCCEEDED(hr)) 
	{ 
		// Get texture from the NUI_IMAGE_FRAME
		pTexture = pImageFrame->pFrameTexture; 
		// Lock data to be acquiered
		NUI_LOCKED_RECT LockedRect; 
		pTexture->LockRect( 0, &LockedRect, NULL, 0 ); 

		// If there is data 
		if( LockedRect.Pitch != 0 ) { 
			// Stock data into the buffer of the current object
			pBuffer = (BYTE*) LockedRect.pBits; 
			
			// Then try to unlock frame
			HRESULT hr=pTexture->UnlockRect(0); 
		} 
		else { 
			// If there is no data show a message error
			OutputDebugString( L"wrong lenght of the buffer\r\n" ); 
			hr = E_FAIL;
		} 

		// Release frame
		hr=NuiImageStreamReleaseFrame(_videoStreamHandle, pImageFrame); 

	} 

	return hr;// Return an HRESULT of the action done 
}

void KinectAccess::shutdown() {
	// end of the communication with the Kinect
	NuiShutdown();
}