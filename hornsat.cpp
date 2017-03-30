/***
	*
	* @author victoraurelio.com
	* @since 27/10/16
	*
*/
#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>

#include <algorithm>
#include <fstream>

using namespace std;

// file to output results
ofstream outFile;
// a vector save represent clauses on expr

bool isFNC(string expr){
	int p=0;
	for(int i=0; i<expr.size(); i++){
		if(expr.at(i) == '('){
			if(p>0) return false;
			else p++;
		}else if(expr.at(i) == ')') p--;
		
		if(p > 0 && expr.at(i) == '.') return false;		
		if(p == 0 &&  expr.at(i) != ')' && expr.at(i) != '.') return false;
	}
	return true;
}
bool isHornClause(string expr){
	int positive=0, p=0;
	for(int i=0; i<expr.size(); i++){
		if(expr.at(i) == '(')p++;
		else if(expr.at(i) == ')'){
			p--;
			if(p==0) positive=0;
		}
		if(96<expr.at(i) && expr.at(i)<101){
			if(expr.at(i-1) != '-'){
				positive++;
				if(positive>1) return false;
			}
		}
	}
	return true;
}
vector<string> getExprs(string s){
	vector<string> clause;
	vector<int> l;//left
	for(int i=0; i<s.size(); i++){
		if(s.at(i) == '('){			
			l.push_back(i);
		}else if(s.at(i) == ')'){			
			clause.push_back(s.substr(l.back(), i-l.back()+1));
			l.pop_back();
		}
	}
	return clause;
}

bool firstLessThenNext(string i,string j){
	if(i.size() < j.size()) return true;	
	return false;
}
string toUpperNegativeLiteral(string s){// turns -b in B;
	for(int i=0; i<s.size(); i++){
		if(s.at(i) == '-'){
			s.at(i) = toupper(s.at(i+1));
			s.erase(s.begin() + i + 1);
		}
	}
	return s;
}
string eliminateRedundancy(string s){
	bool present[8];
	memset(present, false, 8 * sizeof(bool));
	int ind;
	for(int i=1; i < (s.size()-1); i++){		
		if(s.at(i) == ')') memset(present, false, 8 * sizeof(bool));
		if(!(s.at(i-1) == '(' && s.at(i+1) == ')') &&  ((64 < s.at(i) && s.at(i) < 69) || (96 < s.at(i) && s.at(i) < 101) ) ){			
			ind = s.at(i) == toupper(s.at(i)) ? s.at(i) - 65 + 4 : s.at(i) - 97;			
			if(present[ind]){				
				if(s.at(i+1) == ')'){
					s.erase(i-1,2);
					memset(present, false, 8 * sizeof(bool));
				}else s.erase(i,2);
				i-=1;
			}else{
				present[ind] = true;
			}
		}		
	}
	return s;
}
bool notExists(vector<string> a, string s){
	for(int i=0; i<a.size(); i++)
		if(s.compare(a[i]) == 0) return false;
	return true;
}
bool existsOppositeClauses(vector<string> ut){
	for(int i=0; i<ut.size(); i++)
		for(int j=0; j<ut.size(); j++)			
			if(i != j && toupper(ut[i].at(1)) == toupper(ut[j].at(1)) && ut[i].at(1) != ut[j].at(1))
				return true;		
	return false;
}
bool hornSat(string expr){	
	bool newUnit = true;
	string tmp;
	vector<string> mUnits, units, clause;//all, new, unit

	expr = toUpperNegativeLiteral(expr);	
	expr = eliminateRedundancy(expr);	
	clause = getExprs(expr);
	
	sort(clause.begin(), clause.end(), firstLessThenNext);	
	
	for(int i=0; i<clause.size(); i++){		
		if(clause[i].size() < 4){// is (a) or (A)
			units.push_back(clause[i]);
			mUnits.push_back(clause[i]);
		}
	}

	while(!units.empty()){				
		char unit = units.front().at(1);
		char inverse = toupper(unit) == unit ? tolower(unit) : toupper(unit);				
		for(int i=0; i<clause.size(); i++){			
			if(units.front().compare(clause[i]) != 0){
				size_t pn = clause[i].find(unit);
				size_t pu = clause[i].find(inverse);
				if(pn != string::npos){					
					clause.erase(clause.begin() + i);
					i-=1;
				}else if(pu != string::npos){
					tmp = clause[i];					
					while(pu != string::npos){
						if(tmp.size() < 4) return false;
						if(tmp.at(pu+1) == ')'){
							tmp.erase(pu-1, 2);
						}else{
							tmp.erase(pu, 2);
						}
						pu = tmp.find(inverse);
					}					
					if(tmp.size() < 4){
						for(int j=0; j<units.size(); j++){
							if(toupper(tmp.at(1)) == toupper(mUnits[j].at(1)) && tmp.at(1) != mUnits[j].at(1)) return false;
						}
						mUnits.push_back(tmp);
						units.push_back(tmp);
					}
					if(notExists(clause, tmp)) clause.push_back(tmp);					
				}
			}
		}				
		units.erase(units.begin(), units.begin()+1);				
	}	

	return true;
}
int main(){
	int c=0;
	string expr;
	ifstream inFile ("Expressoes.in", ios::in);
	outFile.open("Expressoes.out", ios::out);

	if(inFile.is_open() && outFile.is_open()){
		inFile >> c;
		for(int k=1; k<=c; k++){
			outFile << "caso #" << k << ": ";
			inFile >> expr;			
			
			if(!isHornClause(expr) && isFNC(expr)){
				outFile << "nem todas as clausulas sao de horn";
			}else if(!isFNC(expr)){
				outFile << "nao esta na FNC";
			}else if(hornSat(expr)){
				outFile << "satisfativel";
			}else{
				outFile << "insatisfativel";				
			}			
			
			outFile << endl;			
		}
		inFile.close();
		outFile.close();
	}else{
		//cout << "Nao consegui abrir o arquivo";
	}
	return 0;
}