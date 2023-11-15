#pragma once
#include "Common.h"
//https://docs.opencv.org/3.4/da/d97/tutorial_threshold_inRange.html

using namespace cv;
const int max_value_H = 360 / 2;
const int max_value = 255;

int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
static void on_low_H_thresh_trackbar(int, void*)
{
    low_H = min(high_H - 1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void*)
{
    high_H = max(high_H, low_H + 1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void*)
{
    low_S = min(high_S - 1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void*)
{
    high_S = max(high_S, low_S + 1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void*)
{
    low_V = min(high_V - 1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void*)
{
    high_V = max(high_V, low_V + 1);
    setTrackbarPos("High V", window_detection_name, high_V);
}

/*//opencv library cvt 코드(BGR->HSV)
//HSV struct
typedef struct {
    double h; // Hue (색조) [0, 360]
    double s; // Saturation (채도) [0, 1]
    double v; // Value (명도) [0, 1]
} HSV;

// RGB를 HSV로 변환
HSV RGBtoHSV(uint8_t r, uint8_t g, uint8_t b) {
    HSV hsv;
    double min, max, delta;
    r /= 255.0;
    g /= 255.0;
    b /= 255.0;
    min = fmin(fmin(r, g), b);
    max = fmax(fmax(r, g), b);
    hsv.v = max;
    delta = max - min;
    if (max > 0.0) { hsv.s = delta / max; }
    else {
        hsv.s = 0.0;
        hsv.h = 0.0;
        return hsv;
    }
    if (r == max) { hsv.h = (g - b) / delta; }
    else if (g == max) {
        hsv.h = 2.0 + (b - r) / delta;
    }
    else {
        hsv.h = 4.0 + (r - g) / delta;
    }
    hsv.h *= 60.0;
    if (hsv.h < 0.0) {
        hsv.h += 360.0;
    }
    return hsv;
}
*/
int main(int argc, char* argv[])
{

    //VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
    namedWindow(window_capture_name);
    namedWindow(window_detection_name);
    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

    Mat frame, frame_HSV, frame_threshold;
    while (true) {

        //std::string fileName = "../thirdparty/opencv_480/sources/samples/data/hsv.jpg";
        std::string fileName = "../KCCImageNet/stop_img.png";
        cv::Mat frame = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);


        //cap >> frame;
        if (frame.empty())
        {
            break;
        }
        // Convert from BGR to HSV colorspace
        cvtColor(frame, frame_HSV, COLOR_BGR2HSV);  //opencv library cvt 코드(BGR->HSV)
        // Detect the object based on HSV Range Values
        low_H = 100, high_H = 120;
        low_S = 0, high_S = 30;
        low_V = 185, high_V = 230;
        inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);//threshold 내의 구간에 있으면 관심있는 객체(0 검은색)고 아니면 관심없다.(255흰색)

        // Show the frames
        imshow(window_capture_name, frame);

        imshow(window_detection_name, frame_threshold);

        char key = (char)waitKey(30);
        if (key == 'q' || key == 27)
        {
            break;
        }
    }
    return 0;
}