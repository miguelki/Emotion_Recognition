#include "StdAfx.h"
#include "KinectAccess.h"

using namespace std;

KinectAccess::KinectAccess(DWORD f) : flags(f) {
}

KinectAccess::~KinectAccess(void)
{
}

void KinectAccess::init() {
	// ---------- Initialisation ----------
	NuiInitialize(flags);

	// ---------- Play time ---------- 

	HANDLE _videoStreamHandle; // reception du flux de donnees
	HANDLE _videoNextFrame; // signale la disponibilite de la frame suivante
	HRESULT hr_=S_OK; // The return codes used by COM interfaces
	const NUI_IMAGE_FRAME * pImageFrame = NULL; 

	//Création du manual reset event, afin d'attendre la disponibilité de la prochaine trame 
	_videoNextFrame=CreateEvent( NULL, TRUE, FALSE, NULL ); // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682396(v=vs.85).aspx
	
	// Parametres : type d'image - resolution - pre-processing de l’image - nb de frames a bufferiser - handle 1 - handle 2
	hr_ = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, _videoNextFrame, &_videoStreamHandle);

	while(true) 
	{ 
		/*if (Context::IsCurrentTaskCollectionCanceling ()) 
		{ 
			break; 
		} 
		_pD2D1Helper->D2D1GetContexteRendu()->BeginDraw (); 
		_pD2D1Helper->D2D1PeindreArrierePlan (); */
		const NUI_IMAGE_FRAME * pImageFrame = NULL; 

		WaitForSingleObject (_videoNextFrame,INFINITE); 
		// Recuperation de la trame suivante, parametres : handle - delai d'attente - pointeur sur la structure NUI_IMAGE_FRAME qui recevra les données
		HRESULT hr = NuiImageStreamGetNextFrame( _videoStreamHandle, 0, &pImageFrame );

		// Trame -> Texture -> Structure de données -> données & longueur de la ligne
		if(SUCCEEDED(hr)) 
		{ 
			INuiFrameTexture * pTexture = pImageFrame->pFrameTexture; 
			NUI_LOCKED_RECT LockedRect; 
			pTexture->LockRect( 0, &LockedRect, NULL, 0 ); 
			if( LockedRect.Pitch != 0 ) 
			{ 
				BYTE * pBuffer = (BYTE*) LockedRect.pBits; 
				//hr=_pD2D1Helper->D2D1DrawFrame (pBuffer,LockedRect.Pitch,640,480); 
				hr=pTexture->UnlockRect (0); 
			} 
			else 
			{ 
				OutputDebugString( L"Longueur du buffer erronée\r\n" ); 
			} 
			// liberation des ressources
			hr=NuiImageStreamReleaseFrame(_videoStreamHandle, pImageFrame ); 
		} 
		//_pD2D1Helper->D2D1GetContexteRendu()->EndDraw();
	}

}

void KinectAccess::shutdown() {
		// ---------- Terminaison ----------
	NuiShutdown();
}