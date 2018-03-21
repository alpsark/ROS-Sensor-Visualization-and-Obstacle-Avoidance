#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_listener.h>

#define PI 3.1415926535897932384626433f
#define MAX_TURN_SPD 0.42359879
#define MIN_TURN_SPD 0.05

class ObstacleAvoidance
{

    ros::Subscriber scanSubs;
    ros::Publisher velPub;
    tf::TransformListener transformListener;
    tf::StampedTransform robotTransform;
    ros::NodeHandle nh;

public:
    ObstacleAvoidance()
    {
        scanSubs=nh.subscribe("/scan",10,&ObstacleAvoidance::laserCallback,this);
        velPub=nh.advertise<geometry_msgs::Twist> ("/cmd_vel",1);
        transformListener.waitForTransform("odom","base_link",
                ros::Time::now(),ros::Duration(3.0));
    }

    void laserCallback(const sensor_msgs::LaserScan& input_scan)
    {
        //inspect laser data
        std::cout<<"ranges size: "<<input_scan.ranges.size()<<std::endl;
        std::cout<<"max range: "<<input_scan.range_max<<std::endl;
        std::cout<<"min range: "<<input_scan.range_min<<std::endl;
        std::cout<<"min angle: "<<input_scan.angle_min<<std::endl;
        std::cout<<"max angle: "<<input_scan.angle_max<<std::endl;
        std::cout<<"min increment: "<<input_scan.angle_increment<<std::endl;


		float minrange = 5;
		float index_max = 0 ;
		float index_min = 500 ;
		float crash_imminent = 0.5;
		float x ;
		float z ;
        for(int i=0; i<input_scan.ranges.size(); i++)
        {
            // is current scan valid?
            if(input_scan.ranges[i]<input_scan.range_max&&input_scan.ranges[i]
                    >input_scan.range_min)
            {
				float crash_limit = 0.25 ;
				if(input_scan.ranges[i] > crash_limit) {
                	//std::cout<<input_scan.ranges[i]<<" ";
            	/****
            	 * You are expected to detect obstacles here by inspecting laser scans
            	 */
				
				if(input_scan.ranges[i] < minrange)
					minrange = input_scan.ranges[i];
				}
				if( index_min > i) { index_min = i ;}
				if( index_max < i) { index_max = i ;}
            }
            else
            {
                //std::cout<<"0 ";
            }
        }
			std::cout << "min " << index_min << " max " << index_max << " size " <<  input_scan.ranges.size() << std::endl ;
			if(minrange > crash_imminent){
				x = 4;
				std::cout << "go straight" << std::endl ;
				z = 0;
			}else{
				x = 0.5;
				std::cout << "crashing" << std::endl;
				if(index_max < input_scan.ranges.size()/2){//reading index from right to left
					z = -0.1 ;//left
				}else if(index_min > input_scan.ranges.size()/2) {
					z = 0.1 ; //right			
				}else{
					if(index_max  > (input_scan.ranges.size()-index_min)){
						z = -0.1 ; //left	
					}else{
						z = 0.1 ; //right
					}		
				}							
			}
		
        //inspect robot pose
        transformListener.lookupTransform("world","base_link",
                ros::Time(0),robotTransform);
        float robotX=robotTransform.getOrigin().x();
        float robotY=robotTransform.getOrigin().y();
        float robotYaw=tf::getYaw(robotTransform.getRotation());
        std::cout<<"robot pose: x,y,yaw: "<<robotX<<","<<robotY<<","<<robotYaw               <<std::endl;

        /**
         * You are expected to set a movecmd according to the position of the obstacle you have detected
	 * If you need you can also use the position of the robot.
         */
        geometry_msgs::Twist moveCmd;
        moveCmd.linear.x=x;
        moveCmd.linear.y=0;
        moveCmd.angular.z=z;
        velPub.publish(moveCmd);
    }

    /**
     * Angle normalization method. When you add or substract some value from an angle
     * you need to make sure that the new value is between -pi and pi
     */
    float normalizeRad(float rad)
    {
        while(rad>PI)
        {
            rad=rad-2*PI;
        }
        while(rad<-PI)
        {
            rad=rad+2*PI;
        }
        return rad;
    }
};

int main(int argc,char **argv)
{
    std::cout<<"Obstacle Avoidance Start..."<<std::endl;
    ros::init(argc,argv,"obstacle_avoidance");
    ObstacleAvoidance tp;
    ros::spin();
    printf("Obstacle Avoidance Finish...");
}
