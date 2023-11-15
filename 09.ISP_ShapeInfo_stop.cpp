//stop image 실습을 위한 파일

#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../KCCImageNet/stop_img.png";
	cv::Mat src_color = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);
	
	cv::Mat src_color_HSV1;
	cvtColor(src_color, src_color_HSV1, COLOR_BGR2HSV);//BGR인 output, 즉 원본을 HSV로 변환한게 img_hsv



	cv::Mat src_color_HSV;
	cvtColor(src_color, src_color_HSV, COLOR_BGR2HSV);//BGR인 output, 즉 원본을 HSV로 변환한게 img_hsv
	size_t width = src_color.cols;
	size_t height = src_color.rows;
	/*이진화*/
	Scalar lower_red = Scalar(170, 190, 200); //빨강색 (HSV 범위)
	Scalar upper_red = Scalar(185, 225, 255);
	inRange(src_color_HSV, lower_red, upper_red, src_color_HSV);
	/*이진화*/





	
	RNG rng(12345);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src_color_HSV/*1. 캐니엣지 영상 2. 이진영상 둘 중 하나 넣을 수 있음*/, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	Mat drawing = Mat::zeros(src_color_HSV.size(), CV_8UC3);
	int object_contour_num=0;
	int carea = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, object_contour_num/*int contourIdx*/, color, 2/*thickness*/, LINE_8/*lineType*/, hierarchy, 0);


		/*
		void drawContours( InputOutputArray image, InputArrayOfArrays contours,
                              int contourIdx, const Scalar& color,
                              int thickness = 1, int lineType = LINE_8,
                              InputArray hierarchy = noArray(),
                              int maxLevel = INT_MAX, Point offset = Point() );
		
		
		*/



		if (carea < cv::contourArea(contours[i])) { carea = cv::contourArea(contours[i]); object_contour_num = i; }//가장 넓은 면적을 가진 contours 요소를 찾아내는 함수 object_contour_num에 해당 인덱스가 저장됨
	}
	
	int CoGx, CoGy;//center of Gravity X,Y
	CoGx = CoGy = 0;
	int accX = 0, accY = 0;
	int x_min = width, x_max = 0, y_min = height, y_max = 0;
	int length = contours[object_contour_num].size();
	for (size_t i = 0; i < length; i++)//size()는 요소의 갯수, capacity()는 할당된 메모리공간에서 몇개의 요소를 저장할 수 있는지를 나타냄
	{
		accX += contours[object_contour_num].at(i).x;
		accY += contours[object_contour_num].at(i).y;
		if (x_min > contours[object_contour_num].at(i).x)
			x_min = contours[object_contour_num].at(i).x;
		if (x_max < contours[object_contour_num].at(i).x)
			x_max = contours[object_contour_num].at(i).x;
		if (y_min > contours[object_contour_num].at(i).y)
			y_min = contours[object_contour_num].at(i).y;
		if (y_max < contours[object_contour_num].at(i).y)
			y_max = contours[object_contour_num].at(i).y;
	}
	CoGx = accX / length;
	CoGy = accY / length;



	/*표지판 내 밝기 평균값 구하기*/
	cv::Mat BGR_BIN_AND = Mat::zeros(cv::Size(width, height), CV_8UC3);
	bitwise_and(src_color, src_color, BGR_BIN_AND, src_color_HSV);//output, 즉 원본에 white_mask값을 and 해준게 white_image

	double b_mean_brightness = 0.0; //= cv::mean(BGR_BIN_AND)[0];
	double g_mean_brightness = 0.0;
	double r_mean_brightness = 0.0;
	int b_count = 0, g_count = 0, r_count = 0;

	for (size_t row = y_min; row < y_max; row++)
	{
		for (size_t col = x_min; col < x_max; col++)
		{
			int index = (row)*width + (col);//mono->각 배열에 접근 할 index를 만들어 주는거임.

			int index_B = (row)*width * 3 + (col * 3 + 0);//RGB->B 배열에 접근 할 index를 만들어 주는거임.
			int index_G = (row)*width * 3 + (col * 3 + 1);//RGB->G 배열에 접근 할 index를 만들어 주는거임.
			int index_R = (row)*width * 3 + (col * 3 + 2);//RGB->R 배열에 접근 할 index를 만들어 주는거임.

			if (BGR_BIN_AND.data[index_G] > 40 && BGR_BIN_AND.data[index_G] < 80) { g_mean_brightness += BGR_BIN_AND.data[index_G]; g_count++;}
			if (BGR_BIN_AND.data[index_R] > 200 && BGR_BIN_AND.data[index_R] < 255) { r_mean_brightness += BGR_BIN_AND.data[index_R]; r_count++;}
			if (BGR_BIN_AND.data[index_B] > 40 && BGR_BIN_AND.data[index_B] < 80) { b_mean_brightness += BGR_BIN_AND.data[index_B]; b_count++; }

		}
	}
	g_mean_brightness /= g_count;
	r_mean_brightness /= r_count;
	b_mean_brightness /= b_count;










		double area = contourArea(contours[object_contour_num]);
		RotatedRect rrt = minAreaRect(contours[object_contour_num]);
		double arcLen = arcLength(contours[object_contour_num], true);
		double radius = CoGx - x_min;

		/*string 화면 출력*/
		Point ptTxt = Point(rrt.boundingRect().x, rrt.boundingRect().y);
		string msg;
		msg = std::format("area = {:.1f}", area);
		putText(src_color, msg, Point(ptTxt.x+2*radius, ptTxt.y + 30 * 0), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 3);
		msg = std::format("length = {:.1f}", arcLen);
		putText(src_color, msg, Point(ptTxt.x + 2 * radius, ptTxt.y + 30 * 1), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 3);
		msg = std::format("x,y = {:.1f}, {:.1f}", rrt.center.x, rrt.center.y);													
		putText(src_color, msg, Point(ptTxt.x + 2 * radius, ptTxt.y + 30 * 2), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 3);
		msg = std::format("Radius = {:.1f}",radius);																			
		putText(src_color, msg, Point(ptTxt.x + 2 * radius, ptTxt.y + 30 * 3), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 3);
		msg = std::format("AvgBrightness =");																					
		putText(src_color, msg, Point(ptTxt.x + 2 * radius, ptTxt.y + 30 * 4), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 3);
		msg = std::format("({:.1f},{:.1f},{:.1f})", b_mean_brightness, g_mean_brightness, r_mean_brightness);
		putText(src_color, msg, Point(ptTxt.x + 2 * radius, ptTxt.y + 30 * 5), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 3);
		
			
			
		
		/*string 화면 출력*/
		//cv::rectangle(src_color, rrt.boundingRect().tl(), rrt.boundingRect().br(), CV_RGB(0, 0, 255));//도형 주위 사각형 그리기
		//cv::drawMarker(src_color, rrt.center, CV_RGB(255, 0, 0));//무게중심 그리기

		const int ptSz = 4;
		Point2f pt[ptSz];
		
		cv::circle(src_color/*image src*/, rrt.center/*center Point*/, radius/*radius*/, CV_RGB(0, 0, 255));
		




	return 1;
}