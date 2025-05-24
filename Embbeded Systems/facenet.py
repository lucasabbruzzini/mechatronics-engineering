from keras import backend as K

K.set_image_data_format('channels_first')
import glob
from fr_utils import *
from inception_blocks_v2 import *
import rospy
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
from std_msgs.msg import Bool
import os

PADDING = 50
ready_to_detect_identity = True

FRmodel = faceRecoModel(input_shape=(3, 96, 96))


def triplet_loss(y_true, y_pred, alpha=0.3):
    """
    Implementation of the triplet loss as defined by formula (3)

    Arguments:
    y_pred -- python list containing three objects:
            anchor -- the encodings for the anchor images, of shape (None, 128)
            positive -- the encodings for the positive images, of shape (None, 128)
            negative -- the encodings for the negative images, of shape (None, 128)

    Returns:
    loss -- real number, value of the loss
    """

    anchor, positive, negative = y_pred[0], y_pred[1], y_pred[2]

    # Step 1: Compute the (encoding) distance between the anchor and the positive, you will need to sum over axis=-1
    pos_dist = tf.reduce_sum(tf.square(tf.subtract(anchor, positive)), axis=-1)
    # Step 2: Compute the (encoding) distance between the anchor and the negative, you will need to sum over axis=-1
    neg_dist = tf.reduce_sum(tf.square(tf.subtract(anchor, negative)), axis=-1)
    # Step 3: subtract the two previous distances and add alpha.
    basic_loss = tf.add(tf.subtract(pos_dist, neg_dist), alpha)
    # Step 4: Take the maximum of basic_loss and 0.0. Sum over the training examples.
    loss = tf.reduce_sum(tf.maximum(basic_loss, 0.0))

    return loss


FRmodel.compile(optimizer='adam', loss=triplet_loss, metrics=['accuracy'])
load_weights_from_FaceNet(FRmodel)


def prepare_database():
    database = {}

    # load all the images of individuals to recognize into the database
    for file in glob.glob("images/*"):
        identity = os.path.splitext(os.path.basename(file))[0]
        database[identity] = img_path_to_encoding(file, FRmodel)

    return database


database = prepare_database()


class FaceNet:
    def __init__(self):
        os.system('clear')
        rospy.Subscriber('image_raw', Image, self.callback, queue_size=1)
        self.pub = rospy.Publisher('led', Bool, queue_size=1)
        self.bridge = CvBridge()
        self._show = False
        self._show_result = False
        self.image = None
        self.result = None
        self.ids = None
        self.ready = False
        self.database = None
        self.timer = rospy.Timer(rospy.Duration(nsecs=100), self.show)
        self.startup_timer = None
        self.pub.publish(True)
        print('ready to run')

    def callback(self, data):
        self.image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        self._show = True
        self.webcam_face_recognizer()
        # print(self.ids)
        self.welcome_users()

    def show(self, event):
        cv2.waitKey(1)
        if self._show:
            cv2.imshow('imagem', self.image)
        if self._show_result:
            cv2.imshow('imagem', self.result)

    def webcam_face_recognizer(self):
        """
        Runs a loop that extracts images from the computer's webcam and determines whether or not
        it contains the face of a person in our database.

        If it contains a face, an audio message will be played welcoming the user.
        If not, the program will process the next frame from the webcam
        """

        face_cascade = cv2.CascadeClassifier(
            '/home/jhonas/sistemas/src/face_rec/haarcascade_frontalface_default.xml')

        frame = self.image
        # We do not want to detect a new identity while the program is in the process of identifying another person
        self.result, self.ids = self.process_frame(self.image, frame, face_cascade)
        self._show_result = True

    def process_frame(self, img, frame, face_cascade):
        """
        Determine whether the current frame contains the faces of people from our database
        """
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, 1.3, 5)

        # Loop through all the faces detected and determine whether or not they are in the database
        identities = []
        for (x, y, w, h) in faces:
            x1 = x - PADDING
            y1 = y - PADDING
            x2 = x + w + PADDING
            y2 = y + h + PADDING

            img = cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)

            identity = self.find_identity(frame, x1, y1, x2, y2)

            if identity is not None:
                identities.append(identity)

        # if identities != []:
        #     cv2.imwrite('example.png', img)
        #
        #     ready_to_detect_identity = False
        #
        #     # essas linhas nao sao necessarias
        #     # pool = Pool(processes=1)
        #     # # We run this as a separate process so that the camera feedback does not freeze
        #     # pool.apply_async(welcome_users, [identities])
        return img, identities

    def find_identity(self, frame, x1, y1, x2, y2):
        """
        Determine whether the face contained within the bounding box exists in our database

        x1,y1_____________
        |                 |
        |                 |
        |_________________x2,y2

        """
        height, width, channels = frame.shape
        # The padding is necessary since the OpenCV face detector creates the bounding box around the face and not the head
        part_image = frame[max(0, y1):min(height, y2),
                     max(0, x1):min(width, x2)]

        return self.who_is_it(part_image, FRmodel)

    def who_is_it(self, image, model):
        """
        Implements face recognition for the happy house by finding who is the person on the image_path image.

        Arguments:
        image_path -- path to an image
        database -- database containing image encodings along with the name of the person on the image
        model -- your Inception model instance in Keras

        Returns:
        min_dist -- the minimum distance between image_path encoding and the encodings from the database
        identity -- string, the name prediction for the person on image_path
        """
        encoding = img_to_encoding(image, model)

        min_dist = 100
        identity = None

        # Loop over the database dictionary's names and encodings.
        for (name, db_enc) in database.items():

            # Compute L2 distance between the target "encoding" and the current "emb" from the database.
            dist = np.linalg.norm(db_enc - encoding)

            # If this distance is less than the min_dist, then set min_dist to dist, and identity to name
            if dist < min_dist:
                print('distance for %s is %s' % (name, dist))
                min_dist = dist
                identity = name
        if min_dist > 1:
            return None
        else:
            return str(identity)

    def welcome_users(self):
        message = Bool()
        print(self.ids)
        if 'jhonas' in self.ids or 'lucas' in self.ids or 'dutra' in self.ids:
            message.data = True
        else:
            message.data = False
            print('Alerta de desconhecido!!!')
        if self.ids != []:
            print('Sejam bem vindos')
            if 'dutra' in self.ids:
                print('Dutra')
            if 'jhonas' in self.ids:
                print('Jhonas')
            if 'lucas' in self.ids:
                print('Lucas')
            if 'ronaldo' in self.ids:
                print('Ronaldo')
        self.pub.publish(message)
