#include "face_detection.hpp"
#include "../EventLibrary.hpp"

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
//#include "../include/Cv.h"

using namespace yarp::os;
using namespace yarp::sig;

FaceDetection::FaceDetection() {
  //load cascade file for face detection
  //If the load function returns false, set a break point before this call 
  //and see the contents inside "haarcascade_face".  
  if(!cascade_face.load(haarcascade_face)){
    cerr << "ERROR: Could not load classifier cascade" << endl;
  } else {
      facePort.open("/facePort/out");
      event.addEvent<ImageOf<PixelRgb>*>("image right eye", [this](ImageOf<PixelRgb>* image){
        IplImage* iplImg = static_cast<IplImage*>(image->getIplImage());
        frame = iplImg;
        if( frame.empty() )
            return;
        if( iplImg->origin == IPL_ORIGIN_TL )
            frame.copyTo( frameCopy );
        else
            flip( frame, frameCopy, 0 );
        detectAndDraw( frameCopy, cascade_face, scale);
    });
  }
}

void FaceDetection::detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale) {
     int i = 0;
  double t = 0;
  vector<Rect> faces;
  const static Scalar colors[] =  { CV_RGB(0,0,255),
                                    CV_RGB(0,128,255),
                                    CV_RGB(0,255,255),
                                    CV_RGB(0,255,0),
                                    CV_RGB(255,128,0),
                                    CV_RGB(255,255,0),
                                    CV_RGB(255,0,0),
                                    CV_RGB(255,0,255)} ;
  
  // resizes and applies grey on imgae for methods work easier 

  Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
  
  cvtColor( img, gray, CV_BGR2GRAY );
  resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
  equalizeHist( smallImg, smallImg );
  
  t = (double)cvGetTickCount();
  cascade.detectMultiScale( smallImg, faces,
                            1.1, 2, 0
                              |CV_HAAR_FIND_BIGGEST_OBJECT
                              |CV_HAAR_DO_ROUGH_SEARCH
                              |CV_HAAR_SCALE_IMAGE
                              ,
                              Size(30, 30) );
  t = (double)cvGetTickCount() - t;
  // code will do a rough search and detect always the biggest face.
  
  //Draw Circle on the output image
  for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
  {
    Mat smallImgROI;
    vector<Rect> nestedObjects;
    Point center;
    Scalar color = colors[i%8];
    int radius;
    center.x = cvRound((r->x + r->width*0.5)*scale);
    center.y = cvRound((r->y + r->height*0.5)*scale);
    radius = cvRound((r->width + r->height)*0.25*scale);
    circle( img, center, radius, color, 3, 8, 0 );
    
    event.fire("look at position", center.x, center.y);           

  } 
  //ImageOf<PixelRgb> &tmp = facePort.prepare();
  //auto ref = yarp::cv::fromCvMat<ImageOf<PixelRgb>>(img);
  //facePort.write();
}