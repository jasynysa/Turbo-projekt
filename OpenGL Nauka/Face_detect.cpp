#include "pch.h"
#include "Face_detect.h"

Face_detect::Face_detect(cv::VideoCapture cap) //camera(~VideoCapture cap(o)) + project folder > (face.xml eye.xml) !!!;
{
	data_x = 0;
	data_y = 0;
	scale = 3; //cropp faktor;

	if (!face_cascade.load("haarcascade_frontalface_alt2.xml"))
		std::cout << "<erorr> -11" << std::endl;
	if (!eyes_cascade.load("haarcascade_eye_tree_eyeglasses.xml"))
		std::cout << "<erorr> -12" << std::endl;

}

void Face_detect::data_load(float *x, float *y, float *z, cv::VideoCapture cap) //float  x, float  y,  camera(~VideoCapture cap(o));
{
	cap >> image;

	cv::Mat resized_frame_faces(cvRound(image.rows / scale), cvRound(image.cols / scale), CV_8UC1);
	resize(image, resized_frame_faces, resized_frame_faces.size());


	face_cascade.detectMultiScale(resized_frame_faces, faces, 3, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(1, 1));
	eyes_cascade.detectMultiScale(resized_frame_faces, eyes, 3, 0, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(1, 1));



	for (int i = 0; i < faces.size(); i++)
	{
		cv::Point center_f(scale*(faces[i].x + faces[i].width*0.5), scale*(faces[i].y + faces[i].height*0.5));

//cv::ellipse(image, center_f, cv::Size(scale*(faces[i].width*0.5), scale*(faces[i].height*0.5)), 0, 0, 360, cv::Scalar(0, 0, 255), 4, 8, 0);


std::cout << faces[i].width << "%" << faces[i].x << "$" << "&" << faces[i].y << "^" << faces[i].height << std::endl;
		data_x = 30 * (0.5 - (float)center_f.x / (float)image.cols);
		data_y = 30 * (0.5 - (float)center_f.y / (float)image.rows);
	}


	for (int i = 0; i < eyes.size(); i++)
	{
		cv::Point center_e(scale*(eyes[i].x + eyes[i].width*0.5), scale*(eyes[i].y + eyes[i].height*0.5));
//cv::circle(image, center_e, 1, cv::Scalar(0, 255, 0), 4, 8, 0);


		if (30 * (0.5 - (float)center_e.x / (float)image.cols) != 0)
			data_x = 30 * (0.5 - (float)center_e.x / (float)image.cols);

		if (30 * (0.5 - (float)center_e.y / (float)image.rows) != 0)
			data_y = 30 * (0.5 - (float)center_e.y / (float)image.rows);
	}

	if (data_x > 10.0)	data_x = 10.0;
	if (data_x < -10.0)	 data_x = -10.0;

	if (data_y > 10.0)	data_y = 10.0;
	if (data_y < -10.0)	 data_y = -10.0;


	*x = data_x;
	*y = data_y;
	*z = 5;
//	cv::imshow("1", image);

	cv::waitKey(1);
}



Face_detect::~Face_detect()
{
	data_x = 0;
	data_y = 0;
}
