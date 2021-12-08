Research Track 2^nd Assignment
================================

This repository contains the result of my personal work for the second Assignment of the course.
The goal of this assignment is to obtain a simulation in which a robot:

1. constantly drives around Monza's circuit.
2. provides a node which interacts with the user to increase/decrease the speed and reset the position of the robot.

To do this we had to use ROS for controlling the robot and C++ as programming language.



Table of contents
----------------------

* [Setup-and-nodes-Introduction](#setup-and-nodes-Introduction)
* [Flowchart and behaviour description](#flowchart-and-behaviour-description)
* [Code explanation](#code-explanation)


## Setup-and-nodes-Introduction

This repository Contains all the useful files to run the script that i produced for this assignment.
To try it, it is sufficient to clone this repository: 

```bash
$ git clone https://github.com/claudio-dg/Res_track_2nd_Assignment.git
```

and then type the following command in the terminal to simultaneously launch all the neceassary nodes through the **"launchFile"**:

```bash
$ roslaunch second_assignment starter.launch

```
Scripts contained in the first three directories (i.e **games**, **sr** and **tests**) and in the **run.py** and **License.md** files have not been modified, 
so the code that i produced is all included in the **assignment.py** file.



## Flowchart and behaviour description

Right here you can see a Flowchart produced to briefly describe the ideas i had to obtain the required behaviour of the robot: everything is based on an infinite while loop, during which the robot takes different decisions basing on different possible variables.



<!-- ![my_flowchart](https://github.com/claudio-dg/Research_Track-Assignments/blob/main/my_FlowChart_Res_track_1.png?raw=true) --> 

<p align="center">
<img src="https://github.com/claudio-dg/Research_Track-Assignments/blob/main/images/flowchart.png?raw=true" width="900" />
<p>
 
 

 ### Behaviour description  : ### 
 
 As previously said, everything is based on an infinite while loop, within which the robot checks possible conditions and acts consequently:
 1. First of all it checks whether it is already holding a token or not: 
 - if so it leaves it behind himself and ckecks again,
 otherwise it starts searching for a token by scanning walls and silver tokens in a range with reduced angulation with respect to the 360° degrees given by deafult
 2. then, before moving, it detects if it is close to a wall (a golden box) :
 - if so, it avoids the wall in different ways, based on the fact that wall might be in front of it or at one oh his sides.
 3. Otherwise the robot can move without risking collisions, so it checks if it sees a silver token inside of its range :
 - if it doesn't, it decides to move forward a bit , in order to maybe get a better position with respect to a token.
 4. If it sees a token, instead, it looks for a golden obstacle placed in between:
 - if it finds one, it again decides to move forward to get a better position with respect to the token.
 5. But if it sees no obstacles it makes one last check: it checks if it is close enough to the detected token, 
 - if it is, it simply grabs the box, otherwise he moves a little bit(*) towards it
 
 In the end the loop restarts from point 1. 
 
 (*) REMARK : in this situation the robot does not simply move forward until he grabs the box, but it moves "one step at the time" and returns the control to point 1 after every step: this because i wanted to reduce possible mistakes caused by an error in the obstacles detection.
 This simply means that, when the robot moves towards the token, it is still capable of scanning the area around him and to avoid obstacles; so for example, if it didn't detect an obstacle at the beginning of the movement, but while reaching the token it is getting dangerously close to a wall at its left, it is capable of seeing that and "takes a safier path".

 
 
 
 ## Code explanation
 
 For the code inserted in **assignment.py** file i produced different functions in order to create the robot's behaviour previously described.
 
 Firstly i splitted the given function to turn the robot in two separates functions, just for making the code more readable:
 
 
```bash
def my_turn_left(speed, seconds):
def my_turn_right(speed, seconds):
```
 
 Then i also created two different functions for detecting nearest golden or silver box: this time i splitted them (instead of making just one) for two reasons, that is again making the code easily readable and to use different detecting ranges for the boxes. These functions, in fact, simply detect tokens thate are visible within a certain range in front of the robot, so that it only considers walls or tokens if it is actually moving towards them, while it ignores those who are behind it.
 The resulting degrees used for golden/silver ranges were obtained esperimentally, and they represent the best ones i found for the behaviour of my robot. 
 
 
 ```bash
def find_SILVER_token(): 
    dist=100
    silver_range = 45
    for token in R.see():
        if token.dist < dist and token.info.marker_type == MARKER_TOKEN_SILVER and -silver_range < token.rot_y < silver_range
            dist=token.dist
	    rot_y=token.rot_y
	    
    if dist==100:
	return -1, -1
	
    else:
   	return dist, rot_y
```
															       
```bash
def find_GOLDEN_token():
    dist=100
    golden_range = 35
    for token in R.see():
        if token.dist < dist and token.info.marker_type == MARKER_TOKEN_GOLD and  -golden_range < token.rot_y < golden_range     
            dist=token.dist
	    rot_y=token.rot_y
	    
    if dist==100:
	return -1, -1
	
    else:
   	return dist, rot_y
```
 The picture here below intuitively shows these features (i.e. silver range and golden range, with the first one larger and including the golden one) seen from the robot's point of view, in order to better understand what do these "ranges reduction" mean : note that no distance reduction has been applied, so theorically the robot is capable of seeing tokens within these ranges no matter how far they are. 
 
 
<p align="center">
<img src="https://github.com/claudio-dg/Research_Track-Assignments/blob/main/images/Ranges_picture.png?raw=true" width="500"/>
<p>

After that i made a function that sees if between the robot and the nearest silver token there are obstacles, by simply checking if, within the angulation of the nearest silver token detected (plus an error of +-20° degrees), it finds a golden token whose distance is inferior to the silver's one. So this function was created to allow the robot to understand if it can actually move towards the token or not, and the "error", considered in terms of the degrees, was added in order to take into account the fact that the distance, between a box and the robot, is considered with respect to their center, so by adding this quantity i mananaged to obtain better results in terms of detecting obstacles.
 
 
 ```bash
def no_gold_in_between():
	S_dist,S_rot = find_SILVER_token()
	no_between = 1
	for token in R.see():
		if token.info.marker_type == MARKER_TOKEN_GOLD and (S_rot-20)<token.rot_y<(S_rot+20) and token.dist<S_dist:
			no_between=0 
	return no_between
```
 
 To move the robot towards a detected token i exploited a function i produced for exercise3, that simply aligns the robot to the box by comparing the values returned by the "find_SILVER_token()" function (in terms of angle and distance) with two respective threshold values: thanks to this comparison the robot knows when it is directed towards the token (so it can move straight forward) and when it is close enough to grab it. On the other hand if the robot detects that the token is at its right/left, it rotates until they're not perfectly aligned.
 
```bash
 def move_towards_token(distance, angle):
	grabbed = 0
	if distance<d_th and grabbed == 0: 
		R.grab()
		grabbed = 1
							
	elif -a_th<angle<a_th:
		drive(25,0.1)
		
	elif angle<-a_th:
		my_turn_left(2,0.5)					
				
	elif angle>a_th:  
		my_turn_right(2,0.5)
		
	return grabbed
```
 
 
 To avoid walls instead i created a function that is called as soon as the robot detects that it is getting dangerously close to a wall (i.e. when the risulting distance of find_nearest_golden() function is inferior than 0.8)
 
  
```bash
 def avoid_obstacle(gold_angle):
	if  -17< gold_angle <17: 			
		 avoid_frontal_collision()			 			
			
	elif gold_angle < 0: 								 
		 my_turn_right(20,0.5) 	
			
	elif gold_angle > 0: 				 
		 my_turn_left(20,0.5)	
			
	drive(15,1)
	
```
 This function behaves in two different ways based on "where the wall is with respect to the robot":
 if the obstacle is on his Right/Left it simply makes the robot turn on the opposite way, while if it detects that the wall is in front of it (by looking at the degrees of the detected wall's angle) it calls the following function:
 
   
```bash
 def avoid_frontal_collision():										
	left_dist=100
	right_dist=100
	for gold_token in R.see():
		if gold_token.info.marker_type == MARKER_TOKEN_GOLD:
		
			if 70<gold_token.rot_y<100 and gold_token.dist<right_dist:
				right_dist = gold_token.dist
				
			elif -100<gold_token.rot_y<-70 and gold_token.dist<left_dist: 
				left_dist = gold_token.dist
				
	if left_dist > right_dist: 					
		 my_turn_left(25,1)					 
	else:    
		 my_turn_right(25,1)
```
 
 that allows to avoid the collision in a "smart" way: once in front of a wall it basically looks at the right and at the left of the robot(*), computing for each side the position of the nearest wall; then by simply comparing these two values the robot is capable of understanding which is the best direction to take, and it turns towards the side where the obstacle is more distant from itself.
	
 (*)To consider the obstacles in these position i simply used the R.see() method with another different range, so that the concept is similar to what i implemented before, but the ranges this time are from 70° to 100° (for the right side of the robot) and from -100° to -70° (for the left side).
 
	
 
 The core of the program is then included in another function which has the task of starting robot's Routine. This function simply includes a while loop (that only terminates when a silver token is picked up by the robot), as well as the call of the functions to detect nearest token/wall: through these it is therefore responsibile for checking the status of the robot  step-by-step, and for calling the functions i mentioned above when necessary. In terms of code this function is then a simple series of "if/elif/else statements" included in a while loop that check the conditions described in the flowchart and calls the function needed for every situation (e.g if the robot is close to a wall calls avoid_obstacle() and so on...).
 By grouping every function call in here i tried to make the code more readable and intuitive.
 
```bash
 def get_SILVER_token():
	grabbed = 0
	while(grabbed == 0):
		token_distance,token_angle = find_SILVER_token()
		gold_dist, gold_angle = find_GOLDEN_token()		
		
		if(gold_dist>0.8 and no_gold_in_between()): 
			if(token_distance==-1 and token_angle==-1): 		
				drive(20,0.1)		
	
			else:	 
				grabbed =  move_towards_token(token_distance, token_angle)

	        elif(gold_dist>0.8 and  no_gold_in_between()==0):
			drive(25,0.1)
				
		elif(gold_dist<0.8): 
			avoid_obstacle(gold_angle)

```
 
 In the end the last fuction i made is a very basic one just to make the code look more clean: it is simply called after having grabbed a token, and only makes the robot release it behind itself (calling R.release()) and turning back to the original direction.
 
 ```bash
 def release_token():
	my_turn_right(37,1.5)
	R.release()
	my_turn_left(37,1.5)
```
 
 Therefore the infinite loop on which my programe is based simply has the following structure:
 
 ```bash
while(1): 
 	
	get_SILVER_token()	
	release_token()
	
```
 * REMARK: within the 'assigment.py' file you'll find the whole code introduced in this README, along with futher explanations through comments in which, for example, i explain more in details the inputs and otputs of every function.
 There also are multiple "print()" present in the code, that allow to know "step by step" what the robot is doing during the simulation, by keeping the terminal opened while running the script.
I decided to remove the comments and the "prints()" from the bodies of the function reported in this README in order to avoid weighting too much its reading.
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
