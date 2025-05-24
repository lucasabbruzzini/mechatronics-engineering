import rospy
import time
from cv_bridge import CvBridge

import RPi.GPIO as GPIO
import cv2
from sensor_msgs.msgs import Image
from std_msgs.msgs import Bool


class Camera:
    def __init__(self):
        print('iniciando camera')
        self.pub = rospy.Publisher("image_raw", Image)
        rospy.Subscriber('lock', Bool, self.led)
        self.cap = cv2.VideoCapture(0)
        self.bridge = CvBridge()
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(24, GPIO.OUT)
        self.button_timer = rospy.Timer(rospy.Duration(nsecs=100), self.button)

    def pic(self):
        _, image = self.cap.read()
        self.pub.publish(self.bridge.cv2_to_imgmsg(image, "bgr8"))
        GPIO.output(24, True)

    @staticmethod
    def led(data):
        if data:
            GPIO.output(24, True)
        else:
            GPIO.output(24, False)

    def button(self, event):
        try:
            button_state = GPIO.input(23)
            if button_state == False:
                self.pic()
                print('Button pressed...')
                time.sleep(1)
                GPIO.output(24, False)
        except:
            GPIO.cleanup()


if __name__ == "__main__":
    cam = Camera()
    rospy.init_node('camera')
    try:
        rospy.spin()
    except rospy.ROSInterruptException():
        print('Camera failure')
