#include "Selector.h"

void Selector::selector(){
	//_selected.resize(2*_num);
	Random rand;
	for(int i = 0; i < 2*_num; i ++ ){
		_selected[i].x = rand.Next(-_N, _N);
		_selected[i].y = rand.Next(-_N, _N);
	}
}

void Selector::loadSelector(char* file){
	FILE* f = fopen(file,"r");
	int num = 0;
	fscanf(f,"%d\n",&num);
	//_selected.resize(2*num);
	for(int i = 0; i < 2*num; i ++ ){
	   fscanf(f,"%d %d\n",&(_selected[i].x), &(_selected[i].y));
	}
	fclose(f);



}

void Selector::saveSelector(char* file){
	FILE* f = fopen(file,"w");
	fprintf(f,"%d\n",_num);
	for(int i = 0; i < 2*_num; i ++ ){
	   fprintf(f,"%d %d\n",_selected[i].x, _selected[i].y);
	}
	fclose(f);
}




