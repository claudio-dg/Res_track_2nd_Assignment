#include "ros/ros.h"
//for my service ChangeVel.srv
#include "second_assignment/ChangeVel.h"
//for being able to call reset_position service
#include "std_srvs/Empty.h"

//global definitions so that they can be used outside the main aswell
std_srvs::Empty my_reset; 
 
 
//CALLBACK for my /change_vel Topic, which is called from the input_console after having received a command through keyboard.
/* ***************************************************************************/
bool Mycallback (second_assignment::ChangeVel::Request &req, second_assignment::ChangeVel::Response &res)
{

char given_input = req.input;


//look at the input recieved
switch(given_input)
{ 
 case 'r' : //reset -> call /reset_positions service
 	ros::service::call("/reset_positions", my_reset);
 	ROS_INFO("RESET RECEIVED");
 	res.change_value = 0; 
 	break;
 	
 case 's' : //stop -> stop the robot
 	ROS_INFO("STOP RECEIVED");
 	res.change_value = -1; 
 	break;	
 	
 case'i' :  //increase -> set the response of the service as +0.5
 	ROS_INFO("INCREASE RECEIVED");
 	res.change_value = +0.5; 
 	
 	break;
 	
 case'd' : //decrease -> set the response of the service as -0.5
 	ROS_INFO("DECREASE RECEIVED");
 	res.change_value = -0.5;
 	break;
 	
 default :
 	ROS_INFO("WRONG COMMAND");
 	res.change_value = 0;
 	break;


}
return true;
}
/* ***************************************************************************/



int main(int argc, char **argv)
{
//initalizing the node and the Service
ros::init(argc, argv, "my_server");
ros::NodeHandle n;
ros::ServiceServer service= n.advertiseService("/changevel", Mycallback);



ros::spin();



return 0;

}
