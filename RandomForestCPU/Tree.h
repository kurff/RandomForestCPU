#pragma once
#include <stdio.h>
#include "Node.h"
#include "opencv2/opencv.hpp"
const int dimension = 256; // the number of threshold candidates
class Tree{

public:
	Tree();

	~Tree();

public:

	int Partion(int i0, int i1, int r0, int c0, int r1, int c1, char threshold);
	void TrainNodesRecurse(Node* node, int i0, int i1, int depth);
	void Train();
	void Test();
	//void TrainNodesRecurse(Node* node);
	void saveHistogram(int idx,char* file);

	int apply(cv::Mat& response, int y, int x);
	// Load and save tree.
	void SaveNode(Node* node, FILE* f);
	void Load(char* file);
	void Save(char* file);


private:
	Node* root_;
	// shared memory
	FILE* log_file_;
	double** shared_memory_;
	char* shared_feat_;
	double* left_statistic_;
	double* right_statistic_;
	int node_index_;
	int max_depth_;
	unsigned int all_example_number_;
	unsigned int current_example_number_;

};