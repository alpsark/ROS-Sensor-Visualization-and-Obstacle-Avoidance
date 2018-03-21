# ROS-Sensor-Visualization-and-Obstacle-Avoidance
Visulation and obstacle avoidance using point cloud and laser scan data

cd ros_ws   
sudo apt-get install ros-kinetic-joy  
run "catkin_make" make sure there are no errors.  
edit ~/.bashrc, add "source /home/..../rosws/devel/setup.bash" to the end of the .bashrc file.  
close the terminal and reopen the terminal  
run roscore  
run vrep, open scenes/assignment2.ttt  
. ~/ros-ws/devel/setup.bash  
rosrun obstacle_avoidance listener    
rosrun obstacle_avoidance obstacle_avoidance   
