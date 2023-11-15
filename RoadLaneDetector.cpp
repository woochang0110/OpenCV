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
		���/����� ������ ������ ���� �ش�Ǵ� ������ ���͸��Ѵ�.
	*/
	Mat output;
	UMat img_hsv, img_combine;
	UMat white_mask, white_image;
	UMat yellow_mask, yellow_image;
	img_frame.copyTo(output);

	//���� ���� ����(lower~upper) 
	Scalar lower_white = Scalar(200, 200, 200); //��� ���� (RGB ����)
	Scalar upper_white = Scalar(255, 255, 255);
	Scalar lower_yellow = Scalar(10, 100, 100); //����� ���� (HSV ����)
	Scalar upper_yellow = Scalar(40, 255, 255);

	//BGR ��� ���͸�
	inRange(output, lower_white, upper_white, white_mask);//output, �� ������ white ���Ѱ��� ���Ѱ��� ���� ���� �ִ� ������ white_mask�� ����
	bitwise_and(output, output, white_image, white_mask);//output, �� ������ white_mask���� and ���ذ� white_image

	cvtColor(output, img_hsv, COLOR_BGR2HSV);//BGR�� output, �� ������ HSV�� ��ȯ�Ѱ� img_hsv

	//HSV ����� ���͸�
	inRange(img_hsv, lower_yellow, upper_yellow, yellow_mask);//HSV��ȯ�� img_hsv�� yellow ���Ѱ��� ���Ѱ��� ���� ���� �ִ� ���� yellow_mask�� �����
	bitwise_and(output, output, yellow_image, yellow_mask);//output, �� ������ yellow_mask ���� and ���ذ� yellow_image

	//�� ������ ��ģ��.
	addWeighted(white_image, 1.0, yellow_image, 1.0, 0.0, output);//white_image�� yellow image�� ��ģ ���� output
	return output;//Mat return
}


Mat RoadLaneDetector::limit_region(Mat img_edges) {
	/*
		���� ������ �����ڸ��� �����ǵ��� ����ŷ�Ѵ�.
		���� ������ �����ڸ��� ǥ�õǴ� ���� ������ ��ȯ�Ѵ�.
	*/
	int width = img_edges.cols;//input ���� cols
	int height = img_edges.rows;//input ���� rows

	Mat output;
	Mat mask = Mat::zeros(height, width, CV_8UC1);//mask�� Mat::zeros(height, width, CV_8UC1) ��ȣ �ȿ� �ش��ϴ� ��ŭ 0���� ä����

	//���� ���� ���� ���
	Point points[4]{
		Point((width * (1 - trap_bottom_width)) / 2, height),
		Point((width * (1 - trap_top_width)) / 2, height - height * trap_height),
		Point(width - (width * (1 - trap_top_width)) / 2, height - height * trap_height),
		Point(width - (width * (1 - trap_bottom_width)) / 2, height)
	};

	//�������� ���ǵ� �ٰ��� ������ ������ ä�� �׸���.
	fillConvexPoly(mask, points, 4, Scalar(255, 0, 0));

	//����� ��� ���� edges �̹����� mask�� ���Ѵ�.
	bitwise_and(img_edges, mask, output);
	return output;
}

vector<Vec4i> RoadLaneDetector::houghLines(Mat img_mask) {//���, Vec4i�� ���� ������(x,y),����(x,y) �� 4���� int�� ���� ���͸� ��Ÿ���µ� �����.
	/*
		���ɿ������� ����ŷ �� �̹������� ��� ���� �����Ͽ� ��ȯ
	*/
	vector<Vec4i> line;

	//Ȯ������ ������ȯ ���� ���� �Լ� 
	HoughLinesP(img_mask, line, 1/*��� ���� ����rho��*/, CV_PI / 180/*��� ���� ����theta��*/, 20, 20, 30);	//void HoughLinesP( InputArray image, OutputArray lines,double rho=1, double theta=CV_PI / 180, int threshold=20,double minLineLength = 10, double maxLineGap = 20 );
	/*
	��� ���⿡�� ������ �Ϸ��� rho �� 1�� ����ϰ�, theta �� PI / 180 �� ����ϸ� �ȴ�.  threshold �� �ּ� ���� Ƚ���� ����ڰ� ������ �����ذ��鼭 ã�ƾ���.

	@param rho: �ȼ����� �Ÿ� ����? Distance resolution of the accumulator in pixels.
	@param theta: ������ �������� ǥ�� Angle resolution of the accumulator in radians.
	@param threshold: �ּ� ���� Ƚ�� %Accumulator threshold parameter. Only those lines are returned that get enough votes ( \f$>\texttt{threshold}\f$ ).
	@param minLineLength: �� �� ���� ���� �ּ� ���� Minimum line length. Line segments shorter than that are rejected.
	@param maxLineGap: �� �� ���� ���� �ִ� ����Maximum allowed gap between points on the same line to link them.
	
	*/
	
	return line;
}

