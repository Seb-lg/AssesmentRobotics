#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>
#include <iostream>
#include "../include/Cv.h"

using namespace std;
using namespace cv;
using namespace yarp::os;
using namespace yarp::sig;
//using namespace yarp::cv;
//using namespace cv{using std::vector;}

class FaceDetection {
public:
    FaceDetection();

    void detectAndDraw(ImageOf<PixelRgb>* point, Mat& img, CascadeClassifier& cascade, double scale);

    cv::vector<cv::Vec3f> detect_circle(Mat& image, double *xPos, double *yPos);

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