#pragma once
#include "opencv2/opencv.hpp"
#include "Selector.h"
#include "Config.h"
using namespace cv;
class Feature{
public:
	Feature(){
	  dimension_feature_ = Global()->dimension_feature;
	  feature_ = new char [dimension_feature_];
	}
	//Feature();

	~Feature(){
	  delete [] feature_;
	}

public:
	inline char transform(int result, int range = 1){
		int t = result/range;
		char a = 0;
		if(t >=127){a = 127;}
		else if(t <=-128){a = -128;}
		else{a = t;}
	    return a;
	}


	inline char extractor0(const Mat& response, int row, int col, int r0, int c0, int r1, int c1, float scale){
		//assert(row < response.rows && col < response.cols);
		int x,y;
		ushort v;
		int x0,y0;
		ushort v0;
		int t;
		x = int(scale*c0) + col;
		y = int(scale*r0) + row;

		//if(x<0) x = -x;
		//if(y<0) y = -y;

		//if(x>= response.cols) x -= response.cols;
		//if(y>= response.rows) y -= response.rows;

		if(x >=0 && x < response.cols && y >=0 && y < response.rows){
			v = response.at<ushort>(y,x);
		}else{
			v = 0;
		}
		x0 = int(scale*c1) + col;
		y0 = int(scale*r1) + row;

		//if(x0<0) x0 = -x0;
		//if(y0<0) y0 = -y0;

		//if(x0>= response.cols) x0 -= response.cols;
		//if(y0>= response.rows) y0 -= response.rows;

		if(x0>=0 && x0 < response.cols && y0 >=0 && y0 < response.rows){
			v0 = response.at<ushort>(y0,x0);
		}
		else{
			v0 = 0;
		}

		t = (int) v - (int) v0;
		//if( t >= 127 ){
		//	v = 127;
		//}
		//else if( t <= -128 ){
		//	v = -128;
		//}
		//else{
		//	v = char(t);
		//}
		 
		return transform(t);
	}

	inline void extractor(const Mat& response, int row, int col,const Selector& selector, float scale){
		//assert(selector._num == dimension_feature_);
		//assert(row < response.rows && col < response.cols);
		int x,y;
		ushort v;
		int x0,y0;
		ushort v0;
		int t;

		memset(feature_,0,(dimension_feature_)*sizeof(char));
		for(int i = 0; i < selector._num; i ++ ){

			x = int(scale*selector._selected[i].x) + col;
			y = int(scale*selector._selected[i].y) + row;

			//if(x<0) x = -x;
		 //   if(y<0) y = -y;

		 //   if(x>= response.cols) x -= response.cols;
		 //   if(y>= response.rows) y -= response.rows;


			if(x >=0 && x < response.cols && y >=0 && y < response.rows){
				v = response.at<ushort>(y,x);
			}else{
				v = 0;
			}
			x0 = int(scale*selector._selected[i+selector._num].x) + col;
			y0 = int(scale*selector._selected[i+selector._num].y) + row;


			//if(x0<0) x0 = -x0;
		 //   if(y0<0) y0 = -y0;

		 //   if(x0>= response.cols) x0 -= response.cols;
		 //   if(y0>= response.rows) y0 -= response.rows;

			if(x0>=0 && x0 < response.cols && y0 >=0 && y0 < response.rows){
				v0 = response.at<ushort>(y0,x0);
			}
			else{
				v0 = 0;
			}

			t = (int) v - (int) v0;
			//if( t >= 127 ){
			//	feature_[i] = 127;
			//}
			//else if( t <= -128 ){
			//	feature_[i] = -128;
			//}
			//else{
			//	feature_[i] = char(t);
			//}
			feature_[i] = transform(t);
		}
	
	}
	void copy(char feature[]);
	void set(char feature[]);
	void save(char* file, char* type);

private:
    char* feature_;
	int dimension_feature_;
	// 当内存足够时;
	// 将(ushort-ushort) -> 0-255
	vector<char* > feature_all_;
};