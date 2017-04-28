#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
void OccupancyGridMsgReceived(const nav_msgs::OccupancyGrid &o){

	ROS_INFO_STREAM("Data length: " << (sizeof(o.data) / sizeof(o.data[0])) << "\n"
					<< "origin: " << o.info.origin.position.x << "\n");  
	
	return; 
}

int main(int argc, char** argv){

	ros::init(argc, argv, "test_costmap"); 

	ros::NodeHandle nh; 

	ros::Subscriber sub = nh.subscribe("/move_base/global/costmap/costmap", 1000, &OccupancyGridMsgReceived); 
	
	ros::spinOnce(); 

	return 0; 
}
