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
	vector<string> units, clause;//all, new, unit

	expr = toUpperNegativeLiteral(expr);		
	clause = getExprs(expr);
	
	sort(clause.begin(), clause.end(), firstLessThenNext);	
	for(int i=0; i<clause.size(); i++){		
		if(clause[i].size() < 4){// is (a) or (A)
			units.push_back(clause[i]);			
		}
	}

	while(!units.empty()){		
		char unit = units.front().at(1);
		char inverse = toupper(unit) == unit ? tolower(unit) : toupper(unit);
				
		for(int i=0; i<clause.size(); i++){				
			if(clause[i].compare(units.front()) != 0){
					size_t pn = clause[i].find(unit);
					size_t pu = clause[i].find(inverse);
					if(pn != string::npos && units.front().size() < 4){//é uma unit							
						clause.erase(clause.begin() + i);
					}else if(pu != string::npos){
						//caso eu tenha encontrado algum inverso da unit na expressao vou apagar todas as ocorrencias desse inverso.
						for(int k=0; k<clause[i].size(); k++){
							if(clause[i].at(k) == inverse){
								tmp = clause[i];
								//caso eu tenha encontrado o inverso da unit em uma expressão que é atomica eu encontrei uma contradição..
								if(tmp.size() < 5){
									return false;
								}else{// (a+b+c) our (a+c+b) e quero remover b, ai preciso saber se removo b+ ou +b										
									if(clause[i].at(k+1) == ')'){											
										tmp.erase(k-1, 2);										
									}else{						
										tmp.erase(k, 2);										
									}										
									if(notExists(clause, tmp)){
										clause.push_back(tmp);																	
										if(tmp.size() < 4){//new unit clause											
											units.push_back(tmp);												
										}	
									}
								}
							}
						}
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
		cout << "Nao consegui abrir o arquivo";
	}
	return 0;
}