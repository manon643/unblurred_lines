
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>



#include <iostream>
#include <stdio.h>
#include "cartoon.h"
using namespace cv;
using namespace std;

//hide the local functions in an anon namespace
namespace {
    int process(VideoCapture& capture) {
        string window_name = "video";
        //namedWindow(window_name, WINDOW_KEEPRATIO); //resizable window;
        //Mat frame = imread("../moi.jpg", CV_LOAD_IMAGE_COLOR);
        Mat frame;
        for (;;) {
            capture >> frame;
            if (frame.empty()){
                cout<<"non trouve"<<endl;
                break;
            }
            Mat c;
            //bilateralFilter(frame, c, 30, 5000, 5000);
            //imshow(window_name, frame);waitKey();
            c = cartoonifyImage(frame, false, false, 0);
    //        drawFaceStickFigure(c);
            imshow("c", c);
            char key = waitKey();
            //delay N millis, usually long enough to display and capture input
            if (key == 'q')
                 break;
        }
        return 0;
    }
}

int main(int ac, char** av) {
    // //try to open string, this will attempt to open it as a video file or image sequence
    VideoCapture capture(0);
    if (!capture.isOpened()) {
        return 1;
    }
    return process(capture);
}
