#include "StdAfx.h"
#include "imgProcessing.h"

string ImgProcessing::classLbls[NB_CLASS] = {"neutral", "afraid", "angry", "disgusted", "happy", "sad", "surprised"};
svm_model* ImgProcessing::model = 0;

ImgProcessing::ImgProcessing() : cascade_name("haarcascade_frontalface.xml") {
	// Load classifier for face detection
	cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0 );

	// Allocate the memory storage
	storage = cvCreateMemStorage(0);
}


ImgProcessing::~ImgProcessing() {
	/** Free model **/
	if (model != 0)
		svm_free_and_destroy_model(&model);

	/** Free face detection classifier & storage **/
	cvReleaseMemStorage( &storage );
	cvReleaseHaarClassifierCascade( &cascade );
}

void ImgProcessing::init(string path) {
	if (model == 0) {
		cout << "load model" << endl;
		model = svm_load_model(path.c_str());
	} else {
		cout << "Error while loading model, abort process" << endl;
		exit(-1);
	}
}

string ImgProcessing::processImg(IplImage* src){
	svm_node* node = imgProcess(src);
	string lbl = classifyImg(node);
	delete node;

	return lbl;
}

svm_node* ImgProcessing::imgProcess(IplImage* src){

	std::vector<unsigned int> hist; 
    Mat face = findFace(src); // If we need to find the face
    //	Mat face(src); // Otherwise let's just assign the IplImg* to a matrix

	/** prepare vector **/
	hist.clear();

	/** Divide image in areas **/
	int img_w = face.cols;
	int img_h = face.rows;

	int wdh = (int) floor((double)(img_w / NB_COLS));
	int hgt = (int) floor((double)(img_h / NB_ROWS));

	for (int j = 0; j < NB_ROWS; j++) {
		for (int i = 0; i < NB_COLS; i++) {

			int start_w = i * wdh - 1;
			int end_w = (i+1) * wdh + 1;
			int start_h = j * hgt - 1;
			int end_h = (j+1) * hgt + 1;

			if (j == 0)
				start_h = 0;

			if (j == NB_ROWS - 1)
				end_h = img_h;

			if (i == 0)
				start_w = 0;

			if (i == NB_COLS - 1)
				end_w = img_w;

			/** Process sub-area **/
			CvRect roi = cvRect(start_w, start_h, (end_w - start_w), (end_h - start_h));
			Mat dst = processLbp(face(roi));

			/** Compute histogram **/
			std::vector<unsigned int> tmp = histogramProcess(dst);

			for (unsigned int k = 0; k < tmp.size(); k++) {
				hist.push_back(tmp[k]);
			}
		}
	}	

	std::vector<double> scHist = scaleHist(hist);

	return dataProcess(scHist);
}

Mat ImgProcessing::findFace(IplImage* img){

	cvClearMemStorage(storage);

	// There can be more than one face in an image. So create a growable sequence of faces.
	// Detect the objects and store them in the sequence
	CvSeq* faces = cvHaarDetectObjects(img, cascade, storage,
		1.1, 3, CV_HAAR_DO_CANNY_PRUNING,
		cvSize(160, 160), cvSize(160,160) );
	

	// Loop the number of faces found.
	if (faces->total == 0) {
		cout << "Error : no face detected !" << endl;
		exit(-3);
	} 

	/* get the first detected face */	
	CvRect *face = (CvRect*)cvGetSeqElem(faces, 0);
	cvSetImageROI(img, *face);
	Mat tmp(img);

	return tmp;
}

Mat ImgProcessing::processLbp(Mat src){
	Mat dst = Mat::zeros(src.rows-2, src.cols-2, CV_8UC1);

	for(int i=1;i<src.rows-1;i++) {
		for(int j=1;j<src.cols-1;j++) {

			// determine neighbourhood values
			unsigned char value = 0;
			unsigned int nb_transitions = 0;
			unsigned char center = src.at<unsigned char>(i,j);
			vector<unsigned int> nghbrs;

			src.at<unsigned char>(i-1,j-1) > center ? nghbrs.push_back(1) : nghbrs.push_back(0);
			src.at<unsigned char>(i-1,j) > center ? nghbrs.push_back(1) : nghbrs.push_back(0);
			src.at<unsigned char>(i-1,j+1) > center ? nghbrs.push_back(1) : nghbrs.push_back(0);
			src.at<unsigned char>(i,j+1) > center ? nghbrs.push_back(1) : nghbrs.push_back(0);
			src.at<unsigned char>(i+1,j+1) > center ? nghbrs.push_back(1) : nghbrs.push_back(0);
			src.at<unsigned char>(i+1,j) > center ? nghbrs.push_back(1) : nghbrs.push_back(0);
			src.at<unsigned char>(i+1,j-1) > center ? nghbrs.push_back(1) : nghbrs.push_back(0);
			src.at<unsigned char>(i,j-1) > center ? nghbrs.push_back(1) : nghbrs.push_back(0);

			// compute uniform pattern
			for (unsigned int k = 1; k < nghbrs.size(); k++) {
				value += nghbrs[k];		

				if (nghbrs[k] != nghbrs[k-1]) {
					nb_transitions++;					
				}
				if (nb_transitions > 2) {
					value = 9;
					break;
				}
			}

			if ((nb_transitions < 2 && (nghbrs[0] != nghbrs[nghbrs.size()-1])) || (nb_transitions <= 2 && (nghbrs[0] == nghbrs[nghbrs.size()-1])))
				value += nghbrs[0];

			// assign value
			dst.at<unsigned char>(i-1,j-1) = value;
		}
	}

	return dst;
}

std::vector<unsigned int> ImgProcessing::histogramProcess(Mat src){

	std::vector<unsigned int> hist (10,0);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int n = (int) src.at<unsigned char>(i, j);
			hist[n] += 1;
		}
	}

	return hist;
}

std::vector<double> ImgProcessing::scaleHist(std::vector<unsigned int> histogram) {
	unsigned int sz = histogram.size();
	std::vector<double> scHist(sz, 0);

	double max, min, sc;
	max = 0;
	min = INT_MAX;

	// Go through histogram, find min and max

	for (unsigned int j = 0; j < sz; j++) {
		if (histogram[j] > max)
			max = (double)histogram[j];
		if (histogram[j] < min)
			min = (double)histogram[j];
	}

	sc = max - min;
	for (unsigned int i = 0; i < sz; i++)
		scHist[i] = -1 + 2 *((double)histogram[i] - min)/(sc);

	return scHist;
}

svm_node* ImgProcessing::dataProcess(std::vector<double> histogram){

	unsigned int sz = histogram.size();
	svm_node* node = new svm_node[sz+1];

	for (unsigned int i = 0; i < sz; i++) {
		node[i].index = i+1;
		node[i].value = histogram[i];
	}

	node[sz].index = -1;
	node[sz].value = -1;

	return node;
}

string ImgProcessing::classifyImg(svm_node* vector){

	double res = svm_predict(ImgProcessing::model, vector);
	string str;

	if (res >= 0 && res < NB_CLASS)
		str = classLbls[(unsigned int) res];
	else
		str = "No matching found, there's an error somewhere";
	return str;
}
