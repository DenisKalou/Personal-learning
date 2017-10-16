#include "std_lib_facilities.h"

vector<int> StrParse(const string& str){
	stringstream ss;
	vector<int> tmp(4);
	int test = 0;
	ss << str;
	ss >> test;
	for (size_t i = 0; i < 4; ++i){
		tmp[3-i] = (test / int(pow(10, i))) % 10;
	}
	cout << endl;
	return tmp;
}
bool CheckInput(vector<int> vl, vector<int> vr)
{
	int bull_count = 0;
	int cows_count = 0;
	int help = 0;
	for (int i = 3; i >= 0; --i){
		if (vl[i] == vr[i]){
			bull_count++;
			help |= int(pow(2 , 3-i));
		} else {
			for(int j = 3; j >= 0; --j){
				
				if (vl[i] == vl[j]){
					cows_count++;
				}
			}
		}
	}
	cout << help << endl;
	if (bull_count == 4){
		cout << "You done right!" << endl;
		return true;
	} else{
		cout << bull_count << " bulls; "
				<< cows_count << " cows." << endl;
		return false;
	}
}

int main (){
	vector<int> prog_number= {1, 2, 3, 4};
	vector<int> user_number;
	for(string str; cin >> str;){
		user_number = StrParse(str);
		cout << "You entered : ";
		for (const auto& a : user_number){
			cout << a;
		}
		cout << endl;
		bool tmp = CheckInput(prog_number, user_number);
		if (tmp){
			break;
		}
	}
	return 0;
}
