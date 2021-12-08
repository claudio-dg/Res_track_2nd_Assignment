#include "ros/ros.h"
//for /base_scan topic
#include "sensor_msgs/LaserScan.h" 
//for /cmd_vel topic
#include "geometry_msgs/Twist.h" 
//for my service ChangeVel.srv
#include "second_assignment/ChangeVel.h"
//for my message variation.msg 
#include "second_assignment/Variation.h"

#define STARTING_VEL 2

//global definitions so that they can be used outside the main aswell
ros::Publisher pub;
geometry_msgs::Twist my_vel; 
//array to work with lasers
float ranges_array[721]; 
//a variable which is updated from the input given in the input_console to increase/decrease speed
float variation = 0; 
float previous_vel = 0;

void Turn_right()
{
	/*
    	 Simple Function for making the robot turning right
    	 by modifying my_vel.linear.x AND my_vel.angular.z values.
    	*/
	my_vel.linear.x = 0.8; //andamento ottimo con linear 0.1 angular +-2
 	my_vel.angular.z = -2; //anche aumentanto linear a 1 va bene
 	//printf("going RIGHT");
 	 
}
void Turn_left()
{	
	/*
    	 Simple Function for making the robot turning left
    	 by modifying my_vel.linear.x AND my_vel.angular.z values.
    	*/
	my_vel.linear.x = 0.8;
 	my_vel.angular.z = 2;
 	//printf("going LEFT");
}
void Move_forward()
{
	/*
    	 Simple Function for making the robot move forward
    	 by modifying my_vel.linear.x AND setting my_vel.angular.z to 0.
    	 
    	 remark:  the variation obtained by console only effects the forward movement
    	*/
	my_vel.linear.x = STARTING_VEL + variation; 
	my_vel.angular.z = 0;
 	//printf("moving FORWARD"); 	
 	
}
float GetMinDistance(int min_index,int max_index, float ranges_array[])
{
	/*
    	 Function for detecting nearest wall in a certain direction.
    	 
    	 INPUT: ranges_array[] -> the array of distances received from LaserScan
    	       min/max_index -> values that allow to consider only portion of the array,
    	                        so that it only detects walls in a certain direction
    	                        
    	 OUTPUT: min_distance -> the min distance from a wall in the specified direction                  
    	*/
	float min_distance = 999;
	for(int i = min_index; i <= max_index; i++)
	{
		if(ranges_array[i] < min_distance)
			min_distance = ranges_array[i];
	}
	return min_distance;
}


//CALLBACK for the /base_scan Topic
/* ***************************************************************************/
void LasersCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
//put lasers_scan values into an array
 for(int i=0; i<=720;i++)
 {
  ranges_array[i] = msg->ranges[i]; 
 }
 
 //120 values correspond to about 30 degrees of vision (since 720 values correspond to 180      degrees)
 
 //get the min distance from a wall at the RIGHT of the robot
 float right_dist = GetMinDistance(20,120, ranges_array);
 
 //get the min distance from a wall at the LEFT of the robot
 float left_dist = GetMinDistance(600,700, ranges_array);
 
 //get the min distance forma a wall in FRONT of the robot
 float frontal_dist = GetMinDistance(300,420,ranges_array);


//if close to a FRONTAL wall
 if(frontal_dist<1.5) 
 {
 	//check if the next closest wall is at the RIGHT of the robot turn left and viceversa
 	if(right_dist<left_dist) 
 	 Turn_left();
 	else
 	 Turn_right();
 }
 //otherwise no walls in front of the robot -> it can move forward
 else 
 	Move_forward();
 	
 //publish the velocity on /cmd_vel topic
 pub.publish(my_vel); 
 //print current velocity only if it changes
 if(previous_vel != my_vel.linear.x){
 system("clear");
 printf("\n velocita attuale e'  %f  [variazione totale = %f]\n", my_vel.linear.x, variation );
 previous_vel = my_vel.linear.x;
 }
}
/* ***************************************************************************/


//CALLBACK for my own topic: "/variation"
/* ***************************************************************************/
void ChangeVelCallback(const second_assignment::Variation::ConstPtr& my_msg)
{
 
 variation = variation + my_msg->variation_val;
 
 //if vel is already zero OR a STOP is received set variation to -start_vel to stop the robot
 // and to avoid going bacwards
 if(variation < -STARTING_VEL or my_msg->variation_val == -1 )
 {
  variation = -STARTING_VEL; //the robot stands still
 }
}

/* ***************************************************************************/



int main (int argc, char **argv)
{
// Initialize the node, setup the NodeHandle for handling the communication with the ROS system 
ros::init(argc, argv, "controller"); 
ros::NodeHandle nh;
// Define the subscriber to robot's lasers
ros::Subscriber sub = nh.subscribe("/base_scan", 1, LasersCallback); 
// Define the publisher for robot's velocity 
pub = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
// Define the subscriber to my msg variation
ros::Subscriber my_sub = nh.subscribe("/variation", 1, ChangeVelCallback);
ros::spin();
return 0;
}

