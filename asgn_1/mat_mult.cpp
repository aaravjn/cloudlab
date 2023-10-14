/*
	This script will run in the Virtual Machine
	Each row of the resultant matrix will be computed in a seperate thread thus,
	greately reducing the execution time of the matrix.
	This also puts a system depencency that more the numbers of CPU cores, more will be the number of threads
	computed simultaneously.
*/


#include <vector>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono;

int SIZE;
vector<vector<int>> matrix1, matrix2, matrixResultant;

int currRow = 0; // Represents the current row of execution by any thread

void* compute(void* arg) {
	int i = currRow++;

	for(int j = 0;j < SIZE;j++) {
		for(int k = 0;k < SIZE;k++) {
			matrixResultant[i][j] = matrix1[i][k] + matrix2[k][j];
		}
	}

	return NULL;
}

void multiply(vector<vector<int>> matrix1, vector<vector<int>> matrix2) {
	pthread_t threads[SIZE];

	for(int i = 0;i < SIZE;i++) {
        pthread_create(&threads[i], NULL, compute, NULL);
	}

	for(int i = 0;i < SIZE;i++) {
		pthread_join(threads[i], NULL);
	}


	// Single threaded code for computing matrix multiplication
	/*
		for(int i = 0;i < SIZE;i++) {
			for(int j = 0;j < SIZE;j++) {
				for(int k = 0;k < SIZE;k++) {
					matrixResultant[i][j] = matrix1[i][k] + matrix2[k][j];
				}
			}
		}
	*/



}

vector<vector<int>> extract(int n, char* str) {
	int i = 0, j = 0;
	vector<vector<int>> matrix(n);
	
	int s = 0;
	while(str[i] != '$') {
		if(str[i] == '.') {
			matrix[j].push_back(s);
			s = 0;
		}
		else if(str[i] == ',') {
			matrix[j].push_back(s);
			j++;
			s = 0;
		}
		else {
			s *= 10;
			s += str[i] - '0';
		}
		i++;
	}
	matrix[j].push_back(s);

	return matrix;
}

int main(int argc, char** argv) {
	
	int n;
	sscanf(argv[1], "%d", &n);
	SIZE = n;
	matrixResultant.resize(n, vector<int> (n));

	char* mat1 = argv[2];
	char* mat2 = argv[3];

	auto start = high_resolution_clock::now();
	
	matrix1 = extract(n, mat1);
	matrix2 = extract(n, mat2);
	
	
	multiply(matrix1, matrix2);

	for(int i = 0;i < n;i++) {
		for(int j = 0;j < n;j++) {
			cout<<matrixResultant[i][j]<<' ';
		}
		cout<<endl;
	}

	auto end = high_resolution_clock::now();

	auto time_taken = duration_cast<microseconds>(end - start);
	cout<<time_taken.count()<<endl;
}

// ./a.out 3 1.1.2,1.1.3,1.1.4$ 1.1.2,1.1.3,1.1.4$