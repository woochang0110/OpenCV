//RoadLaneDetector.cpp
//  https://codingwell.tistory.com/60

#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "RoadLaneDetector.h"
#include <iostream>
#include <string>
#include <vector>


Mat RoadLaneDetector::filter_colors(Mat img_frame) {
	/*
		흰색/노란색 색상의 범위를 정해 해당되는 차선을 필터링한다.
	*/
	Mat output;
	UMat img_hsv, img_combine;
	UMat white_mask, white_image;
	UMat yellow_mask, yellow_image;
	img_frame.copyTo(output);

	//차선 색깔 범위(lower~upper) 
	Scalar lower_white = Scalar(200, 200, 200); //흰색 차선 (RGB 범위)
	Scalar upper_white = Scalar(255, 255, 255);
	Scalar lower_yellow = Scalar(10, 100, 100); //노란색 차선 (HSV 범위)
	Scalar upper_yellow = Scalar(40, 255, 255);

	//BGR 흰색 필터링
	inRange(output, lower_white, upper_white, white_mask);//output, 즉 원본의 white 하한값과 상한값의 범위 내에 있는 값들을 white_mask로 만듬
	bitwise_and(output, output, white_image, white_mask);//output, 즉 원본에 white_mask값을 and 해준게 white_image

	cvtColor(output, img_hsv, COLOR_BGR2HSV);//BGR인 output, 즉 원본을 HSV로 변환한게 img_hsv

	//HSV 노란색 필터링
	inRange(img_hsv, lower_yellow, upper_yellow, yellow_mask);//HSV변환한 img_hsv의 yellow 하한값과 상한값의 범위 내에 있는 값을 yellow_mask로 만들고
	bitwise_and(output, output, yellow_image, yellow_mask);//output, 즉 원본에 yellow_mask 값을 and 해준게 yellow_image

	//두 영상을 합친다.
	addWeighted(white_image, 1.0, yellow_image, 1.0, 0.0, output);//white_image와 yellow image를 합친 것이 output
	return output;//Mat return
}


Mat RoadLaneDetector::limit_region(Mat img_edges) {
	/*
		관심 영역의 가장자리만 감지되도록 마스킹한다.
		관심 영역의 가장자리만 표시되는 이진 영상을 반환한다.
	*/
	int width = img_edges.cols;//input 받은 cols
	int height = img_edges.rows;//input 받은 rows

	Mat output;
	Mat mask = Mat::zeros(height, width, CV_8UC1);//mask에 Mat::zeros(height, width, CV_8UC1) 괄호 안에 해당하는 만큼 0으로 채워줌

	//관심 영역 정점 계산
	Point points[4]{
		Point((width * (1 - trap_bottom_width)) / 2, height),
		Point((width * (1 - trap_top_width)) / 2, height - height * trap_height),
		Point(width - (width * (1 - trap_top_width)) / 2, height - height * trap_height),
		Point(width - (width * (1 - trap_bottom_width)) / 2, height)
	};

	//정점으로 정의된 다각형 내부의 색상을 채워 그린다.
	fillConvexPoly(mask, points, 4, Scalar(255, 0, 0));

	//결과를 얻기 위해 edges 이미지와 mask를 곱한다.
	bitwise_and(img_edges, mask, output);
	return output;
}

vector<Vec4i> RoadLaneDetector::houghLines(Mat img_mask) {//통상, Vec4i는 선의 시작점(x,y),끝점(x,y) 총 4개의 int를 갖는 벡터를 나타내는데 사용함.
	/*
		관심영역으로 마스킹 된 이미지에서 모든 선을 추출하여 반환
	*/
	vector<Vec4i> line;

	//확률적용 허프변환 직선 검출 함수 
	HoughLinesP(img_mask, line, 1/*모든 방향 검출rho값*/, CV_PI / 180/*모든 방향 검출theta값*/, 20, 20, 30);	//void HoughLinesP( InputArray image, OutputArray lines,double rho=1, double theta=CV_PI / 180, int threshold=20,double minLineLength = 10, double maxLineGap = 20 );
	/*
	모든 방향에서 검출을 하려면 rho 는 1을 사용하고, theta 는 PI / 180 을 사용하면 된다.  threshold 는 최소 교차 횟수로 사용자가 여러번 조정해가면서 찾아야함.

	@param rho: 픽셀간의 거리 단위? Distance resolution of the accumulator in pixels.
	@param theta: 각도는 라디안으로 표시 Angle resolution of the accumulator in radians.
	@param threshold: 최소 교차 횟수 %Accumulator threshold parameter. Only those lines are returned that get enough votes ( \f$>\texttt{threshold}\f$ ).
	@param minLineLength: 두 점 사이 선의 최소 길이 Minimum line length. Line segments shorter than that are rejected.
	@param maxLineGap: 두 점 사이 선의 최대 길이Maximum allowed gap between points on the same line to link them.
	
	*/
	
	return line;
}

