#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <vector>
#include <queue> 

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

double xGoal = 0.0; 

double yGoal = 0.0; 



void goalMessageReceived(const geometry_msgs::Pose &p){
	xGoal = p.position.x; 
	yGoal = p.position.y; 

	return; 
}
int main(int argc,char **argv) {

	ros::init(argc,argv,"movearoundwithfeedback");
	ros::NodeHandle nh;
	 
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>ac("move_base",true);

	ros::Subscriber sub = nh.subscribe("/newgoal", 1000, &goalMessageReceived);
	ROS_INFO_STREAM("Waiting for server to be available...");
	while (!ac.waitForServer()) {
	}
	ROS_INFO_STREAM("done!");

	

	move_base_msgs::MoveBaseGoal goal;

		// not running through this iteratively, keeps reupdating goal when it should send it to the goal and wait!!! then send it to the next goal. 
	

	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	goal.target_pose.pose.position.x = xGoal;
	goal.target_pose.pose.position.y = yGoal; 
	goal.target_pose.pose.orientation.w = 0.1;
	
	ac.sendGoal(goal); 

	ac.waitForResult(ros::Duration(10.0)); 
	

	while(ros::ok()){
		if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
			ROS_INFO_STREAM("Current State: " << ac.getState().toString().c_str()); 
			ROS_INFO_STREAM("Goal reached"); 

				ROS_INFO_STREAM("Waiting for server to be available...");
			while (!ac.waitForServer()) {
			}
			ROS_INFO_STREAM("done!");
			goal.target_pose.pose.position.x = xGoal; 
			goal.target_pose.pose.position.y = yGoal; 
			ac.sendGoal(goal); 
			ac.waitForResult(ros::Duration(10.0)); 
		}
		else if(ac.getState() == actionlib::SimpleClientGoalState::ABORTED){
			ROS_INFO_STREAM("Current State: " << ac.getState().toString().c_str()); 
			break; 
		}
		

		ros::spinOnce(); 
	}
	

	
	return 0;    
}
