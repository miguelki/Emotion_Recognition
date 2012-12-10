#pragma once
#include "stdafx.h"
#include "svm\svm.h"

#define NB_COLS 6
#define NB_ROWS 7
#define NB_CLASS 7

#define NB_FEAT 420

using namespace std;
using namespace cv;

class ImgProcessing
{
private:
	static svm_model* model;
	static string classLbls[NB_CLASS]; 

	// Haar face classifier
CvMemStorage* storage;
CvHaarClassifierCascade* cascade;
const char *cascade_name;

public:
	ImgProcessing();
	~ImgProcessing();
	void init(string path); // loads the model

	/** Image Processing **/	
	string processImg(IplImage* src); // wrapper function

	/** LBP Processing **/	
	Mat findFace(IplImage* img); // viola-jones algorithm for face detection
	Mat processLbp(Mat src); // output an lbp thresholded image
	std::vector<unsigned int> histogramProcess(Mat src); // compute histogram out of lbp processed image
	std::vector<double> scaleHist(std::vector<unsigned int> histogram); // scale computed histogram
	svm_node* dataProcess(std::vector<double> histogram); // compute data line out of image histogram
	svm_node* imgProcess(IplImage* src); // wrapper function for lbp processing

	/** SVM classification **/
	string classifyImg(svm_node* vector);

};