vector<vector<Vec4i>> RoadLaneDetector::separateLine(Mat img_edges, vector<Vec4i> lines) {
	/*
		검출된 모든 허프변환 직선들을 기울기 별로 정렬한다.
		선을 기울기와 대략적인 위치에 따라 좌우로 분류한다.
	*/

	vector<vector<Vec4i>> output(2);
	Point ini, fini;
	vector<double> slopes;
	vector<Vec4i> selected_lines, left_lines, right_lines;
	double slope_thresh = 0.5;

	//검출된 직선들의 기울기를 계산
	for (int i = 0; i < lines.size(); i++) {
		Vec4i line = lines[i];
		ini = Point(line[0], line[1]);
		fini = Point(line[2], line[3]);

		double slope = (static_cast<double>(fini.y) - static_cast<double>(ini.y))
			/ (static_cast<double>(fini.x) - static_cast<double>(ini.x) + 0.00001);//기울기

		//기울기가 너무 수평인 선은 제외
		if (abs(slope) > slope_thresh) {
			slopes.push_back(slope);
			selected_lines.push_back(line);
		}
	}

	//선들을 좌우 선으로 분류
	img_center = static_cast<double>((img_edges.cols / 2));
	for (int i = 0; i < selected_lines.size(); i++) {
		ini = Point(selected_lines[i][0], selected_lines[i][1]);
		fini = Point(selected_lines[i][2], selected_lines[i][3]);

		if (slopes[i] > 0 && fini.x > img_center && ini.x > img_center) {
			right_lines.push_back(selected_lines[i]);
			right_detect = true;
		}
		else if (slopes[i] < 0 && fini.x < img_center && ini.x < img_center) {
			left_lines.push_back(selected_lines[i]);
			left_detect = true;
		}
	}

	output[0] = right_lines;
	output[1] = left_lines;
	return output;
}

vector<Point> RoadLaneDetector::regression(vector<vector<Vec4i>> separatedLines, Mat img_input) {
	/*
		선형 회귀를 통해 좌우 차선 각각의 가장 적합한 선을 찾는다.
	*/
	vector<Point> output(4);
	Point ini, fini;
	Point ini2, fini2;
	Vec4d left_line, right_line;
	vector<Point> left_pts, right_pts;

	if (right_detect) {
		for (auto i : separatedLines[0]) {
			ini = Point(i[0], i[1]);
			fini = Point(i[2], i[3]);

			right_pts.push_back(ini);
			right_pts.push_back(fini);
		}

		if (right_pts.size() > 0) {
			//주어진 contour에 최적화된 직선 추출
			fitLine(right_pts, right_line, DIST_L2, 0, 0.01, 0.01);

			right_m = right_line[1] / right_line[0];  //기울기
			right_b = Point(right_line[2], right_line[3]);
		}
	}

	if (left_detect) {
		for (auto j : separatedLines[1]) {
			ini2 = Point(j[0], j[1]);
			fini2 = Point(j[2], j[3]);

			left_pts.push_back(ini2);
			left_pts.push_back(fini2);
		}

		if (left_pts.size() > 0) {
			//주어진 contour에 최적화된 직선 추출
			fitLine(left_pts, left_line, DIST_L2, 0, 0.01, 0.01);

			left_m = left_line[1] / left_line[0];  //기울기
			left_b = Point(left_line[2], left_line[3]);
		}
	}

	//좌우 선 각각의 두 점을 계산한다.
	//y = m*x + b  --> x = (y-b) / m
	int ini_y = img_input.rows;
	int fin_y = 470;

	double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
	double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;

	double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
	double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;
	//시작,끝 y좌표는 좌 우 모두 동일
	output[0] = Point(right_ini_x, ini_y);
	output[1] = Point(right_fin_x, fin_y);
	output[2] = Point(left_ini_x, ini_y);
	output[3] = Point(left_fin_x, fin_y);

	return output;
}

string RoadLaneDetector::predictDir() {
	/*
		두 차선이 교차하는 지점(사라지는 점)이 중심점으로부터
		왼쪽에 있는지 오른쪽에 있는지로 진행방향을 예측한다.
	*/

	string output;
	double vx;
	double thres_vp = 10;

	//두 차선이 교차하는 지점 계산
	vx = static_cast<double>(((right_m * right_b.x) - (left_m * left_b.x) - right_b.y + left_b.y) / (right_m - left_m));

	if (vx < img_center - thres_vp)
		output = "Left Turn";
	else if (vx > img_center + thres_vp)
		output = "Right Turn";
	else if (vx >= (img_center - thres_vp) && vx <= (img_center + thres_vp))
		output = "Straight";

	return output;
}

Mat RoadLaneDetector::drawLine(Mat img_input, vector<Point> lane, string dir) {
	/*
		좌우 차선을 경계로 하는 내부 다각형을 투명하게 색을 채운다.
		좌우 차선을 영상에 선으로 그린다.
		예측 진행 방향 텍스트를 영상에 출력한다.
	*/
	//input으로 들어오는 lane은 regression을 거쳐 리턴 받은 값
	//0: 우측 선 시작점, 1: 우측 선 끝점 /  2: 좌측 선 시작점,  3: 좌측 선 끝점


	vector<Point> poly_points;
	Mat output;

	img_input.copyTo(output);
	poly_points.push_back(lane[2]);
	poly_points.push_back(lane[0]);
	poly_points.push_back(lane[1]);
	poly_points.push_back(lane[3]);

	fillConvexPoly(output, poly_points/*points*/, Scalar(0, 230, 30)/*color*/, LINE_AA/*lineType*/, 0);  //다각형 색 채우기
	//fillConvexPoly(InputOutputArray img, InputArray points,const Scalar& color, int lineType = LINE_8,int shift = 0);
	addWeighted(output, 0.3, img_input, 0.7, 0, img_input);  //영상 합하기 img_input=img_input*0.7+output*0.3

	//좌우 차선 선 그리기
	line(img_input, lane[0], lane[1], Scalar(0, 255, 255), 5, LINE_AA);
	line(img_input, lane[2], lane[3], Scalar(0, 255, 255), 5, LINE_AA);

	//예측 진행 방향 텍스트를 영상에 출력
	putText(img_input, dir, Point(520, 100), FONT_HERSHEY_PLAIN, 3, Scalar(255, 255, 255), 3, LINE_AA);

	return img_input;
}
