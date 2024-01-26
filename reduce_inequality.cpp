// Using alorithm greedy // 
/* Output of programm will be stored in file Trial_reduce_ineq.txt that
   describes the constraint equations to build the MILP model for modular addition */

#include<iostream>
#include<bitset>
#include<fstream>
#include<vector>

using namespace std;

#define row_t 56    // Number of row in Trail.txt
#define col_t 7		// Number  of column in Trail.txt
#define size_full_t (1 << col_t)
#define row_ineq 53 // Number of row in Trail_ineq.txt
#define col_ineq 8	 // Number of column in Trail_ineq.txt

void fileout_matrix(string filename, int** A, int row, int col){
	ifstream file;
	file.open(filename);
	if(!file){
		cout << "Failed to open File\n";
		exit(0);
	}
	while(!file.eof()){
		for(int i = 0; i < row; i++){
			for(int j = 0; j < col; j++){
				file >> (A[i][j]);
			}
		}
	}
	file.close();
}

void print_Matrix(int **A, int row, int col){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			cout << A[i][j] << " ";
		}
		cout << endl;
	}
}

int valueofexpress(vector<int> A, vector<int> B){
	int count = 0;
	vector<int> temp;
	for(int i = 0; i < A.size(); i++){
		temp.push_back(A[i]);
	}
	temp.push_back(1);
	for(int i = 0; i < B.size(); i++){
		count += temp[i] * B[i];
	}
	return count;
}

vector<vector<int> > remove_matrix(vector< bitset<col_t> > A, vector< bitset<col_t> > B){

	vector< bitset<col_t> > vector_C_to_bitset;
	for(int i = 0; i < B.size(); i++){
		int count = 0;
		for(int j = 0; j < A.size(); j++){
			if(B[i] == A[j]) count++;
		}
		if(count == 0) vector_C_to_bitset.push_back(B[i]);
	}
	vector<vector<int> > temp;
	for(int i = 0; i < vector_C_to_bitset.size(); i++){
		vector<int> x;
		for(int j = 0; j < col_t; j++){
			x.push_back(vector_C_to_bitset[i][col_t-1-j]);
		}
		temp.push_back(x);
	}
	return temp;
}

void Write_to_File(string filename, vector<vector<int> > A){
	ofstream file;
	file.open(filename);
	if(!file){
		cout << "Failed to open File\n";
		exit(0);
	}
	for(int i = 0; i < A.size(); i++){
		for(int j = 0; j < A[i].size(); j++){
			file << A[i][j] << " ";
		}
		file << endl;
	}
}

int main(){
	/* Creat 2D-array Trail store the trail*/
	string filename_trail = "Trail.txt";
	int** Trail = new int* [row_t];
	for(int i = 0; i < row_t; i++){
		Trail[i] = new int [col_t];
	}
	fileout_matrix(filename_trail, Trail, row_t, col_t);
	/* Tranform Array2D to array bitset */
	vector< bitset<col_t> > Trail_to_bitset;
	for(int i = 0; i < row_t; i++){
		bitset<col_t> x = 0;
		for(int j = 0; j < col_t; j++){
			x |= (Trail[i][j] << (col_t-1-j));
		}
		Trail_to_bitset.push_back(x);
	}

	/* Ineq - 2D array store constraits H-Representation */
	string filename_inequalitiy = "Trail_ineq.txt";
	int** Ineq = new int* [row_ineq];
	for(int i = 0; i < row_ineq; i++){
		Ineq[i] = new int [col_ineq];
	}
	fileout_matrix(filename_inequalitiy, Ineq, row_ineq, col_ineq);
	vector<vector<int> > Ineq_to_bitset;
	for(int i = 0; i < row_ineq; i++){
		vector<int> x;
		for(int j = 0; j < col_ineq; j++){
			x.push_back(Ineq[i][j]);
		}
		Ineq_to_bitset.push_back(x);
	}

	/* Creat array 2D aTrain store all possile vector length n */
	int** aTrail = new int* [size_full_t];
	for(int i = 0; i < size_full_t; i++){
		aTrail[i] = new int [col_t];
	}
	for(int i = 0; i < size_full_t; i++){
		bitset<col_t> x = i;
		for(int j = 0; j < col_t; j++){
			aTrail[i][j] = x[col_t - 1 - j];
		}
	}
	/* Tranform Array2D to array bitset */
	vector< bitset<col_t> > aTrail_to_bitset;
	for(int i = 0; i < size_full_t; i++){
		bitset<col_t> x = 0;
		for(int j = 0; j < col_t; j++){
			x |= (aTrail[i][j] << (col_t-1-j));
		}
		aTrail_to_bitset.push_back(x);
	}

	/* cTrail store vector not in Trail */
	vector<vector<int> > cTrail;
	cTrail = remove_matrix(Trail_to_bitset, aTrail_to_bitset);
	
	/* Creat array 2D to store reduce inequalities */
	vector<vector<int> > R_ineq;
	while(cTrail.size() > 0){
		vector<vector<int> > temp_p;
		vector<int> temp_l;
		for(int i = 0; i < Ineq_to_bitset.size(); i++){
			vector<vector<int> > temp;
			for(int j = 0; j < cTrail.size(); j++){
				if(valueofexpress(cTrail[j], Ineq_to_bitset[i]) < 0) temp.push_back(cTrail[j]);
			}
			if(temp.size() > temp_p.size()){
				temp_p = temp;
				temp_l = Ineq_to_bitset[i];
			}
		}
		for(int i = 0; i < temp_p.size(); i++){
			for(int j = 0; j < cTrail.size(); j++){
				if(cTrail[j] == temp_p[i]) cTrail.erase(cTrail.begin()+j);
			}
		}
		R_ineq.push_back(temp_l);
		for(int i = 0; i < Ineq_to_bitset.size(); i++){
			if(Ineq_to_bitset[i] == temp_l) Ineq_to_bitset.erase(Ineq_to_bitset.begin() + i);
		}
	}

	/* Creat file name RESLT.txt and store in file */
	string fileout = "Trail_reduce_Ineq.txt";
	Write_to_File(fileout, R_ineq);
	cout << "END" << endl;
}
