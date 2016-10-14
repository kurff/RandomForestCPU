#pragma once
#define MAX_PATH 260
class Configuration{
public:
	Configuration(){
	
	}

	~Configuration(){
	
	}

public:
	void LoadConfiguration(char* file);

public:
	int max_depth;
	int num_class;
	double threshold_entropy;
	int num_samples_node;
	int dimension_feature;
	int max_probe_offset;
	char positive_image_path[MAX_PATH];
	char label_path[MAX_PATH];
	char map_path[MAX_PATH];
	char save_tree_path[MAX_PATH];
};

Configuration* Global();