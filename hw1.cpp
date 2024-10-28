#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include "measure_cpu_time.h"

using namespace std;

const char* FOLDER = "./Data/";
const char* CONFIG = "HW1_config.txt";

class case_image {
private:
	int num_algo;
	string input_pgm;
	string average_txt;
	string output_txt;
	int ** container;
	int size;
	int average;
	int MAX_VALUE;

public:
	case_image() {
		num_algo = 0;
		input_pgm = "";
		average_txt = "";
		output_txt = "";
		container = nullptr;
		size = 0;
		average = 0;
		MAX_VALUE = 0;
	}

	~case_image() {
		for (int i = 0; i < size; i++) {
			delete[] container[i];
		}
		delete[] container;
	}

	int update(int num_algo, string input_pgm, string average_txt, string output_txt) {
		this->num_algo = num_algo;
		this->input_pgm = input_pgm;
		this->average_txt = average_txt;
		this->output_txt = output_txt;

		size_t underscore1 = input_pgm.find('_');

		if (underscore1 != string::npos) {
			this->size = stoi(input_pgm.substr(underscore1 + 1));
		}
		else {
			cout << "Error: Invalid input file name. " << input_pgm << endl;
			return 0;
		}

		if (container) {
			for (int i = 0; i < size; i++) {
				delete[] container[i];
			}
			delete[] container;
		}

		if (size > 0) {
			this->container = new int* [size];

			for (int i = 0; i < size; i++) {
				container[i] = new int[size];
			}
		}
		else {
			cout << "Error: Invalid input file name." << input_pgm << endl;
			this->container = nullptr;
			return 0;
		}

		return 1;
	}

	int read_input() {
		if (input_pgm != "") {
			const string FILEPATH = FOLDER + input_pgm;
			ifstream fp(FILEPATH);

			if (!fp.is_open()) {
				cout << "Error: Can not open the file. " << input_pgm << endl;
				if (container) {
					for (int i = 0; i < size; i++) {
						for (int j = 0; j < size; j++) {
							container[i][j] = 0;
						}
					}
				}

				return 0;
			}

			else {
				string format;
				int width = 0, height = 0;

				fp >> format;
				fp >> width >> height;
				fp >> MAX_VALUE;

				if (format != "P2") {
					cout << "Error: The input file is not P2 format. " << input_pgm << endl;
					return 0;
				}

				if (width != size || height != size) {
					cout << "Error: the image size does not match with the input file name" << width << ' ' << height << ' ' << input_pgm << endl;
					return 0;
				}

				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						fp >> container[i][j];
					}
				}
			}
		}
		return 1;
	}

	int sub_average() {
		string FILEPATH = FOLDER + average_txt;
		ifstream fp(FILEPATH);
		
		if (fp) {
			fp >> average;

			if (container) {
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						container[i][j] -= average;
					}
				}
			}

			else {
				cout << "Error: No pixel container created." << endl;
				return 0;
			}
		}

		else {
			cout << "Error: Can not open file. " << average_txt << endl;
			return 0;
		}
		return 1;
	}

	void write(int s, int i, int j, int k, int l) {
		string FILEPATH = FOLDER + output_txt;
		ofstream fp(FILEPATH);

		fp << s << " " << i << " " << j << " " << k << " " << l;
		return;
	}

	int algo() {
		return num_algo;
	}

	int** pixels() {
		return container;
	}

	int length() {
		return size;
	}
};

class case_container {
private:
	int num_cases;
	case_image * cases;
	string config;

public:
	case_container() {
		config = CONFIG;
		string FILEPATH = FOLDER + config;
		ifstream fp(FILEPATH);

		if (!fp.is_open()) {
			cout << "Error: Can not open the file. " << config << endl;
			num_cases = 0;
			cases = nullptr;
		}

		else {
			fp >> num_cases;

			cases = new case_image[num_cases];

			for (int i = 0; i < num_cases; i++) {
				int num_algo = 0;
				string input_pgm = "";
				string average_txt = "";
				string output_txt = "";

				fp >> num_algo;
				fp >> input_pgm;
				fp >> average_txt;
				fp >> output_txt;

				int FLAG = cases[i].update(num_algo, input_pgm, average_txt, output_txt);
				if (FLAG) {
					cout << "Successfully created case " << i+1 << '.' << endl;
				}
				else {
					cout << "Error: Failed to create case " << i+1 << '.' << endl;
				}
			}
		}
	}

