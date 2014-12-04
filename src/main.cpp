
//http://docs.ros.org/api/sensor_msgs/html/msg/LaserScan.html
#include "ros/ros.h"
#include <stdlib.h>  
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>

#include <sstream>

#include <stdlib.h>  

/* modify these vallues for other frames */
#define WHEEL_DISTENCE 11.0
#define CORRECTION 1.0

ros::Publisher command_pub;

void chatterCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
	float linear = msg->linear.x;
	float angular = msg->angular.z;

	/* make shure the input is between -1 and 1 so the act simmular as the turtlebots */
	if( linear > 1.0) linear = 1.0;
	if( linear < -1.0) linear = -1.0;	
	
	if( angular > 1.0) angular = 1.0;
	if( angular < -1.0) angular = -1.0;	
	
	/* scale it back to the vallues the motor driver understands */
	linear *= 255;
	angular *= 255;
	
	float rightMotorSpeed = linear + (2.0/WHEEL_DISTENCE)*angular*CORRECTION;
	float leftMotorSpeed = linear - (2.0/WHEEL_DISTENCE)*angular*CORRECTION;
	
	/* example command DR100FS D = drive, R = right other option here is L, 100 =  0 < speed < 255, F = Forward other option is B for back, S is end of packige*/
	
	
	std::stringstream ssR;
	ssR << abs((int)rightMotorSpeed);
	
	std_msgs::String right;
	right.data = "D";
	right.data += "R";
	right.data += ssR.str();
	right.data +=(rightMotorSpeed >=0)?"F":"B";
	right.data += "S";
	
	command_pub.publish(right);
	
	
	std::stringstream ssL;
	ssL << abs((int)leftMotorSpeed);
	
	std_msgs::String left;
	left.data = "D";
	left.data += "R";
	left.data += ssL.str();
	left.data +=(leftMotorSpeed >=0)?"F":"B";
	left.data += "S";
	
	command_pub.publish(left);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "RSPiBot_Laser_Node", ros::init_options::AnonymousName);
	
	ros::NodeHandle n;
	
	command_pub = n.advertise<std_msgs::String>("/rspibot/aruinoCommands", 1000);
	
	//sensor_msgs::LaserScan
	
	ros::Subscriber sub = n.subscribe("/rspibot/cmd_vel", 1000, chatterCallback);
	
	ros::spin();
	return 0;
}
