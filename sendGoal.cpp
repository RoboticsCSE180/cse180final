#include<ros/ros.h>
#include<geometry_msgs/Pose.h>
#include<queue>
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
			G.at(i).at(j).x = ((-50 + (G.at(i).at(j).width * j)) + (100.0/(grids * 2.0))) * 0.1; 
			G.at(i).at(j).y = ((50 - (i * G.at(i).at(j).height)) - (100.0 / (2.0 * grids))) * 0.1;
		}
	}

	return; 
}

int main(int argc, char** argv){

	ros::init(argc, argv, "send_goal");
	ros::NodeHandle nh; 

	ros::Publisher pub = nh.advertise<geometry_msgs::Pose>("/newgoal", 1000); 

	int matrixSize = 8; 
    
	vector< vector<GridCells > > Grids(matrixSize, vector<GridCells>(matrixSize)); 

	setupGrid(Grids, matrixSize);

	queue<GridCells> queueOfGridCells; 

	for(int i = 0; i < matrixSize; ++i){
		for(int j = 0; j < matrixSize; ++j){
			queueOfGridCells.push(Grids.at(i).at(j)); 
		}
	}

	geometry_msgs::Pose goalPose; 

	ros::Rate rate(2); 
	
	GridCells g = queueOfGridCells.front(); 
	queueOfGridCells.pop(); 

	while(ros::ok() && !queueOfGridCells.empty()){
		goalPose.position.x = g.x; 
		goalPose.position.y = g.y; 

		pub.publish(goalPose); 

		g = queueOfGridCells.front(); 
		queueOfGridCells.pop(); 

		ros::spinOnce(); 

		rate.sleep(); 
	}


	return 0; 
}