vector<vector<Vec4i>> RoadLaneDetector::separateLine(Mat img_edges, vector<Vec4i> lines) {
	/*
		����� ��� ������ȯ �������� ���� ���� �����Ѵ�.
		���� ����� �뷫���� ��ġ�� ���� �¿�� �з��Ѵ�.
	*/

	vector<vector<Vec4i>> output(2);
	Point ini, fini;
	vector<double> slopes;
	vector<Vec4i> selected_lines, left_lines, right_lines;
	double slope_thresh = 0.5;

	//����� �������� ���⸦ ���
	for (int i = 0; i < lines.size(); i++) {
		Vec4i line = lines[i];
		ini = Point(line[0], line[1]);
		fini = Point(line[2], line[3]);

		double slope = (static_cast<double>(fini.y) - static_cast<double>(ini.y))
			/ (static_cast<double>(fini.x) - static_cast<double>(ini.x) + 0.00001);//����

		//���Ⱑ �ʹ� ������ ���� ����
		if (abs(slope) > slope_thresh) {
			slopes.push_back(slope);
			selected_lines.push_back(line);
		}
	}

	//������ �¿� ������ �з�
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
		���� ȸ�͸� ���� �¿� ���� ������ ���� ������ ���� ã�´�.
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
			//�־��� contour�� ����ȭ�� ���� ����
			fitLine(right_pts, right_line, DIST_L2, 0, 0.01, 0.01);

			right_m = right_line[1] / right_line[0];  //����
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
			//�־��� contour�� ����ȭ�� ���� ����
			fitLine(left_pts, left_line, DIST_L2, 0, 0.01, 0.01);

			left_m = left_line[1] / left_line[0];  //����
			left_b = Point(left_line[2], left_line[3]);
		}
	}

	//�¿� �� ������ �� ���� ����Ѵ�.
	//y = m*x + b  --> x = (y-b) / m
	int ini_y = img_input.rows;
	int fin_y = 470;

	double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
	double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;

	double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
	double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;
	//����,�� y��ǥ�� �� �� ��� ����
	output[0] = Point(right_ini_x, ini_y);
	output[1] = Point(right_fin_x, fin_y);
	output[2] = Point(left_ini_x, ini_y);
	output[3] = Point(left_fin_x, fin_y);

	return output;
}

string RoadLaneDetector::predictDir() {
	/*
		�� ������ �����ϴ� ����(������� ��)�� �߽������κ���
		���ʿ� �ִ��� �����ʿ� �ִ����� ��������� �����Ѵ�.
	*/

	string output;
	double vx;
	double thres_vp = 10;

	//�� ������ �����ϴ� ���� ���
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
		�¿� ������ ���� �ϴ� ���� �ٰ����� �����ϰ� ���� ä���.
		�¿� ������ ���� ������ �׸���.
		���� ���� ���� �ؽ�Ʈ�� ���� ����Ѵ�.
	*/
	//input���� ������ lane�� regression�� ���� ���� ���� ��
	//0: ���� �� ������, 1: ���� �� ���� /  2: ���� �� ������,  3: ���� �� ����


	vector<Point> poly_points;
	Mat output;

	img_input.copyTo(output);
	poly_points.push_back(lane[2]);
	poly_points.push_back(lane[0]);
	poly_points.push_back(lane[1]);
	poly_points.push_back(lane[3]);

	fillConvexPoly(output, poly_points/*points*/, Scalar(0, 230, 30)/*color*/, LINE_AA/*lineType*/, 0);  //�ٰ��� �� ä���
	//fillConvexPoly(InputOutputArray img, InputArray points,const Scalar& color, int lineType = LINE_8,int shift = 0);
	addWeighted(output, 0.3, img_input, 0.7, 0, img_input);  //���� ���ϱ� img_input=img_input*0.7+output*0.3

	//�¿� ���� �� �׸���
	line(img_input, lane[0], lane[1], Scalar(0, 255, 255), 5, LINE_AA);
	line(img_input, lane[2], lane[3], Scalar(0, 255, 255), 5, LINE_AA);

	//���� ���� ���� �ؽ�Ʈ�� ���� ���
	putText(img_input, dir, Point(520, 100), FONT_HERSHEY_PLAIN, 3, Scalar(255, 255, 255), 3, LINE_AA);

	return img_input;
}
