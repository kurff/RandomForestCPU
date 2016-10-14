#include "Selector.h"
#include "opencv2/opencv.hpp"
#include "Response.h"
using namespace cv;
#include "Data.h"
#include "Config.h"
#include "Tree.h"
#include "Feature.h"
void main(){
   Configuration* pconfig = Global();
   pconfig->LoadConfiguration("Configuration.txt");

   Data* data = GlobalData();
   data->Load();
   data->LoadImageAndLabel();
   data->generatePixelAndLabel();
   data->generateResponseMap();
   //data->saveAllPixel("allpixel.txt");
   //data->saveFileName("file_name.txt");

   	/*Configuration* conf = Global();
	int dimension_feature = conf->dimension_feature;
	int max_probe_offset = conf->max_probe_offset;
	Selector select(max_probe_offset,dimension_feature);
	select.selector();

	Feature feature;
   
   for(int i = 0; i < data->pixel_label_all_.size(); i ++){
        Element element = data->pixel_index_all_[i];
        feature.extractor(data->data_[element.idx].response, element.y,
			element.x, select, 1.0);
		char name[200];
		sprintf(name,"./feature/%d.txt",i);
		feature.save(name,"w"); 
   }

   data->saveLabel("label.txt");*/
   //printf("%d", uchar(-8));
   //data->saveAllPixel("index.txt");
   Tree tree;
   tree.Train();


}		