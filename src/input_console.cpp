#include "ros/ros.h"
//for my service ChangeVel.srv
#include "second_assignment/ChangeVel.h"
//for /base_scan topic
#include "sensor_msgs/LaserScan.h" 
//for my message variation.msg 
#include "second_assignment/Variation.h"


//global definitions so that they can be used outside the main aswell

//define a client for my ChangeVel service
ros::ServiceClient client;
second_assignment::ChangeVel change_vel;
char input = 'z';
//define a publisher for my Variation.msg
ros::Publisher my_pub;


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
 client.waitForExistence(); 
 client.call(change_vel);
 change_vel.response.change_value;
 
 //i publish the obtained value as a msg of my topic /variation
 //so that it can be read from controller.cpp and modify the current robot's speed
 second_assignment::Variation var;
 var.variation_val  = change_vel.response.change_value;
 my_pub.publish(var);
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
client = nh.serviceClient<second_assignment::ChangeVel>("/changevel"); 

//define the publisher on my /variation topic
my_pub = nh.advertise<second_assignment::Variation> ("/variation", 1); 

ros::spin();
return 0;
}
