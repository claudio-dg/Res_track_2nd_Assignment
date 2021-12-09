#include "ros/ros.h"
//for my service ChangeVel.srv
#include "second_assignment/ChangeVel.h"
//for my service UpdateVel.srv
#include "second_assignment/UpdateVel.h"
//for /base_scan topic
#include "sensor_msgs/LaserScan.h" 

//global definitions so that they can be used outside the main aswell

//define client for my services
ros::ServiceClient client1;
ros::ServiceClient client2;
second_assignment::ChangeVel change_vel;
second_assignment::UpdateVel up_vel;

char input = 'z';

//CALLBACK for the /base_scan Topic (used for avoiding an infinite while loop in the main that could not be interrupted with CTRL+C)
/* ***************************************************************************/
void myCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
//print possible choices
 printf("--- \n PRESS 'r' to reset the robot to the starting position \n PRESS 's' to stop the robot   \n PRESS 'i' to increase velocity  \n PRESS 'd' to decrease velocity \n--- \n");
 scanf(" %c", &input);
 system("clear");
 
 //fill the request field with the input received via keyboard
 change_vel.request.input = input;
 //wait for the existence of the service and then call it
 client1.waitForExistence(); 
 client1.call(change_vel);
 
 float resp = change_vel.response.change_value;
 //put the obtained value in the request of updateVal service
 up_vel.request.value = resp;
 //then call the service to actually change the speed
 client2.waitForExistence(); 
 client2.call(up_vel);

}
/* ***************************************************************************/
 
 
int main (int argc, char **argv)
{
// Initialize the node, setup the NodeHandle for handling the communication with the ROS system 
ros::init(argc, argv, "console");
ros::NodeHandle nh;

// Define the subscriber to robot's lasers
ros::Subscriber sub = nh.subscribe("/base_scan", 1, myCallback);

//define the client of my service Changecel.srv
client1 = nh.serviceClient<second_assignment::ChangeVel>("/changevel"); 

//define the client of my service UpdateVel.srv
client2 = nh.serviceClient<second_assignment::UpdateVel>("/updatevel"); 

ros::spin();
return 0;
}
