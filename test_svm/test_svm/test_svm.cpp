// test_svm.cpp : définit le point d'entrée pour l'application console.

#include "stdafx.h"

#define MAX_SZ 2500
#define NB_FEAT 780

using namespace std;

struct svm_problem* read_file(string train_file);
struct svm_parameter* fill_params(struct svm_problem* prob);
double classify_accuracy(struct svm_model* model, struct svm_problem* test_d);

int main(int argc, char* argv[])
{
	int existing_model = 1; // 1 if model already exists, 0 otherwise

	string model_file = "model_mnist";
	string train_file = "svm/mnist.tr";
	string test_file = "svm/mnist.t";

	struct svm_model *model;
	cout << "reading train data" << endl;
	struct svm_problem *prob = read_file(train_file);
	cout << "reading test data" << endl;
	struct svm_problem *test_d = read_file(test_file);
	struct svm_parameter *param;

	if (!existing_model) {

		param = fill_params(prob);

		cout << "parameters fetched - training in progress" << endl;
		
		/** Train model **/
		model = svm_train(prob, param);

		cout << "training done";

		/** Save model into a file **/

		cout << "- saving model" << endl;
		if (svm_save_model(model_file.c_str(), model) == -1)
			exit(-1);

		cout << "model saved - classification" << endl;

	} else {
		/** Load model **/
		cout << "Loading model" << endl;
		model = svm_load_model(model_file.c_str());

		cout << "model loaded - classification" << endl;
	}

	/** Classify and test accuracy **/
	cout << "Accuracy : " <<  classify_accuracy(model, test_d) << endl;

	/** Release memory **/
	if (!existing_model) {
		free(prob);
		free(param);
		free(test_d);
	}
	free(model);

	system("pause");
	return 0;
}

struct svm_problem* read_file(string train_file) {
	struct svm_problem* prob = new struct svm_problem;
	ifstream infile;
	int nb_d = 0; // number of data points
	vector<double> labels; // labels
	vector<struct svm_node*> values;
	char *buf = new char[MAX_SZ];

	/** Open file **/
	infile.open (train_file, ifstream::in);

	while (infile.good()) {		
		struct svm_node* ln_vec;
		int nb, nb_elem = 0;
		size_t lbl_p, cur_p, nxt_p;

		infile.getline(buf, MAX_SZ);
		string tmp(buf);
		
		// First value is label
		lbl_p = tmp.find_first_of(" \t");
		labels.push_back(atof(tmp.substr(0, lbl_p).c_str()));
		cur_p = lbl_p;

		// read rest of line and see how many elements
		while (cur_p != string::npos) {
			size_t tmp_p = cur_p+1;		
			cur_p = tmp.find_first_of(" \t", tmp_p);
			nb_elem++;
		}

		// rewind, allocate memory, fill up svm_node*
		ln_vec = new struct svm_node[nb_elem];
		cur_p = lbl_p+1;
		nb = 0;

		while (nb < nb_elem) {
			size_t tmp_p;

			nxt_p = tmp.find_first_of(" \t", cur_p);
			string tmp_b = tmp.substr(cur_p, nxt_p-cur_p);

			tmp_p = tmp_b.find_first_of(':');
			ln_vec[nb].index = atoi(tmp_b.substr(0, tmp_p).c_str());
			ln_vec[nb].value = atof(tmp_b.substr(tmp_p+1).c_str());

			cur_p = nxt_p+1;
			nb++;
		}

		// don't forget to add a pair <-1, -1> to mark the end of the vector
		ln_vec[nb_elem-1].index = -1;
		ln_vec[nb_elem-1].value = -1;
	
		nb_d++; // new node :) 

		// push it into values vector
		values.push_back(ln_vec);
	}

	/** Close file **/
	infile.close();

	/** Copy and assign structures to svm_problem so it won't be wiped out at the end of the function **/
	prob->l = nb_d;		
	prob->y = new double[nb_d];
	prob->x = new struct svm_node*[nb_d];

	for (int i = 0; i < nb_d; i++) {
		*(prob->y+i) = labels[i];
		*(prob->x+i) = values[i];
	}

	return prob;
}

struct svm_parameter* fill_params(struct svm_problem* prob) {
	struct svm_parameter* params = new struct svm_parameter;

			/** Fill up parameter structure : each value is taken from default values in the Matlab implementation**/
		params->svm_type = C_SVC;
		params->kernel_type = LINEAR;
		params->gamma = 1/NB_FEAT;
		params->degree = 3;
		params->coef0 = 0;
		params->nr_weight = 0;
		params->eps = 0.001;
		params->cache_size = 100;
		params->C = 1;
		params->shrinking = 1;
		params->probability = 0;

		/** Check parameters **/

		cout << "check parameters : ";
		char* str = _strdup(svm_check_parameter(prob, params));

		if (str != NULL)
			cout << str << endl;
		else
			cout << "no problem !" << endl;

		free(str);

	return params;
}

double classify_accuracy(struct svm_model* model, struct svm_problem* test_d) {
	double accuracy = 0;
	int nb_elemts = test_d->l;

	for (int i = 0; i < nb_elemts; i++) {
		double predict = svm_predict(model, *(test_d->x+i));
		if (predict == *(test_d->y+i))
			accuracy++;
	}

	return accuracy/nb_elemts;
	/*cout << "# of elements : " << nb_elemts << " - accuracy : " << accuracy << endl;
	 return accuracy;*/
}
