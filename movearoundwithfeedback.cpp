#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <sensor_msgs/LaserScan.h> 
#include <geometry_msgs/Twist.h>

#include <vector>
#include <queue> 

using namespace std; 


float angle=0.0;
float closest=0.0;
int closestIndex=0;

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
void scanMessageReceived (const sensor_msgs::LaserScan&msg) {
 ROS_INFO_STREAM( "Received scan . Finding closest obstacle" ) ; 
 closest=msg.ranges[0];

 closestIndex=0;
 for(int i=1; i<msg.ranges.size();i++){
    if(msg.ranges[i]<closest){
        closest=msg.ranges[i];
        closestIndex=i;
        angle=msg.angle_min+(float)(i/720*270);
    }
 } 
   ROS_INFO_STREAM( "Closest obstacle at distance (m) :" << closest ) ;
    ROS_INFO_STREAM("Index: "<<closestIndex);
    ROS_INFO_STREAM("Angle: "<<angle);
 }
 /*int main ( int argc , char ** argv ) {
 ros::init( argc , argv , "scanreader") ; 
  ros::NodeHandle nh; 
   ros::Subscriber subScan = nh.subscribe ( "/scan" ,1000 , &scanMessageReceived ) ; 
    ros::spin ();
    }
*/
double xGoal = 0.0; 

double yGoal = 0.0; 



/*void goalMessageReceived(const geometry_msgs::Pose &p){
	xGoal = p.position.x; 
	yGoal = p.position.y; 

	return; 
}*/
int main(int argc,char **argv) {

	ros::init(argc,argv,"movearoundwithfeedback");
	ros::NodeHandle nh;
	 ros::Subscriber subScan = nh.subscribe ( "/scan" ,1000 , &scanMessageReceived ) ; 
     ros::Publisher pub=nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1000);
     geometry_msgs::Twist mg;
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>ac("move_base",true);

	//ros::Subscriber sub = nh.subscribe("/newgoal", 1000, &goalMessageReceived);
	ROS_INFO_STREAM("Waiting for server to be available...");
	while (!ac.waitForServer()) {
	}
	ROS_INFO_STREAM("done!");
	
	int matrixSize = 8; 
	vector< vector<GridCells > > Grids(matrixSize, vector<GridCells>(matrixSize)); 

	setupGrid(Grids, matrixSize);

	queue<GridCells> queueOfGridCells; 

	for(int i = 0; i < matrixSize; ++i){
		for(int j = 0; j < matrixSize; ++j){
			queueOfGridCells.push(Grids.at(i).at(j)); 
		}
	}

	move_base_msgs::MoveBaseGoal goal;
	GridCells g = queueOfGridCells.front(); 
	queueOfGridCells.pop(); 

		// not running through this iteratively, keeps reupdating goal when it should send it to the goal and wait!!! then send it to the next goal. 
	

	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	goal.target_pose.pose.position.x = g.x;
	goal.target_pose.pose.position.y = g.y; 
	goal.target_pose.pose.orientation.w = 0.1;
	
	ac.sendGoal(goal); 

	ac.waitForResult(ros::Duration(30.0)); 
	

	while(ros::ok() && !queueOfGridCells.empty()){
		if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
			ROS_INFO_STREAM("Current State: " << ac.getState().toString().c_str()); 
			ROS_INFO_STREAM("Goal reached"); 

				ROS_INFO_STREAM("Waiting for server to be available...");
			while (!ac.waitForServer()) {
			}
			ROS_INFO_STREAM("done!");
	
			g = queueOfGridCells.front(); 
			queueOfGridCells.pop(); 
	           if(closest<0.5){

               }
			goal.target_pose.pose.position.x = g.x; 
			goal.target_pose.pose.position.y = g.y; 
			goal.target_pose.pose.orientation.w = 0.2; 
			
			ac.sendGoal(goal); 
	
			ac.waitForResult(ros::Duration(30.0)); 
		}
		else if(ac.getState() == actionlib::SimpleClientGoalState::ABORTED){
			ROS_INFO_STREAM("Current State: " << ac.getState().toString().c_str()); 
			break; 
		}
		
ros::spinOnce();
		
	}
	
	ros::shutdown(); 

	
	return 0;    
}
