#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>

void OccupancyGridMsgReceived(const nav_msgs::OccupancyGrid &o){
	
	int length = (sizeof(o.data) / sizeof(o.data[0])); 

	for(int i = 0; i < length; ++i){
		ROS_INFO("data: %d\n", o.data[i]); 
		ROS_INFO_STREAM("more info: " <<  o.info.width << "\n"
				<< "even more info: " << o.info.origin.position.x 
				<< "\neven even more info: " << o.info.origin.position.y); 
	}
	
	return; 
}

int main(int argc, char** argv){

	ros::init(argc, argv, "test_costmap"); 

	ros::NodeHandle nh; 

	ros::Subscriber sub = nh.subscribe("/move_base/local_costmap/costmap", 1000, &OccupancyGridMsgReceived); 
	
	ros::spin(); 

	return 0; 
}
