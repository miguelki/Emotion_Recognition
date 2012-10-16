#include "StdAfx.h"
#include "KinectAccess.h"
#include <comdef.h>

KinectAccess::KinectAccess(DWORD f) : flags(f), pBuffer(NULL), pTexture(NULL) {
}

KinectAccess::~KinectAccess(void)
{
}

void KinectAccess::init() {
	// ---------- Initialisation ----------
	NuiInitialize(flags);

	//Création du manual reset event, afin d'attendre la disponibilité de la prochaine trame 
	_videoNextFrame=CreateEvent( NULL, TRUE, FALSE, NULL ); // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682396(v=vs.85).aspx

	// Parametres : type d'image - resolution - pre-processing de l’image - nb de frames a bufferiser - handle 1 - handle 2
	NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, _videoNextFrame, &_videoStreamHandle);
}

HRESULT KinectAccess::fetchImg() {

	pBuffer = NULL;
	const NUI_IMAGE_FRAME* pImageFrame = NULL;

	WaitForSingleObject (_videoNextFrame,INFINITE); 

	// Recuperation de la trame suivante, parametres : handle - delai d'attente - pointeur sur la structure NUI_IMAGE_FRAME qui recevra les données
	HRESULT hr = NuiImageStreamGetNextFrame(_videoStreamHandle, 0, &pImageFrame );

	if (hr == E_POINTER)
		cout << "pointer problem" << endl;
	if (hr == E_NUI_FRAME_NO_DATA)
		cout << "nui frame no data" << endl;
	if (hr == E_INVALIDARG)
		cout << "invalid argument" << endl;

	// Trame -> Texture -> Structure de données -> données & longueur de la ligne
	if(SUCCEEDED(hr)) 
	{ 
		pTexture = pImageFrame->pFrameTexture; 
		NUI_LOCKED_RECT LockedRect; 
		pTexture->LockRect( 0, &LockedRect, NULL, 0 ); 

		if( LockedRect.Pitch != 0 ) { 
			pBuffer = (BYTE*) LockedRect.pBits; 
			
			HRESULT hr=pTexture->UnlockRect(0); 
		} 
		else { 
			OutputDebugString( L"Longueur du buffer erronée\r\n" ); 
			hr = E_FAIL;
		} 

		hr=NuiImageStreamReleaseFrame(_videoStreamHandle, pImageFrame); 

	} 

	return hr;
}

void KinectAccess::shutdown() {
	// ---------- Terminaison ----------
	NuiShutdown();
}