#pragma once
#include "Random.h"
#include <vector>
using namespace std;
#include "Structure.h"
class Selector{
public:
	// N is the max value of random samples. sampling from [-N, N];
	// num is the dimension of features
	Selector(int N, int num){
	   _N = N;
	   _num = num;
	   _selected = new Element [2*num];
	}
	~Selector(){
		delete [] _selected;
	}

public:
	void selector();
	void loadSelector(char* file);
	void saveSelector(char* file);


public:
   int _N; //
   int _num; // the number of selected features
   //std::vector<Element> _selected;
   Element* _selected;

};