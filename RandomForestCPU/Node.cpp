#include "Node.h"
#include "Config.h"
#include <memory>
bool Node::initialNode(int node_index){

	right_ = nullptr;
	left_ = nullptr;
	bIsLeaf_ = false;
	bIsSplit_ = false;
	threshold_ = 0;
	node_index_ = node_index;
	r0_ = 0;
	c0_ = 0;
	r1_ = 0;
	c1_ = 0;
	feature_index_ = 0;
	num_class_ = Global()->num_class;
	statistic_ = new double [num_class_ +1];
	if(statistic_ == nullptr){
	  return false;
	}
    memset(statistic_, 0, (num_class_ + 1)*sizeof(double));
	return true;
}

bool Node::initialLeaf(int node_index){
	right_ = nullptr;
	left_ = nullptr;
	bIsLeaf_ = true;
	bIsSplit_ = false;
	threshold_ = 0;
	node_index_ = node_index;
	r0_ = 0;
	c0_ = 0;
	r1_ = 0;
	c1_ = 0;
	feature_index_ = 0;
	num_class_ = Global()->num_class;
	statistic_ = new double [num_class_ +1];
	if(statistic_ == nullptr){
	  return false;
	}
    memset(statistic_,0,(num_class_+1)*sizeof(double));
	return true;

}

bool Node::initialSplit(int node_index){
	right_ = nullptr;
	left_ = nullptr;
	bIsLeaf_ = false;
	bIsSplit_ = true;
	threshold_ = 0;
	node_index_ = node_index;
	r0_ = 0;
	c0_ = 0;
	r1_ = 0;
	c1_ = 0;
	feature_index_ = 0;
	num_class_ = Global()->num_class;
	return true;

}

bool Node::destroyNode(){
	delete [] statistic_;
	statistic_ = nullptr;
	return true;
}