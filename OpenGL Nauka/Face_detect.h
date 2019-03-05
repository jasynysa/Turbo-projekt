#pragma once
class Face_detect
{
public:
	Face_detect(cv::VideoCapture cap);

	void data_load(float *x, float *y, float *z, cv::VideoCapture);

	~Face_detect();

private:
	int scale;

	float data_x;
	float data_y;

	cv::Mat image;

	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eyes_cascade;

	std::vector<cv::Rect> faces;
	std::vector<cv::Rect> eyes;

};

