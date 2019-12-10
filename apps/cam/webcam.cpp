// Copyright 2019 [BVU CMSC491 class]

#include "opencv2/opencv.hpp"
#include <X11/Xlib.h>
using namespace cv;


#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"



struct FrameInfo {
  Mat *frame;
  unsigned int frameNum;
} typedef FrameInfo;


/*
*/
class Quad : public Operator {
 public:
  void processData(Data data) {
    FrameInfo *fInfo= (FrameInfo*)data.value;

    Mat &oldFrame = *(fInfo->frame);

    int rows = oldFrame.rows;
    int cols = oldFrame.cols;

    Mat *newFrame = new Mat(rows, cols, oldFrame.type());


    for(int y=0; y < rows; y+=2) {
      for(int x=0; x < cols; x+=2) {

        Vec3b pixel = oldFrame.at<Vec3b>(Point(x,y));

        newFrame->at<Vec3b>(Point(         x/2,          y/2)) = pixel;
        newFrame->at<Vec3b>(Point(cols/2 + x/2,          y/2)) = pixel;
        newFrame->at<Vec3b>(Point(         x/2, rows/2 + y/2)) = pixel;
        newFrame->at<Vec3b>(Point(cols/2 + x/2, rows/2 + y/2)) = pixel;
      }
    }

    delete fInfo->frame;
    fInfo->frame = newFrame;

    emit(Data(fInfo, sizeof(FrameInfo), data.timestamp));
  }
};



class HorizontalFlip : public Operator {
 public:
  void processData(Data data) {
    FrameInfo *fInfo= (FrameInfo*)data.value;

    Mat &oldFrame = *(fInfo->frame);

    int rows = oldFrame.rows;
    int cols = oldFrame.cols;

    Mat *newFrame = new Mat(rows, cols, oldFrame.type());


    for(int y=0; y < rows; y++) {
      for(int x=0; x < cols; x++) {
        newFrame->at<Vec3b>(Point(cols-x,y)) = oldFrame.at<Vec3b>(Point(x,y));
      }
    }

    delete fInfo->frame;
    fInfo->frame = newFrame;

    emit(Data(fInfo, sizeof(FrameInfo), data.timestamp));
  }
};


class NegativeCorners : public Operator {
 public:
  void processData(Data data) {
    FrameInfo *fInfo= (FrameInfo*)data.value;

    int rows = fInfo->frame->rows;
    int cols = fInfo->frame->cols;

    Mat &frame = *(fInfo->frame);

    // Top-Left
    for(int y=0; y < rows/2; y++) {
      for(int x=0; x < cols/2; x++) {
        Vec3b pixel = frame.at<Vec3b>(Point(x,y));
        pixel.val[0] = 255 - pixel.val[0];
        pixel.val[1] = 255 - pixel.val[1];
        pixel.val[2] = 255 - pixel.val[2];
        frame.at<Vec3b>(Point(x,y)) = pixel;
      }
    }

    // Bottom-right
    for(int y=rows/2; y < rows; y++) {
      for(int x=cols/2; x < cols; x++) {
        Vec3b pixel = frame.at<Vec3b>(Point(x,y));
        pixel.val[0] = 255 - pixel.val[0];
        pixel.val[1] = 255 - pixel.val[1];
        pixel.val[2] = 255 - pixel.val[2];
        frame.at<Vec3b>(Point(x,y)) = pixel;
      }
    }
    //std::cout << "Increment Frame#: " << fInfo->frameNum << std::endl;

    emit(Data(fInfo, sizeof(FrameInfo), data.timestamp));
  }
};

class OutputOp : public Operator {
 public:
  void processData(Data data) {
    FrameInfo *fInfo= (FrameInfo*)data.value;


    imshow("SPE Cam", *(fInfo->frame));
    //std::cout << "Show Frame#: " << fInfo->frameNum << std::endl;

    delete fInfo->frame;
  }
};

class FrameCapture : public InputSource {
  void generateData() {

    VideoCapture cap;
    // Open the camera. Index corresponds to: /dev/video[index]
    if(!cap.open(2)) {
      std::cout << "Failed to attach to webcam" << std::endl;
      exit(1);
    }

    unsigned int id = 0;
    bool keepCapturing = true;
    while (keepCapturing) {
      FrameInfo fInfo = { new Mat, ++id };
      //std::cout << "Capturing frame #: " << id << std::endl;

      cap >> *(fInfo.frame);
      if( fInfo.frame->empty() || waitKey(10) == 27 ) {
        delete fInfo.frame;
        keepCapturing = false;
      } else {
        emit(Data(&fInfo, sizeof(FrameInfo)));
      }
    }
  }
};

int main(int argc, char** argv) {

  XInitThreads();
  //gtk_init(&argc, &argv);

  std::cout << "SPE Starting up." << std::endl;

  FrameCapture inOp;
  HorizontalFlip hFlipOp;
  Quad quadOp;
  NegativeCorners negCornersOp;
  OutputOp outOp;

  StreamProcessingEngine spe;

  spe.addInputSource(&inOp, {&hFlipOp});
  spe.connectOperators(&hFlipOp, {&quadOp});
  spe.connectOperators(&quadOp, {&negCornersOp});
  spe.connectOperators(&negCornersOp, {&outOp});

  spe.run();

  std::cout << "SPE Finished." << std::endl;
  return 0;
}
