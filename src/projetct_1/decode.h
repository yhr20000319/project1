#pragma once
#include <iostream>
#include<fstream>
#include<cstring>
#include "opencv.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
void decode();
cv::Point Center_cal(vector<vector<cv::Point> > contours, int i);
void position(cv::Mat InImage,/* Mat &OutImage,*/  cv::Point point[4], cv::Mat& threshold_output);
int rounding(double x);//Àƒ…·ŒÂ»Î
void decoding(cv::Point point[3],cv::Mat InImage, ofstream& file_out);
void adjusting(cv::Mat src,cv::Mat& src_all, cv::Point point[]);