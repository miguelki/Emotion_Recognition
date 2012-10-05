#pragma once

#include "FrameWindow.h"

/** Modèle : récupération du flux d'images depuis la Kinect **/

class KinectAccess
{
private:
	DWORD flags;
	FrameWindow* window;

public:
	KinectAccess(DWORD f);
	~KinectAccess(void);
	void init();
	void shutdown();

	/** Getters & setters **/

	HWND getWdwHandle() {return window->getHandle();}
	HWND setWdwHandle(HWND wh) {window->setHandle(wh);}
};

