#include <iostream>
#include <vector>
using namespace std; 

class GridCells{
public: 
	double width; 
	double height; 
	double x; 
	double y; 
	bool visited; 

	GridCells(){
		width = 0.0; 
		height = 0.0; 
		x = 0.0; 
		y = 0.0; 
		visited = false; 
	}

	GridCells(int size){
		width = 100.0 / (double)(size); 
		height = 100.0 / (double)(size); 
		x = 0.0; 
		y = 0.0; 
		visited = false; 
	}

	
}; 

void setupGrid(vector < vector<GridCells> > & G, int grids){



	for(int i = 0; i < grids; ++i){
		for(int j = 0; j < grids; ++j){
			G.at(i).at(j).width = 100.0 / (double)(grids); 
			G.at(i).at(j).height = 100.0 / (double)(grids); 
			G.at(i).at(j).x = (-50 + (G.at(i).at(j).width * j)) + (100.0/(grids * 2.0)); 
			G.at(i).at(j).y = (50 - (i * G.at(i).at(j).height)) - (100.0 / (2.0 * grids));
		}
	}

	return; 
}

int main(){

	int matrixSize = 8; 
	vector< vector<GridCells > > Grids(matrixSize, vector<GridCells>(matrixSize)); 

	setupGrid(Grids, matrixSize); 

	for(int i = 0; i < matrixSize; ++i){

		for(int j = 0; j < matrixSize; ++j){
			cout << "x, y: " << "( " << Grids.at(i).at(j).x << ", " << Grids.at(i).at(j).y << " )" << endl;
		}
	}



	return 0; 
}

