// network_project1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "decoding.h"
#include "encode.h"
#include "src_code_pic2video.h"
#include "video2result_pic.h"
using namespace std;
using namespace cv;
int main(){
	int choice;
	cout << "请输入选择:" << endl << " 1.生成随机二维码图片及二进制文件 并生成视频文件" << endl 
		<<"2.拍摄的视频文件转图片"<<endl
		<<"3.解码图片生成二进制文件"<<endl
		<<"4.退出"<<endl;
	cin >> choice;
	while (choice == 1) {
		encode();
		video2result_pic();
		cout << "生成完毕，请输入选择："<<endl;
		cin >> choice;
	}
	while (choice == 2) {
		video2pic();
		cout << "生成完毕，请输入选择：" << endl;
		cin >> choice;
	}
	while (choice == 3) {
		decoding();
		cout << "生成完毕" << endl;
		return 1;
	}
		while(choice==4)return 0;
	return 0;
}

