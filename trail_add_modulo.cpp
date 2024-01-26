// This program implements the vector representation used in the MILP model for modulo addition
// Output will be stored in file name "Trail.txt"

#include<iostream>
#include<fstream>
#include<bitset>
#include<vector>
using namespace std;

bool check_var(bitset<7> x){
    bool temp = true;
    if(x[6] == x[5] && x[5] == x[4]){
        if(x[6] != (x[3] ^ x[2] ^ x[1]) || x[0] != 1){
            temp = false;
        }
    }
    else if(x[6] != x[5] || x[6] != x[4] || x[5] != x[4]){
        if(x[0] != 0) temp = false;
    }
    return temp;
}

int main(){
    string filename = "Trail_sage.txt";
    ofstream file;
    file.open(filename);
    int count = 0;
    for(int i = 0; i < (1 << 7); i++){
        bitset<7> x = i;
        bool temp = check_var(x);
        if(temp == true){
            file << "[";
            for(int i = 0; i < 7; i++){
                if(i < 6) file << x[6-i] << ", ";
                else file << 1 - x[6-i];
            }
            file << "]," << endl;
            count++;
        }
    }
    string filename_1 = "Trail.txt";
    ofstream file1;
    file1.open(filename_1);
    int count1 = 0;
    for(int i = 0; i < (1 << 7); i++){
        bitset<7> x = i;
        bool temp = check_var(x);
        if(temp == true){
            for(int i = 0; i < 7; i++){
                if(i < 6) file1 << x[6-i] << " ";
                else file1 << 1 - x[6-i];
            }
            file1 << endl;
            count1++;
        }
    }
    // cout << count << endl;
}