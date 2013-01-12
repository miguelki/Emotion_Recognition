// kdef_model.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#define NB_CLASS 7
#define NB_COLS 6
#define NB_ROWS 7

using namespace std;
using namespace cv;

const string testFolder = "CK/test_data";
const string trainFolder = "scaled/train_data";
const string sampleFolder = "sample";
const string classLbls[NB_CLASS] = {"neutral", "afraid", "angry", "disgusted", "happy", "sad", "surprised"};
const unsigned int imgWgt[NB_ROWS*NB_COLS] = {1,1,1,1,1,1, 1,2,4,4,2,1, 1,4,4,4,4,1, 1,4,4,4,4,1, 1,2,2,2,2,1, 1,2,4,4,2,1, 1,1,2,2,1,1};  

// Haar face classifier
CvMemStorage* storage = 0;
CvHaarClassifierCascade* cascade = 0;
const char *cascade_name = "haarcascade_frontalface.xml";

std::vector<string> listDir(string root, int depth); // recursively go through directory
std::string imgProcess(IplImage* src, unsigned int label); // wrapper function for lbp processing
Mat findFace(IplImage* img); // viola-jones algorithm for face detection
Mat processLbp(Mat src); // output an lbp thresholded image
std::vector<unsigned int> histogramProcess(Mat src); // compute histogram out of lbp processed image
std::vector<double> scaleHist(std::vector<unsigned int> hist); // scale histogram values so they fit into interval [-1; 1]
std::string dataProcess(std::vector<double> histogram, unsigned int label); // compute data line out of image histogram
std::string dataProcess(std::vector<unsigned int> histogram, unsigned int label);
unsigned int fetchLabel(std::string root); // fetch class label from path

int main(int argc, char* argv[]) {

	std::vector<string> dataT, dataTr;
	string trainFile = "kdef_wgt_sc.tr";
	string testFile = "ck_sc.t";

	/** Load classifier for face detection **/
	cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0 );

	// Allocate the memory storage
	storage = cvCreateMemStorage(0);

	// Check whether the cascade has loaded successfully. Otherwise report error and quit
	if( !cascade ) {
		cout <<  "ERROR: Could not load classifier\n" << endl;
		return -1;
	}

	/** Compute data **/
	/*cout << "Processing train folder" << endl;
	dataTr = listDir(trainFolder, 0);
	cout << "Train folder done" << endl; 
	*/cout << "Processing test folder" << endl;
	dataT = listDir(testFolder, 0);
	cout << "Test folder done" << endl;

	/** Create a file and write data in it **/
	//ofstream outfileTr(trainFile, ios_base::trunc);
	ofstream outfileT(testFile, ios_base::trunc);

	/*for (unsigned int i = 0; i < dataTr.size(); i++) {
		outfileTr << dataTr[i] << flush;
	}
	outfileTr.close();*/

	for (unsigned int j = 0; j < dataT.size(); j++) {
		outfileT << dataT[j] << flush;
	}
	outfileT.close();

	system("pause");
	cvReleaseMemStorage( &storage );
	cvReleaseHaarClassifierCascade( &cascade );
	cvDestroyAllWindows();
	return 0;
}

