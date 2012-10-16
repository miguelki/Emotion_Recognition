#pragma once

/** Modèle : récupération du flux d'images depuis la Kinect **/

class KinectAccess
{
private:
	BYTE* pBuffer;
	DWORD flags;
	HANDLE _videoStreamHandle; // reception du flux de donnees
	HANDLE _videoNextFrame; // signale la disponibilite de la frame suivante
	INuiFrameTexture* pTexture;

public:
	KinectAccess(DWORD f);
	~KinectAccess(void);

	HRESULT fetchImg();

	void init();
	void shutdown();

	/** Getters & setters **/

	HANDLE getStreamHandle() {return _videoStreamHandle;}
	HANDLE getNextFrame() {return _videoNextFrame;}
	BYTE* getBuffer() {return pBuffer;}
};

