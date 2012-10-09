#pragma once

/** Modèle : récupération du flux d'images depuis la Kinect **/

class KinectAccess
{
private:
	DWORD flags;

public:
	KinectAccess(DWORD f);
	~KinectAccess(void);
	void init();
	void shutdown();
};

