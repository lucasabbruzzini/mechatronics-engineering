#include <camera/camera.hpp>

int main(int argc, char** argv)
{
	ros::init(argc, argv, "camera");
	ros::NodeHandle nh;
	Camera cam(nh);
	ros::spin();
}
