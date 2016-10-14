#pragma once
#include "opencv2/opencv.hpp"
using namespace cv;
#include <vector>
#include <string>
using namespace std;
#include "Structure.h"

class Data{
public:
	Data();
	~Data();

public:
	// read the files from subfolders
	void Load();

	// read the mapping from int to string "a~z".
	void LoadMap(char* file);
	
	// read file name from folder: path
	void LoadFileFromFolder();

	// read original image and label image from folder: path
	void LoadImageAndLabel();

	// generate pixel and pixel label for random forest
	void generatePixelAndLabel();

	// generate response map for input image
	void generateResponseMap();

	void resampling();

	// save pixel
	void saveAllPixel(char* file);

	void saveLabel(char* file);

	void saveFileName(char* file);

public:


	vector<DataNode> data_;
	vector<int> pixel_label_all_;
	vector<Element> pixel_index_all_;
	map<string, short> map_;
	int numChar_;
	int num_positive_image;
	int num_negative_image;
	int* num_pixel_class;
};

Data* GlobalData();