std::vector<string> listDir(string root, int depth) {

	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	string dirName = root + "\\*";

	std::vector<string> res;

	szDir[dirName.size()]=0;

	// As much as we'd love to, we can't use memcpy() because
	// sizeof(TCHAR)==sizeof(char) may not be true:
	std::copy(dirName.begin(),dirName.end(),szDir);

	// Find first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) {
		cout << "Error FindFirstFile 1" << endl;
		return res;
	} 

	do {
		std::wstring fName = ffd.cFileName;
		string file(fName.begin(), fName.end());
		file.assign(fName.begin(), fName.end());

		for (int i = 0; i < depth; i++)
			cout << "  ";

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {								
			cout << file << endl;	
			// there are relative paths in each directory: do not compute them
			if (file != "." && file != "..") {
				string newRoot = root + "/" + file ;
				std::vector<string> tmp = listDir(newRoot, depth+1);

				for (unsigned int i = 0; i < tmp.size(); i++)
					res.push_back(tmp[i]);
			}			          
		} else {
			DWORD start, end;
			start = GetTickCount();
			cout << "+ " << file << endl;

			unsigned int label = fetchLabel(root);

			if (label == -1)
				exit(-2);				
			else {
				string path = root + "/" + file;
				IplImage* img = cvLoadImage(path.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

				if (img == NULL) {
					cout << "Could not load image " << path << endl;
					exit(-1);					
				} else {
					string tmp = imgProcess(img, label);
					res.push_back(tmp);
					end = GetTickCount();
					cout << "Processing time : " << end-start << " msec" << endl;
				}
			}
		}
	}

	while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		cout << "Error FindFirstFile 2" << endl;
	}

	FindClose(hFind);

	return res;
}

std::string imgProcess(IplImage* src, unsigned int label) {
	vector<double> scHist;
	vector<unsigned int> hist; 
	Mat face = findFace(src);
	unsigned int idx = 0;

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
			vector<unsigned int> tmp = histogramProcess(dst);

			/** Weight histogram **/
				unsigned int wgt = imgWgt[idx];
				//unsigned int wgt = 1;
				for (unsigned int k = 0; k < tmp.size(); k++) {										
					hist.push_back(wgt*tmp[k]);
				}
				idx++;
		}
	}
	scHist = scaleHist(hist);

	return dataProcess(scHist, label);
}

Mat findFace(IplImage* img) {

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

	imshow("Output image", tmp);
	cvWaitKey(1);

	return tmp;
}

Mat processLbp(Mat src) {
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

std::vector<unsigned int> histogramProcess(Mat src) {
	vector<unsigned int> hist (10,0);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int n = (int) src.at<unsigned char>(i, j);
			hist[n] += 1;
		}
	}

	return hist;
}

std::vector<double> scaleHist(std::vector<unsigned int> hist) {
	unsigned int sz = hist.size();
	std::vector<double> scHist(sz, 0);

	double max, min, sc;
	max = 0;
	min = INT_MAX;

	// Go through histogram, find min and max

	for (unsigned int j = 0; j < sz; j++) {
		if (hist[j] > max)
			max = (double)hist[j];
		if (hist[j] < min)
			min = (double)hist[j];
	}

	sc = max - min;
	for (unsigned int i = 0; i < sz; i++)
		scHist[i] = -1 + 2 *((double)hist[i] - min)/(sc);

	return scHist;
}

std::string dataProcess(std::vector<double> histogram, unsigned int label) {
	std::string data;
	std::stringstream str_s;
	str_s << label;
	str_s >> data;

	for (unsigned int i = 0; i < histogram.size() ; i++) {
		std::string value;

		if (histogram[i] != 0) {
			std::stringstream str_v;
			/*double scale = 0.00001;
			double v = (int)(histogram[i]/scale)*scale;
			*/
			str_v << " " << i+1 << ":" << histogram[i];
			value = str_v.str();	
		}
		data.append(value);
	}	
	data.append("\n");

	return data;
}

std::string dataProcess(std::vector<unsigned int> histogram, unsigned int label) {
	std::string data;
	std::stringstream str_s;
	str_s << label;
	str_s >> data;

	for (unsigned int i = 0; i < histogram.size() ; i++) {
		std::string value;

		if (histogram[i] != 0) {
			std::stringstream str_v;

			str_v << " " << i+1 << ":" << histogram[i];
			value = str_v.str();	
		}
		data.append(value);
	}	
	data.append("\n");

	return data;
}

unsigned int fetchLabel(std::string root) {
	unsigned int lbl = -1;

	size_t c = root.find_last_of("/");
	string cl = root.substr(++c, string::npos);
	for (int i = 0; i < NB_CLASS; i++) {
		if (!cl.compare(classLbls[i])) {
			lbl = i;
			break;
		}
	}	

	return lbl;
}