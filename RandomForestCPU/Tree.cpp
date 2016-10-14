#include "Tree.h"
#include <memory.h>
#include "Config.h"
#include "Selector.h"
#include "Data.h"
#include "Feature.h"
#include "Response.h"

//#define MULTI_THREAD



Tree::Tree(){
	int dimension_feature = Global()->dimension_feature;
	shared_memory_ = new double*[dimension_feature];
	
	int num_class = Global()->num_class;

	for(int i = 0; i < dimension_feature; i ++ ){
		shared_memory_[i] = new double [dimension*(num_class)]();
	}


	left_statistic_ = new double [num_class];
	right_statistic_ = new double [num_class];
	shared_feat_ = new char [dimension_feature ];
	max_depth_ = Global()->max_depth;
	node_index_ = 0;
	root_ = new Node ();

	log_file_ = fopen("log.txt","w");
	if(log_file_==NULL)	exit(0);


}

Tree::~Tree(){
	fclose(log_file_);
	int dimension_feature = Global()->dimension_feature;
	for(int i = 0; i < dimension_feature; i ++ ){
		delete [] shared_memory_[i];
	}

	delete [] shared_memory_;

	delete [] shared_feat_;

	delete [] left_statistic_;

	delete [] right_statistic_;
}



void Tree::Load(char* file){





}

void Tree::SaveNode(Node* node, FILE* f){
	fprintf(f,"%d ",node->node_index_);
	fprintf(f,"%d %d ",node->bIsLeaf_,node->bIsSplit_);
	fprintf(f,"%d %d %d %d ",node->r0_, node->c0_, node->r1_, node->c1_);
	fprintf(f,"%d ", node->threshold_);
	fprintf(f,"%d ", node->feature_index_);
	fprintf(f,"%d %d ",node->left_child_index_, node->right_child_index_);
	fprintf(f,"%d\n", node->num_class_);
	if(node->bIsLeaf_){
		for(int i = 0; i < node->num_class_;i ++ ){
			fprintf(f,"%f ",node->statistic_[i]);
		}
	    fprintf(f,"\n");
	}
	if(node->left_ != nullptr )  SaveNode(node->left_,  f);	
	if(node->right_ != nullptr)  SaveNode(node->right_, f);

}

void Tree::Save(char* file){
	FILE* f = fopen(file,"w");
	fprintf(f,"%d\n", node_index_+1);
	SaveNode(root_,f);
	fclose(f);
}

