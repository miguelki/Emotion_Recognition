// classification.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "imgProcessing.h"

using namespace std;

int main()
{
	cout << "Ho ho ho " << endl;
	ImgProcessing processImg;

	/** Load image **/
	IplImage *img = cvLoadImage("test_face.jpg");
        if (!img) {
                printf("Error: Couldn't open the image file.\n");
                return 1;
        }

	/** Init **/
	processImg.init("model_kdef");

	/** Perform LBP + SVM **/
	cout << "emotion: " << processImg.processImg(img) << endl;

	system("pause");
	return 0;
}

