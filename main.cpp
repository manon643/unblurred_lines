
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>



#include <iostream>
#include <stdio.h>
#include "cartoon.h"
using namespace cv;
using namespace std;

RNG rng(12345);

//hide the local functions in an anon namespace
namespace {
    
    /// Global Variables
    const int alpha_slider_max = 100;
    int alpha_slider;
    double alpha;
    double beta;
    
    Ptr<BackgroundSubtractor> pMOG2;
    
    /// Matrices to store images
    Mat src1;
    Mat src2;
    Mat dst;
    Mat edges;
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    void on_trackbar( int, void*)
    {
        return;
    }
    
    int process(VideoCapture& capture) {
        //string window_name = "video";
        //namedWindow(window_name, WINDOW_KEEPRATIO); //resizable window;
        
        //Mat frame = imread("../moi.jpg", CV_LOAD_IMAGE_COLOR);
        
        alpha_slider = 10;
        
        /// Create Windows
        namedWindow("slider", 1);
        
        
        createTrackbar("TrackbarName", "slider", &alpha_slider, alpha_slider_max, on_trackbar );
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        Mat frame, precedentFrame;
        Mat precedentDrawing;
        Mat result_and, result_or;
        Mat mask;
        Mat diff_gray, diff_gray_inv;
        int erosion_size = 1;
        Mat element = getStructuringElement( MORPH_RECT,
                                            Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                            Point( erosion_size, erosion_size ) );
        Mat lap;
        
        capture>>precedentFrame;
        //pMOG2 = createBackgroundSubtractorMOG2();
        
        for (int j=0;j>=0;j++) {
            capture >> frame;
            
            if (frame.empty()){
                cout<<"non trouve"<<endl;
                break;
            }
            
            
            
            Mat bgModel;
            Mat fgModel;
            //Rect rect(10, 10, frame.cols-10, frame.rows-10);
            //grabCut(frame, mask, rect, bgModel, fgModel, 10, GC_INIT_WITH_RECT);
            
            //pMOG2->apply(frame, mask);
            //bitwise_and(frame, frame, frame, mask);
            
            Mat c;
            //bilateralFilter(frame, c, 30, 5000, 5000);
            //imshow(window_name, frame);waitKey();
            absdiff(frame, precedentFrame, diff_gray);
            cvtColor(diff_gray, diff_gray, CV_BGR2GRAY);
            threshold(diff_gray, diff_gray, 35, 255, THRESH_BINARY_INV);
            
            
            cvtColor(frame, edges, CV_BGR2GRAY);
            GaussianBlur(edges, edges, Size(7, 7), 1.5, 5);
            //Laplacian(edges, lap, CV_8U);
            Canny(edges, edges, 3*alpha_slider, alpha_slider);
            //threshold(lap*alpha_slider, lap, 125, 255, THRESH_BINARY);
            //cout<<alpha_slider<<endl;
            findContours( edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
            
            /// Draw contours
            Mat drawing = Mat::zeros( edges.size(), CV_8UC3 );
            for( int i = 0; i< contours.size(); i++ )
            {
                drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
            }
            
            if(j>0){
                /*bitwise_or(drawing, precedentDrawing, result_and, diff_gray);
                erode(result_and, result_and, element);
                dilate(result_and, result_and, element);
                bitwise_not(diff_gray, diff_gray_inv);
                bitwise_or(drawing, drawing, drawing, diff_gray_inv);
                bitwise_or(drawing, result_and, result_or);*/
                
                imshow("slider", drawing);
            }
            
    //        drawFaceStickFigure(c);
            
            char key = waitKey(10);
            //delay N millis, usually long enough to display and capture input
            if (key == 'q')
                 break;
            frame.copyTo(precedentFrame);
            //precedentDrawing = drawing;
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
