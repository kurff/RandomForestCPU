
#include "Data.h"
#include<windows.h>
#include "Response.h"
#include "Config.h"
Data data;

Data* GlobalData(){
	return &data;
}

Data::Data():num_pixel_class(nullptr){

}

Data::~Data(){
	if(num_pixel_class!=nullptr)
	delete [] num_pixel_class;
}

void Data::Load(){
	Configuration* p = Global();
	LoadFileFromFolder();
	num_positive_image = data_.size();
	LoadMap(p->map_path);

}

void Data::LoadImageAndLabel(){
	unsigned int i = 0;
	IplImage* img = NULL;
	for( i =0 ; i < data_.size(); i ++ ){
		//img = cvLoadImage(data_[i].imageName,CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
		data_[i].image = imread(data_[i].imageName,CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
		//cvNot(&IplImage(data_[i].image),&IplImage(data_[i].image));
		//imshow("src",data_[i].image);
		//waitKey(0);
		if(data_[i].image.empty()){
		   continue;
		}
		//imshow();
		//data_[i].image = Mat(img);
		//cvShowImage("image",img);
		//img = cvLoadImage(data_[i].labelName,CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
		data_[i].label = imread(data_[i].labelName,CV_LOAD_IMAGE_GRAYSCALE);
		//imshow("label",data_[i].label);
		//cvWaitKey(0);
		//cout<<data_[i].label<<endl;
		if(data_[i].label.empty()){
		   //img = cvCreateImage(cvSize(data_[i].image.rows, data_[i].image.cols),
			  // IPL_DEPTH_16U, 1);
		   //cvZero(img);
			data_[i].label = Mat::zeros(data_[i].image.rows, data_[i].image.cols,CV_8UC1);
		}
		//data_[i].label = Mat(img);//imread(data_[i].labelName);
		//cout<<"Load" <<data_[i].imageName<<endl;
	}
}

void Data::generatePixelAndLabel(){

	unsigned int i = 0;
	int row,col;
	int step = 5;
	int label = 0;
	Element element;
	pixel_index_all_.clear();
	pixel_label_all_.clear();

	int num_class = Global()->num_class;
	num_pixel_class = new int [num_class]; // without background
	memset(num_pixel_class,0,(num_class)*sizeof(int));

	int counter = -1;
	//FILE* f = fopen("l.txt","w");
	for( i = 0; i < data_.size(); i ++ ){
		if(data_[i].image.empty() || data_[i].label.empty() ) continue;
		data_[i].pixelLabel.clear();
		data_[i].pixel.clear();
		for(row = 0; row < data_[i].image.rows; row = row + step){
			for(col = 0; col < data_[i].image.cols; col = col + step){
				label = (int)(data_[i].label.at<uchar>(row,col));
				//fprintf(f,"%d ",label);
				if( i < num_positive_image ){
					if(label ==0) continue;
					    counter ++;
						data_[i].pixelLabel.push_back(label-1);
						//cout<<label-1<<endl;
						element.x = col;
						element.y = row;
						element.idx = i;
						element.cnt = counter;
						num_pixel_class[label-1]++;
						data_[i].pixel.push_back(element);
				}
			}
			//fprintf(f,"\n");
		}
		pixel_index_all_.insert(pixel_index_all_.end(), data_[i].pixel.begin(),data_[i].pixel.end() );
		pixel_label_all_.insert(pixel_label_all_.end(), data_[i].pixelLabel.begin(), data_[i].pixelLabel.end());
	}
	//fclose(f);
	for(i = 0; i < Global()->num_class; i ++ ){
		printf("%d ", num_pixel_class[i]);
	}
	printf("\n");

}

void Data::LoadFileFromFolder(){
	char szFind[MAX_PATH];
	char fullPath[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	Configuration* p = Global();
	strcpy(szFind,p->positive_image_path);
	strcat(szFind,"\\*.*");
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return;
	while(TRUE)
	{
		if(FindFileData.dwFileAttributes
			&FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!='.')
			{

			}
		}
		else
		{
			    DataNode data_node_;
				sprintf(fullPath,"%s\\%s",p->positive_image_path, FindFileData.cFileName);
				strcpy(data_node_.imageName,fullPath);
				sprintf(fullPath,"%s\\%s",p->label_path, FindFileData.cFileName);
				strcpy(data_node_.labelName,fullPath);

				data_.push_back(data_node_);
				//cout << data_node_.imageName <<endl;

		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);
}


void Data::resampling(){
	
	



}


void Data::LoadMap(char* file){
	FILE* f = fopen(file,"r");
	fscanf(f,"%d",&numChar_);
	string str;
	map_.clear();
	char cha[2];
	for(short i = 0; i < numChar_; i++ ){
	   fscanf(f,"%s ",&cha);
	   str = cha;
	   map_.insert(map<string,   short>::value_type(str,i+1));
	}
	fclose(f);
}

void Data::generateResponseMap(){

	Response response;
	for(unsigned int i = 0; i < data_.size(); i ++ ){
		//printf("%d\n",i);
		//response.sobel(data_[i].image, data_[i].response);
		if(data_[i].image.empty()) continue;
		data_[i].image.copyTo(data_[i].response);
	}
}

void Data::saveAllPixel(char* file){
	FILE* f = fopen(file,"w");
	for(int i = 0; i < pixel_index_all_.size(); i ++ ){
		fprintf(f,"%s %d %d %d %d\n", data_[pixel_index_all_[i].idx].imageName,
		   pixel_index_all_[i].idx,pixel_index_all_[i].y, pixel_index_all_[i].x, 
		   pixel_label_all_[i]);
	}


	fclose(f);
}

void Data::saveLabel(char* file){
	FILE* f = fopen(file,"w");
	for(int i = 0; i < pixel_index_all_.size(); i ++ ){
		fprintf(f,"%d\n",  pixel_label_all_[i]);
	}
	fclose(f);

}

void Data::saveFileName(char* file){
    FILE* f = fopen(file,"w");
	fprintf(f,"%d\n",data_.size());
	for(int i = 0; i < data_.size(); i ++ ){
		fprintf(f,"%s\n", data_[i].imageName);
	}
	fclose(f);
}