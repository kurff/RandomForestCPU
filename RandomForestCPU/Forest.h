#pragma once
#include "Tree.h"
#include <vector>
using namespace std;
#include "opencv2/opencv.hpp"
using namespace cv;
class Forest{
public:
	Forest(int tree_number);
	~Forest();

public:
	void Save(char* file);
	void Load(char* file);

	void Train();
	void Apply(const Mat& response);



private:
	int tree_number_;
	vector<Tree> tree_;



};