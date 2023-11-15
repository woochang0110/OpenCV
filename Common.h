#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <memory>
#include <fstream> // ofstream header
#include <format>

#define OPENCV_480
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#ifdef OPENCV_480
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
#endif // OPENCV_480

#ifdef _DEBUG
#pragma comment(lib,"opencv_world480d.lib")
#else	//RELEASE
#pragma comment(lib,"opencv_world480.lib")
#endif
using namespace std;
using namespace cv;