#pragma once

/** Modèle : récupération du flux d'images depuis la Kinect **/

class KinectAccess
{
private:
	BYTE* pBuffer; // Buffer
	DWORD flags; // flag for kinect access
	HANDLE _videoStreamHandle; // data stream
	HANDLE _videoNextFrame; // notice if a frame is available
	INuiFrameTexture* pTexture;

public:

	// Constructor destructor
	KinectAccess(DWORD f);
	~KinectAccess(void);

	// Get Image
	HRESULT fetchImg();

	void init();
	void shutdown();

	// Getters and setters 

	HANDLE getStreamHandle() {return _videoStreamHandle;}
	HANDLE getNextFrame() {return _videoNextFrame;}
	BYTE* getBuffer() {return pBuffer;}
};

