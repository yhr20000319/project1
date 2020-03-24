#pragma once
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<vector>

using namespace std;
cv::Mat  gaycolor(cv::Mat& inputimage, int flag);
void video2pic();