void Tree::saveHistogram(int idx,char* file){
	
	int num_class = Global()->num_class;
	FILE* f = fopen(file,"w");
	for(int i = 0;  i < dimension; i ++ ){
		for(int j = 0; j < (num_class); j ++ ){
			fprintf(f,"%f ",shared_memory_[idx][i*(num_class) + j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);


}

void Tree::Train(){
	Data* data = GlobalData();
	all_example_number_ = data->pixel_index_all_.size();
	current_example_number_ = 0;
	//data->saveAllPixel("feat.txt");
	TrainNodesRecurse(root_, 0, data->pixel_index_all_.size(), 0);
	printf("training process down!\n");
	Save(Global()->save_tree_path);
	Test();
	//system("Shutdown -s -t 1");
}

int Tree::Partion(int i0, int i1, int r0, int c0, int r1, int c1, char threshold){
	Data* data = GlobalData();
	char v = 0;
	Feature feature;
	assert(i1 > i0);
	int i = i0;
	int j = i1 - 1;
	Element element;
	int label;
	//FILE* f = fopen("allfeat.txt","w");
	while(i!=j){

		element = data->pixel_index_all_[i];
		v = feature.extractor0(data->data_[element.idx].response,
			element.y, element.x, r0,c0,r1,c1,1.0);
		//fprintf(f,"%d ",v);
		if(v >= threshold){
			// swap the element and label
			element = data->pixel_index_all_[i];
			data->pixel_index_all_[i] = data->pixel_index_all_[j];
			data->pixel_index_all_[j] = element;
			label = data->pixel_label_all_[i];
			data->pixel_label_all_[i] = data->pixel_label_all_[j];
			data->pixel_label_all_[j] = label;		
			j--;
		}else{
		  i ++;
		}
	    
	}

	//fclose(f);
	element = data->pixel_index_all_[i];
	v = feature.extractor0(data->data_[element.idx].response,
			element.y, element.x, r0,c0,r1,c1,1.0);

	return v >= threshold ? i : i + 1;
}

void Tree::TrainNodesRecurse(Node* node, int i0, int i1, int depth){
	//
	printf("node_index_ : %5d,  number of example: %10d,  depth: %2d,  %4.2f%%\n", node_index_, i1 - i0 , depth, 100*double(current_example_number_)/double(all_example_number_));
	fprintf(log_file_,"node_index_ : %5d,  number of example: %10d,  depth: %2d, %4.2f%%\n", node_index_, i1 - i0 , depth, 100*double(current_example_number_)/double(all_example_number_));
	
	Configuration* conf = Global();
	int dimension_feature = conf->dimension_feature;
	int max_probe_offset = conf->max_probe_offset;
	Selector select(max_probe_offset,dimension_feature);
	select.selector();
	
	//char save_name[100];
	//sprintf(save_name,".\\select\\selector%d.txt",node_index_);
	//select.saveSelector(save_name);

	int i,j,k;

	Feature feature;
	Data* data = GlobalData();


	int num_class = Global()->num_class;
	int col = 0;
	int row = 0;
	int label = 0;

	// 三个终止条件 1) depth 超过阈值 2) 熵的值小于阈值
	//             3) 该节点的样本个数小于阈值
	// depth, entropy_parent, i1 - i0 || i1 - i0 
	
	if(depth >= conf->max_depth || i1 - i0  <= conf->num_samples_node){
		node->initialLeaf(node_index_);
		for( i = i0; i < i1; i ++ ){
			label = data->pixel_label_all_[i];
			data->pixel_index_all_[i].leafId = node_index_;
			node->statistic_[label] ++;
		}
		current_example_number_ += (i1-i0);

		for( i = 0; i < num_class ; i ++ ){
			node->statistic_[i] /= double(i1 - i0 ) + 1e-20;
			fprintf(log_file_,"%f ",node->statistic_[i]);
		}
		fprintf(log_file_,"\n");
		//printf("Exceed max depth or smaller than num_samples_node\n");
		return;
	
	}


	node->initialSplit(node_index_);
	for( i = 0; i < dimension_feature; i ++ ){
	  	memset(shared_memory_[i], 0, dimension*(num_class)*sizeof(double));
	}



	
#ifdef MULTI_THREAD
	#pragma omp parallel for
#endif
	
	for( i = i0; i < i1; i ++ ){
		Element element = data->pixel_index_all_[i];
		feature.extractor(data->data_[element.idx].response, element.y,
			element.x, select, 1.0);
		feature.copy(shared_feat_);
		char feat_name[200];
		//sprintf(feat_name,"./feature0/%d_%d_%d.txt",element.idx,element.y,element.x);
		//feature.save(feat_name,"w");
		for( j = 0; j < dimension_feature; j ++ ){
			col = shared_feat_[j] + 128;
            //assert(col >= 0 && col < 256 );
			label = data->pixel_label_all_[i];
			shared_memory_[j][col*(num_class) + label] ++;
		}
	}

	//for( i = 0; i < dimension_feature; i ++ ){
	//	char name[200];
	//	sprintf(name,"./histogram/%d.txt",i);
	//	//saveHistogram(i,name);
	//}

	double maxGain = -1e20;
	double gain = 0;
	
	double num_left = 0;
	double num_right = 0;

	double entropy_left = 0;
	double entropy_right = 0;
	double entropy_parent = 0;
	//Node* split_node = new Node();
	//split_node->initialSplit();
	int n;
	for( n = 0; n < dimension_feature; n++  ){
	  memset(left_statistic_,0, (num_class)*sizeof(double));
	  memset(right_statistic_,0,(num_class)*sizeof(double));

	  
	  //saveHistogram(i);
	  for( k = 0; k < num_class ; k ++ ){
		  for( j = 0; j < dimension; j ++ ){  // the number of threshold candidate
			  right_statistic_[k] += shared_memory_[n][j*(num_class) + k];		   
		  }
	  } 

	  //for(k = 0; k < num_class ; k ++){
	  //   printf("%4.3f ", right_statistic_[k]);
	  //}
	  //printf("\n");

	  entropy_parent = 0;
	  for( k = 0; k < num_class; k ++ ){
		  if(right_statistic_[k] > 1e-10 ){
	        entropy_parent -= (right_statistic_[k] / (double(i1 - i0)+1e-20))*std::log(right_statistic_[k] / (double(i1 - i0)+1e-20))/std::log(2.0);		  
		  }
	  }

	  if(entropy_parent <= 0.0){
		  node->initialLeaf(node_index_);
		  for( i = i0; i < i1; i ++ ){
			  label = data->pixel_label_all_[i];
			  data->pixel_index_all_[i].leafId = node_index_;
			  node->statistic_[label] ++;
		  }
		  current_example_number_ += (i1-i0);

		  for( i = 0; i < num_class ; i ++ ){
			  node->statistic_[i] /= double(i1 - i0 ) + 1e-20;
			  fprintf(log_file_,"%f ",node->statistic_[i]);
		  }
		  fprintf(log_file_,"\n");
		  //printf("Exceed max depth or smaller than num_samples_node\n");
		  return;
	  }


	   
	  for( j = 0;  j < dimension; j ++ ){
		   for( k = 0; k < num_class; k ++ ){
		      left_statistic_[k] += shared_memory_[n][j*(num_class) + k];
			  right_statistic_[k] -= shared_memory_[n][j*(num_class) + k];
		   }		   
	   // compute the information gain;
		   num_left = 0;
		   num_right = 0;
		   for(k = 0; k < num_class; k ++ ){
			   num_left += left_statistic_[k];
			   num_right += right_statistic_[k];
		   }

		   entropy_left = 0;
		   entropy_right = 0;
		   double left_prob = 0, right_prob = 0;
		   for(k = 0; k < num_class; k ++){
			   if(left_statistic_[k] >= 1e-10 ){
                  left_prob = (left_statistic_[k]/(double(num_left)+1e-20));
			      entropy_left -= left_prob*std::log(left_prob)/std::log(2.0);			   
			   }
			   if(right_statistic_[k] >= 1e-10 ){
				   right_prob = (right_statistic_[k]/((double)num_right+1e-20));
			      entropy_right -= right_prob*std::log(right_prob)/std::log(2.0);	   
			   }
		   }

           gain = entropy_parent - (num_left/(num_left + num_right))*entropy_left - (num_right/(num_left + num_right))*entropy_right;

		   if(maxGain < gain){
		      maxGain = gain;
			  //if(gain ==0){
			  //   saveHistogram(i);
			  //}
			  //saveHistogram(i);
			  node->threshold_ = (j - 128);
			  node->r0_ = select._selected[n].y;	
			  node->c0_ = select._selected[n].x;
			  node->r1_ = select._selected[n+dimension_feature].y;
			  node->c1_ = select._selected[n+dimension_feature].x;
			  node->feature_index_ = n;
		   }

	  }
	}

	//printf("node->threshold_: %d, maxGain: %f\n", node->threshold_, maxGain);
	//if( maxGain < conf->threshold_entropy){
	//   	node->initialLeaf(node_index_);
	//	for( i = i0; i < i1; i ++ ){
	//		label = data->pixel_label_all_[i];
	//		data->pixel_index_all_[i].leafId = node_index_;
	//		node->statistic_[label] ++;
	//	}
	//	current_example_number_ += (i1-i0);
	//	for( i = 0; i < num_class ; i ++ ){
	//		node->statistic_[i] /= double(i1 - i0 ) + 1e-20;
	//		fprintf(log_file_,"%f ",node->statistic_[i]);
	//	}
	//	fprintf(log_file_,"\n");
	//	//printf("Smaller than threshold_entropy\n");

	//	return;	
	//}


	// quick sort

	int ii = Partion( i0,  i1, node->r0_,  node->c0_,
		node->r1_, node->c1_, node->threshold_);

	//if( ii-i0 ==0 || i1-ii==0){
	//	node->initialLeaf(node_index_);
	//	for( i = i0; i < i1; i ++ ){
	//		label = data->pixel_label_all_[i];
	//		node->statistic_[label] ++;
	//	}
	//	current_example_number_ += (i1-i0);
	//	for( i = 0; i < num_class ; i ++ ){
	//		node->statistic_[i] /= double(i1 - i0 ) + 1e-20;
	//		fprintf(log_file_,"%f ",node->statistic_[i]);
	//	}
	//	fprintf(log_file_,"\n");
	//	printf("ii-i0 smaller than 1\n");

	//	return;	
	//
	//}

	node_index_ ++;
	node->left_child_index_ = node_index_;

#ifdef DEBUG// _DEBUG
	double* statistic_l = new double [num_class];
	memset(statistic_l,0, (num_class)*sizeof(double));
	for(int i = i0; i < ii; i ++ ){
	   statistic_l[data->pixel_label_all_[i]] ++; 
	}
	double* statistic_r = new double [num_class];
	memset(statistic_r,0, (num_class)*sizeof(double));

	for(int i = ii; i < i1; i ++ ){
		statistic_r[data->pixel_label_all_[i]] ++; 
	}


	for(int i = 0; i < num_class ; i ++ ){
	  printf("%4.3f ", statistic_l[i]);
	}

	printf("\n");

	for(int  i = 0; i < num_class ; i ++ ){
	  printf("%4.3f ", statistic_r[i]);
	}
	printf("\n");

#endif
	node->left_ = new Node();	
	TrainNodesRecurse(node->left_,  i0, ii,  depth+1);
	node_index_ ++;
	node->right_child_index_ = node_index_;
    node->right_ = new Node();
	TrainNodesRecurse(node->right_,  ii, i1,  depth+1);

}

void Tree::Test(){
	Data* data = GlobalData();
	Configuration* conf = Global();
	int dimension_feature = conf->dimension_feature;
	int max_probe_offset = conf->max_probe_offset;
	
	char v = 0;

	int label = 0;
	int counter = 0;
	int all_example = data->pixel_index_all_.size();
	FILE* f = fopen("d.txt","w");
	fprintf(f,"%d\n",all_example);
	for(int i = 0; i < all_example; i ++ ){
	   	Element element = data->pixel_index_all_[i];
		//v = feature.extractor0(data->data_[element.idx].response,
		//		i, j, root_[current].r0_,root_[current].c0_,
		//		root_[current].r1_,root_[current].c0_,1.0);	
		//feature.extractor(data->data_[element.idx].response, element.y,
		//	element.x, select, 1.0);

		label = apply(data->data_[element.idx].response, element.y, element.x);
		if(label == data->pixel_label_all_[i]){
		   counter ++;
		}
		fprintf(f,"%d %d %d %d %d %d\n",element.idx,element.y,element.x,
			element.cnt, element.leafId, label);

	}
	fclose(f);
	printf("train result: %4.2f%% \n",100*double(counter)/double(all_example));
	fprintf(log_file_,"train result: %4.2f%% \n",100*double(counter)/double(all_example));
}

int Tree::apply( Mat& response, int y, int x){
	Feature feature;
	Node* current = root_;
	char v = 0;
	while( current->bIsSplit_ ){

		v = feature.extractor0(response,
				y, x, current->r0_,current->c0_,
				current->r1_, current->c1_, 1.0);

		if( v < current->threshold_ ){
			current = current->left_;
		}else{
		    current = current->right_;
		}
	}

	double maxScore = 0;

	int label = 0;
	double s = 0;
	for(int i = 0; i < current->num_class_; i ++ ){
		if( maxScore <= current->statistic_[i]){
			maxScore = current->statistic_[i];
		    label = i;
		}
		s += current->statistic_[i];
	}

	
	//printf("%d\n",current->node_index_);
	return label;






}