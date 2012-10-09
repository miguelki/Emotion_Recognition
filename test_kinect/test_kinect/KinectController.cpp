#include "StdAfx.h"
#include "KinectController.h"


KinectController::KinectController(DWORD f) {
	model = new KinectAccess(f);
	window = new FrameWindow();
}


KinectController::KinectController(KinectAccess* m, FrameWindow* w) : model(m), window(w) {
}

KinectController::~KinectController(void){

}
