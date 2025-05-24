#!/usr/bin/env python

import rospy

# import src.facenet
from src.facenet import FaceNet

if __name__ == "__main__":
    rospy.init_node('face_rec', anonymous=True)
    fn = FaceNet()
    try:
        rospy.spin()
    except rospy.ROSInterruptException:
        print('Ros failure')