	~case_container() {
		delete[] cases;
	}

	int length() {
		return num_cases;
	}

	case_image* case_ptr() {
		return cases;
	}
};

// Summed Area-Table Algorithm
void algo_3(int * s, int * i, int * j, int * k, int * l, int ** container, int size) {
	
	__int64 start, freq, end;
	float compute_time;

	CHECK_TIME_START(start, freq);

	int** SAT = new int* [size];
	for (int a = 0; a < size; a++) {
		SAT[a] = new int[size];
	}

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			SAT[x][y] = container[x][y];

			if (x > 0) {
				SAT[x][y] += SAT[x - 1][y];
			}

			if (y > 0) {
					SAT[x][y] += SAT[x][y - 1];
			}

			if (x > 0 && y > 0){
				SAT[x][y] -= SAT[x - 1][y - 1];
			}
		}
	}

	int max_sum = 0;

	for (int y1 = 0; y1 < size; y1++) {
		for (int x1 = 0; x1 < size; x1++) {
			for (int y2 = y1; y2 < size; y2++) {
				for (int x2 = x1; x2 < size; x2++) {
					int sub_sum = SAT[x2][y2];

					if (x1 > 0) {
						sub_sum -= SAT[x1 - 1][y2];
					}
					if (y1 > 0) {
						sub_sum -= SAT[x2][y1 - 1];
					}
					if (x1 > 0 && y1 > 0) {
						sub_sum += SAT[x1 - 1][y1 - 1];
					}

					if (sub_sum > max_sum || (sub_sum == max_sum && (y1 < *i || (y1 == *i && y2 < *j) || (y1 == *i && y2 == *j && x1 < *k) || (y1 == *i && y2 == *j && x1 == *k && x2 < *l)))) {
						max_sum = sub_sum;
						*s = max_sum;
						*i = y1;
						*j = y2;
						*k = x1;
						*l = x2;
					}
				}
			}
		}
	}

	for (int a = 0; a < size; a++) {
		delete[] SAT[a];
	}
	delete[] SAT;

	CHECK_TIME_END(start, end, freq, compute_time);
	printf("Algorithm 3 execution time: %.3f ms\n", compute_time);
	return;
}

struct algo_4_result {
	int max_sum = 0;
	int k = 0, l = 0;
};

algo_4_result algo_4_max_mid(int column[], int top, int mid, int bottom) {
	int top_sum = 0;
	int temp_k = mid;
	int temp_l = mid + 1;
	int sub_sum = 0;
	for (int k = mid; k >= top; k--) {
		sub_sum += column[k];
		if (sub_sum > top_sum) {
			top_sum = sub_sum;
			temp_k = k;
		}
	}

	int bot_sum = 0;
	sub_sum = 0;
	for (int l = mid + 1; l <= bottom; l++) {
		sub_sum += column[l];
		if (sub_sum > bot_sum) {
			bot_sum = sub_sum;
			temp_l = l;
		}
	}

	return { top_sum + bot_sum, temp_k, temp_l };
}

algo_4_result algo_4_max_lr(int column[], int top, int bottom) {
	if (top == bottom) {
		return { column[top], top, bottom };
	}

	int mid = (top + bottom) / 2;

	algo_4_result max = { 0 };

	algo_4_result top_max = algo_4_max_lr(column, top, mid);
	algo_4_result bot_max = algo_4_max_lr(column, mid + 1, bottom);
	algo_4_result mid_max = algo_4_max_mid(column, top, mid, bottom);

	// 값이 같을 때 mid_max의 k, l 인덱스가 bot_max의 k, l 인덱스보다 더 앞에 위치함
	if (mid_max.max_sum >= bot_max.max_sum) {
		max = mid_max;
	}

	else {
		max = bot_max;
	}

	// 값이 같다면 top_max의 k, l 인덱스가 더 앞에 위치함

	if (top_max.max_sum >= max.max_sum) {
		max = top_max;
	}

	return max;
}

