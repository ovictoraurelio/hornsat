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
vector<string> clause;

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
void getExprs(string s){
	vector<int> l;//left
	for(int i=0; i<s.size(); i++){
		if(s.at(i) == '('){			
			l.push_back(i);
		}else if(s.at(i) == ')'){			
			clause.push_back(s.substr(l.back(), i-l.back()+1));
			l.pop_back();
		}
	}
}
bool firstLessThenNext(string i,string j){
	if(i.size() <= j.size()) return true;
	else{
		for(int k=0; k<i.size()>j.size()?j.size():i.size(); k++){
			if(i.at(k) > j.at(k)) return false;
		}		
	}
	return true;
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
	vector<string> al, nw, ut;//all, new, unit

	expr = toUpperNegativeLiteral(expr);		
	getExprs(expr);
	sort(clause.begin(), clause.end(), firstLessThenNext);
		
	for(int i=0; i<clause.size(); i++){			
		al.push_back(clause[i]);		
		if(clause[i].size() < 5){// is (a) or (-a)
			nw.push_back(clause[i]);
			ut.push_back(clause[i]);
		}
	}

	while(!nw.empty()){
		for(int z=1; z<nw.front().size(); z+=2){		
			if(nw.front().at(z-1) == ')') continue;
			char unit = nw.front()[z];
			char inverse = toupper(unit) == unit ? tolower(unit) : toupper(unit);
					
			for(int i=0; i<al.size(); i++){				
				if(al[i].compare(nw.front()) != 0){							
						size_t pn = al[i].find(unit);
						size_t pu = al[i].find(inverse);
						if(pn != string::npos && nw.front().size() < 5){//é uma unit							
							al.erase(al.begin() + i);
						}else if(pu != string::npos){
							for(int k=0; k<al[i].size(); k++){
								if(al[i].at(k) == inverse){																	
									tmp = al[i];
									// (a+b+c) our (a+c+b) e quero remover b, ai preciso saber se removo b+ ou +b
									if(al[i].at(k+1) == ')' && nw.front().size() < 5){										
										tmp.erase(k-1, 2);										
									}else if(notExists(al,al[i])){
										tmp.erase(k, 2);										
									}
									if(notExists(al, tmp)){
										al.push_back(tmp);																	
										if(tmp.size() < 5){//new unit clause
											newUnit	= true;
											nw.push_back(tmp);	
											ut.push_back(tmp);
										}	
									}
								}
							}
						}
					}
				}
			}

			if(newUnit && existsOppositeClauses(ut)) return false;
			newUnit = false;
			nw.erase(nw.begin(), nw.begin()+1);				
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
			if(!clause.empty())
				clause.clear();
			outFile << endl;			
		}
		inFile.close();
		outFile.close();
	}else{
		cout << "Nao consegui abrir o arquivo";
	}
	return 0;
}