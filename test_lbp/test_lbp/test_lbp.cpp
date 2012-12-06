// test_lbp.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

/** Lors du passage en MVC, il serait peut-être plus judicieux de transformer ces définitions en membres d'une classe **/

#define NB_ROWS 7
#define NB_COLS 6

using namespace cv;
using namespace std;

Mat lbpProcess(Mat src); // 3x3 LBP normalization
vector<unsigned int> histogramProcess(Mat src); // compute histogram out of normalized image

int main (int argc, char* argv[]) {
	DWORD start, end;
	Mat i_mat, o_mat;
	vector<unsigned int> hist;
	string i_file = "visage.png"; // input image
	string o_file = "output_" + i_file;

	/** load input image, greyscale mode **/
	i_mat = imread(i_file, 0);
	o_mat.create(i_mat.rows, i_mat.cols, CV_8UC1);

	if (i_mat.data == NULL) {
		cout << "Could not load image ! "  << endl;
		return -1;
	} else {
		start = GetTickCount();
		/** Divide image in areas **/
		int img_w = i_mat.cols;
		int img_h = i_mat.rows;

		int wdh = (int) floor((double)(img_w / NB_COLS));
		int hgt = (int) floor((double)(img_h / NB_ROWS));

		/** prepare vector **/
		hist.clear();

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

				Mat res = lbpProcess(i_mat(roi));

				/** Compute histogram **/
				vector<unsigned int> tmp = histogramProcess(res);

				for (unsigned int k = 0; k < tmp.size(); k++) {
					hist.push_back(tmp[i]);
				}

				/** Copy new area into output matrix **/
				Mat tmp_m = o_mat(roi);
				res.copyTo(tmp_m);
			}
		}
		end = GetTickCount();
		cout << "taille de l'histogramme : " << "1x" << hist.size() << endl;
		cout << "temps necessaire au traitement de l'image : " << end-start << "msec" << endl;
		//o_mat = lbpProcess(i_mat);

		/** Save output image and histogram **/
		//	imwrite(o_file, o_mat);

		/** Display both images **/
		//namedWindow( "Input image", CV_WINDOW_AUTOSIZE );
		//	namedWindow( "Output image", CV_WINDOW_AUTOSIZE );
		//imshow("Input image", i_mat);
		//	imshow("Output image", o_mat);
		//waitKey(0);
		//cvDestroyAllWindows();
	}

	
	return 0;
}


Mat lbpProcess(Mat src) {

	Mat dst = Mat::zeros(src.rows-2, src.cols-2, CV_8UC1);

	/*for(int i=1;i<src.rows-1;i++) {
	for(int j=1;j<src.cols-1;j++) {
	unsigned char center = src.at<unsigned char>(i,j);
	unsigned char code = 0;
	code |= (src.at<unsigned char>(i-1,j-1) > center) << 7;
	code |= (src.at<unsigned char>(i-1,j) > center) << 6;
	code |= (src.at<unsigned char>(i-1,j+1) > center) << 5;
	code |= (src.at<unsigned char>(i,j+1) > center) << 4;
	code |= (src.at<unsigned char>(i+1,j+1) > center) << 3;
	code |= (src.at<unsigned char>(i+1,j) > center) << 2;
	code |= (src.at<unsigned char>(i+1,j-1) > center) << 1;
	code |= (src.at<unsigned char>(i,j-1) > center) << 0;

	dst.at<unsigned char>(i-1,j-1) = code;
	}
	}*/

	for(int i=1;i<src.rows-1;i++) {
		for(int j=1;j<src.cols-1;j++) {
			// determine neighbourhood values
			unsigned char value = 0;
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

			unsigned int nb_transitions = 0;

			// affichage debug
		/*	cout << "neighbourhood : ";
			for (unsigned int v = 0; v < nghbrs.size(); v++)
				cout << nghbrs[v];
*/
			for (unsigned int k = 1; k < nghbrs.size(); k++) {
				value += nghbrs[k];		

				if (nghbrs[k] != nghbrs[k-1])
					nb_transitions++;

				if (nb_transitions > 2) {
					value = 9;
					break;
				}
			}

			if ((nb_transitions < 2 && (nghbrs[0] != nghbrs[nghbrs.size()-1])) || (nb_transitions <= 2 && (nghbrs[0] == nghbrs[nghbrs.size()-1]))){
				value += nghbrs[0];
			}

			//cout << " value : " << (int)value << endl;
			// assign value
			dst.at<unsigned char>(i-1,j-1) = value;
		}
	}

	//cout << "------" << endl;
	return dst;
}

/** TODO : Improve it elegantly with a recursive algorithm **/

vector<unsigned int> histogramProcess(Mat src) {
	vector<unsigned int> hist (10,0);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int n = (int) src.at<unsigned char>(i, j);
			hist[n] += 1;
		}
	}

	return hist;
}