// Divide-and-Conquer
void algo_4(int* s, int* i, int* j, int* k, int* l, int ** container, int size) {

	__int64 start, freq, end;
	float compute_time;

	CHECK_TIME_START(start, freq);

	int* column = new int[size]();

	algo_4_result max_sum = { 0 };

	for (int x = 0; x < size; x++) {
		memset(column, 0, size * sizeof(int));
		for (int y = x; y < size; y++) {
			for (int a = 0; a < size; a++) {
				column[a] += container[a][y];
			}

			algo_4_result sub_sum = algo_4_max_lr(column, 0, size);

			if (sub_sum.max_sum > max_sum.max_sum  || (sub_sum.max_sum == max_sum.max_sum && ((x < *i || (x == *i && y <*j) || (x == *i && y == *j && sub_sum.k < *k) || (x == *i && y == *j &&sub_sum.k == *k && sub_sum.l < *l))))) {
				max_sum = sub_sum;
				*s = max_sum.max_sum;
				*i = x;
				*j = y;
				*k = max_sum.k;
				*l = max_sum.l;
			}

		}
	}

	CHECK_TIME_END(start, end, freq, compute_time);
	printf("Algorithm 4 execution time: %.3f ms\n", compute_time);

	return;
}

// Dynamic Programming
void algo_5(int* s, int* i, int* j, int* k, int* l, int ** container, int size) {

	__int64 start, freq, end;
	float compute_time;

	CHECK_TIME_START(start, freq);

	int* column = new int[size]();

	int max_sum = 0;

	for (int x = 0; x < size; x++) {
		memset(column, 0, size * sizeof(int));
		for (int y = x; y < size; y++) {
			for (int a = 0; a < size; a++) {
				column[a] += container[a][y];
			}

			int sub_sum = 0;
			int temp_k = 0;

			for (int a = 0; a < size; a++) {
				sub_sum += column[a];

				if (sub_sum < 0) {
					sub_sum = 0;
					temp_k = a + 1;
				}

				else if (sub_sum > max_sum || (sub_sum == max_sum && (x < *i || (x == *i && y < *j) || (x == *i && y == *j && temp_k < *k) || (x == *i && y == *j && temp_k == *k && a < *l)))) {
					max_sum = sub_sum;
					*s = max_sum;
					*l = a;
					*i = x;
					*j = y;
					*k = temp_k;
				}
			}
		}
	}

	delete[] column;

	CHECK_TIME_END(start, end, freq, compute_time);
	printf("Algorithm 5 execution time: %.3f ms\n", compute_time);

	return;
}

int main(void) {
	
	case_container images;
	int num_cases = images.length();
	case_image* case_pointer = images.case_ptr();

	for (int c = 0; c < num_cases; c++ ) {
		case_image* current = case_pointer + c;
		int FLAG1 = current->read_input();

		if (!FLAG1) {
			cout << "Error: Failed to read input file. case " << c + 1 << '.' << endl;
			continue;
		}

		int FLAG2 = current->sub_average();

		if (!FLAG2) {
			cout << "Error: Failed to subtract average. case " << c+1 << '.' << endl;
			continue;
		}

		int s = 0, i = 0, j = 0, k = 0, l = 0;

		switch (current->algo()) {
		case 3:
			algo_3(&s, &i, &j, &k, &l, current->pixels(), current->length());
			break;
		case 4:
			algo_4(&s, &i, &j, &k, &l, current->pixels(), current->length());
			break;
		case 5:
			algo_5(&s, &i, &j, &k, &l, current->pixels(), current->length());
			break;
		default:
			cout << "Error: Invalid algorithm number. case " << c+1 << '.' << endl;
			continue;
		}

		current->write(s, i, j, k, l);
	}

	return 0;
}