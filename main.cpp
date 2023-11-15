//main.cpp



#if 0
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "RoadLaneDetector.h"

int main()
{





	
	
	RoadLaneDetector roadLaneDetector;// Ŭ���� ��ü ����
	Mat img_frame, img_filter, img_edges, img_mask, img_lines, img_result;
	vector<Vec4i> lines;
	vector<vector<Vec4i> > separated_lines;
	vector<Point> lane;
	string dir;

	VideoCapture video("input.mp4");  //���� �ҷ�����

	if (!video.isOpened())
	{
		cout << "������ ������ �� �� �����ϴ�. \n" << endl;
		getchar();
		return -1;
	}

	video.read(img_frame);
	if (img_frame.empty()) return -1;

	VideoWriter writer;
	int codec = VideoWriter::fourcc('X', 'V', 'I', 'D');  //���ϴ� �ڵ� ����
	double fps = 10.0;  //������
	string filename = "./result.avi";  //��� ����

	writer.open(filename, codec, fps, img_frame.size(), CV_8UC3);
	if (!writer.isOpened()) {
		cout << "����� ���� ���� ������ �� �� �����ϴ�. \n";
		return -1;
	}

	video.read(img_frame);
	int cnt = 0;

	while (1) {
		//1. ���� ������ �о�´�.
		if (!video.read(img_frame)) break;

		//2. ���, ����� ���� ���� �ִ� �͸� ���͸��Ͽ� ���� �ĺ��� �����Ѵ�.
		img_filter = roadLaneDetector.filter_colors(img_frame);

		//3. ������ GrayScale ���� ��ȯ�Ѵ�.
		cvtColor(img_filter, img_filter, COLOR_BGR2GRAY);

		//4. Canny Edge Detection���� ������ ����. (���� ���Ÿ� ���� Gaussian ���͸��� ����)
		Canny(img_filter, img_edges, 50, 150);

		//5. �ڵ����� ������� �ٴڿ� �����ϴ� �������� �����ϱ� ���� ���� ������ ����
		img_mask = roadLaneDetector.limit_region(img_edges);

		//6. Hough ��ȯ���� ���������� ���� ������ ����
		lines = roadLaneDetector.houghLines(img_mask);

		if (lines.size() > 0) {
			//7. ������ ������������ �¿� ������ ���� ���ɼ��� �ִ� �����鸸 ���� �̾Ƽ� �¿� ���� ������ ����Ѵ�. 
			// ���� ȸ�͸� �Ͽ� ���� ������ ���� ã�´�.
			separated_lines = roadLaneDetector.separateLine(img_mask, lines);
			lane = roadLaneDetector.regression(separated_lines, img_frame);

			//8. ���� ���� ����
			dir = roadLaneDetector.predictDir();

			//9. ���� ���� ������ ������ �׸��� ���� �ٰ����� ������ ä���. ���� ���� ���� �ؽ�Ʈ�� ���� ���
			img_result = roadLaneDetector.drawLine(img_frame, lane, dir);
		}

		//10. ����� ������ ���Ϸ� ����. ĸ���Ͽ� ���� ����
		writer << img_result;
		if (cnt++ == 10)
			imwrite("img_result.jpg", img_result);  //ĸ���Ͽ� ���� ����

		//11. ��� ���� ���
		imshow("result", img_result);

		//esc Ű ����
		if (waitKey(1) == 27)
			break;
	}
	
	return 0;
}


#else

#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	// ���� ĸó �ʱ�ȭ
	cv::VideoCapture cap("input.mp4");

	if (!cap.isOpened()) {
		std::cerr << "Error opening video file." << std::endl;
		return -1;
	}

	while (true) {
		cv::Mat frame;
		cap >> frame;

		if (frame.empty()) {
			std::cerr << "End of video stream." << std::endl;
			break;
		}

		// �׷��̽����� ��ȯ
		cv::Mat gray;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// ��� ���� ����
		cv::Mat white_lane;
		cv::inRange(gray, 220, 255, white_lane);  // ��Ⱑ 200 �̻��� �κ��� ��� �������� ����

		// �ܰ��� ã��
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(white_lane, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		// ��� ���� �׸���
		cv::Mat result = frame.clone();
		cv::drawContours(result, contours, -1, cv::Scalar(0, 255, 0), 2);

		// ��� ǥ��
		cv::imshow("White Lane Detection", result);

		// Ű �Է��� ��ٸ�
		if (cv::waitKey(30) == 27)  // 27�� ESC Ű
			break;
	}

	// ���� ĸó ���� �� â �ݱ�
	cap.release();
	cv::destroyAllWindows();

	return 0;
}

#endif

