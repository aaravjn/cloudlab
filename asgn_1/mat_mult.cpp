/*
This script will run in the Virtual Machine
*/

#include <bits/stdc++.h>

using namespace std;


vector<vector<int>> multiply(vector<vector<int>> matrix1, vector<vector<int>> matrix2) {
	int n = matrix1.size();

	vector<vector<int>> result(n, vector<int> (n));
	for(int i = 0;i < n;i++) {
		for(int j = 0;j < n;j++) {
			int sum = 0;
			for(int k = 0;k < n;k++) {
				sum += matrix1[i][k] * matrix2[k][j];
				result[i][j] = sum;
			}
		}
	}

	return result;
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

	char* mat1 = argv[2];
	char* mat2 = argv[3];

	vector<vector<int>> matrix1(n), matrix2(n);
	matrix1 = extract(n, mat1);
	matrix2 = extract(n, mat2);
	
	
	vector<vector<int>> result = multiply(matrix1, matrix2);

	for(int i = 0;i < n;i++) {
		for(int j = 0;j < n;j++) {
			cout<<result[i][j]<<' ';
		}
		cout<<endl;
	}

}

// ./a.out 3 1.1.2,1.1.3,1.1.4$ 1.1.2,1.1.3,1.1.4$