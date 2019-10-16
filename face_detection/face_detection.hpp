#pragma once

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>
#include <iostream>

using namespace std;
using namespace cv;
using namespace yarp::os;
using namespace yarp::sig;

class FaceDetection {
public:
    FaceDetection();

    void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale);

private:
    CascadeClassifier cascade_face;

    Mat frame;
    Mat frameCopy;
    Mat image;

    //path to our haarcascade classifier xml document
    String haarcascade_face = "./face_detection/haarcascade_face.xml";
    double scale = 3.0;

    BufferedPort<ImageOf<PixelRgb> > facePort;
};