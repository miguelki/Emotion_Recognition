#include "StdAfx.h"
#include "KinectAccess.h"


KinectAccess::KinectAccess(DWORD f) : flags(f)
{
	window = new FrameWindow();
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
	HRESULT hr=S_OK; // The return codes used by COM interfaces
	const NUI_IMAGE_FRAME * pImageFrame = NULL; 

	//Création du manual reset event, afin d'attendre la disponibilité de la prochaine trame 
	_videoNextFrame=CreateEvent( NULL, TRUE, FALSE, NULL ); // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682396(v=vs.85).aspx

	// Parametres : type d'image - resolution - pre-processing de l’image - nb de frames a bufferiser - handle 1 - handle 2
	hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, _videoNextFrame, &_videoStreamHandle);

	// Recuperation de la trame suivante, parametres : handle - delai d'attente - pointeur sur la structure NUI_IMAGE_FRAME qui recevra les données
	HRESULT hr2 = NuiImageStreamGetNextFrame(_videoStreamHandle, 0, &pImageFrame );

	// ---------- Terminaison ----------
	NuiShutdown();
}

void KinectAccess::shutdown() {

}