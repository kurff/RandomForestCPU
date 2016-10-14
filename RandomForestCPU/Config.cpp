#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Configuration config;

Configuration* Global(){
   return & config;
}

void Configuration::LoadConfiguration(char* file){
    FILE* f = fopen(file,"r");
	if(f==NULL){
	   printf("Can not find the configuration file\n");
	   exit(0);
	}

	char parameter[MAX_PATH];
	char value[MAX_PATH];
	while(!feof(f)){
	  fscanf(f,"%s %s",&parameter,&value);
	  if(!strcmp(parameter,"max_depth")){
	     max_depth = atoi(value);
	  }else if(!strcmp(parameter,"num_class")){
	     num_class = atoi(value);
	  }else if(!strcmp(parameter,"dimension_feature")){
	     dimension_feature = atoi(value);
	  }else if(!strcmp(parameter,"max_probe_offset")){
		 max_probe_offset = atoi(value);
	  }else if(!strcmp(parameter,"positive_image_path")){
		  strcpy(positive_image_path,value);
	  }else if(!strcmp(parameter,"label_path")){
		  strcpy(label_path,value);
	  }else if(!strcmp(parameter,"map_path")){
	     strcpy(map_path,value); 
	  }else if(!strcmp(parameter,"threshold_entropy")){
	     threshold_entropy = atof(value);
	  }else if(!strcmp(parameter,"num_samples_node")){
	     num_samples_node = atoi(value);
	  }else if(!strcmp(parameter,"save_tree_path")){
	     strcpy(save_tree_path,value); 
	  }
	  else{
	     printf("load configuration error!\n");
		 exit(0);
	  }
	}

	fclose(f);
}