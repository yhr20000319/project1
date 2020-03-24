#pragma once
#include <iostream>
#include<fstream>
#include<cstring>
#include "opencv.hpp"
using namespace std;

#define black p[0] = 0;p[1] = 0;p[2] = 0
#define white p[0] = 255;p[1] = 255;p[2] = 255
#define red p[0] = 0;p[1] = 0;p[2] = 255
#define orange p[0] = 0;p[1] = 165;p[2] = 255
#define yellow p[0] = 0;p[1] = 255;p[2] = 255
#define green p[0] = 0;p[1] = 255;p[2] = 0
#define blue p[0] = 255;p[1] = 0;p[2] = 0
#define cyan p[0] = 255;p[1] = 255;p[2] = 0
#define purple p[0] = 240;p[1] = 32;p[2] = 160 //用于彩图制作
//制作定位点及边框等
int analysis(int h, int w);
void create_white(cv::Mat& img);
int encode();