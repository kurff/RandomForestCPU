#include "Feature.h"

//char Feature::extractor0(const Mat& response, int row, int col, int r0, int c0, int r1, int c1, float scale){
//
//}

//void Feature::extractor(const Mat& response, int row, int col, const Selector& selector, float scale){
//	assert(selector._num == dimension_feature_);
//	assert(row < response.rows && col < response.cols);
//	int x,y;
//	uchar v;
//	int x0,y0;
//	uchar v0;
//	int t;
//
//	memset(feature_,0,(dimension_feature_)*sizeof(char));
//	for(int i = 0; i < selector._num; i ++ ){
//
//		 x = int(scale*selector._selected[i].x) + col;
//	     y = int(scale*selector._selected[i].y) + row;
//
//		 if(x >=0 && x < response.rows && y >=0 && y < response.cols){
//		     v = response.at<uchar>(y,x);
//		 }else{
//		     v = 0;
//		 }
//		 x0 = int(scale*selector._selected[i+selector._num].x) + col;
//	     y0 = int(scale*selector._selected[i+selector._num].y) + row;
//
//		 if(x0>=0 && x0 < response.rows && y0 >=0 && y0 < response.cols){
//		    v0 = response.at<uchar>(y0,x0);
//		 }
//		 else{
//		    v0 = 0;
//		 }
//
//		 t = (int) v - (int) v0;
//		 if( t >= 127 ){
//		    feature_[i] = 127;
//		 }
//		 else if( t <= -128 ){
//		    feature_[i] = -128;
//		 }
//		 else{
//		    feature_[i] = char(t);
//		 }
//	}
//}

void Feature::copy(char feature[]){
	int i = 0;
	for( i = 0; i < dimension_feature_; i ++ ){
	    feature[i] = feature_[i];
	}
}

void Feature::set(char feature[]){
	int i = 0;
	for( i = 0; i < dimension_feature_; i ++ ){
	    feature_[i] = feature[i];
	}

}

void Feature::save(char* file, char* type){
	FILE* f  = fopen(file,type);
	for(int i = 0; i < dimension_feature_; i ++ ){
	  fprintf(f,"%d ", feature_[i]);
	}
	fclose(f);
}




