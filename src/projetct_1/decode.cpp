// decoding.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "decode.h"
void decode()
{
	cv::Mat src = cv::imread(".\\result_pic\\pic0.jpg");
	cv::Mat src3 = cv::imread(".\\result_pic\\pic1.jpg");
	cv::Mat src5 = cv::imread(".\\result_pic\\pic2.jpg");
	cv::Mat src7 = cv::imread(".\\result_pic\\pic3.jpg");
	cv::Mat src9 = cv::imread(".\\result_pic\\pic4.jpg");
	cv::Mat src11 = cv::imread(".\\result_pic\\pic5.jpg");
	cv::Mat src2, src4, src6, src8, src10, src12;
	cv::Point point[3]; cv::Point pointAdjust[3];
	
	string dname;
	cout << "请输入解码文件名：";
	cin >> dname;

	ofstream file_out(dname, ios::app);
	if (file_out.fail()) {
		cout << "打开文件失败" << endl;
		exit(0);
	}
	//position(src, point, src2);
	//adjusting(src2, src2, point);
	//decoding(point, src2, file_out);//imshow("1", src2);
	/*for (int i = 0; i < 3; i++) {
		cout << "第" <<  i<< "个中心点坐标" << point[i].x << "." << point[i].y << endl;
	}*/
	//imshow("1", src2);
	//position(src3, point, src4);
	//cv::imshow("1", src4);
	//adjusting(src3, src3, point);
	//decoding(point, src4, file_out);
	//position(src5, point, src6);
	//adjusting(src6, src6, point);
	//decoding(point, src6, file_out);
	//position(src7, point, src8);
	//adjusting(src8, src8, point);
	//decoding(point, src8, file_out);
	//position(src9, point, src10);
	//adjusting(src10, src10, point);
	//decoding(point, src10, file_out);
	//position(src11, point, src12);
	//adjusting(src12, src12, point);
	//decoding(point, src12, file_out);
	file_out.close();
	//imshow("title", src2);
	cv::waitKey(0);
}

void position(cv::Mat InImage, cv::Point point[4], cv::Mat& threshold_output)
{
	cv::Point QRPoints[4];
	cv::Mat src = InImage;
	cv::Mat src_gray;
	cv::Mat src_all = src.clone();

	vector<vector<cv::Point> > contours, contours2;
	vector<cv::Vec4i> hierarchy;
	//预处理
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, cv::Size(3, 3)); //模糊，去除毛刺
	threshold(src_gray, threshold_output, 100, 255, cv::THRESH_OTSU);//输入图，输出图
	//imshow("1", threshold_output);
	//寻找轮廓 
	//第一个参数是输入图像 2值化的
	//第二个参数是内存存储器，FindContours找到的轮廓放到内存里面。
	//第三个参数是层级，**[Next, Previous, First_Child, Parent]** 的vector
	//第四个参数是类型，采用树结构
	//第五个参数是节点拟合模式，这里是全部寻找
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));

	//轮廓筛选
	int c = 0, ic = 0, area = 0;
	int parentIdx = -1;
	for (int i = 0; i < contours.size(); i++)
	{
		//hierarchy[i][2] != -1 表示不是最外面的轮廓
		if (hierarchy[i][2] != -1 && ic == 0)
		{
			parentIdx = i;
			ic++;
		}
		else if (hierarchy[i][2] != -1)
		{
			ic++;
		}
		//最外面的清0
		else if (hierarchy[i][2] == -1)
		{
			ic = 0;
			parentIdx = -1;
		}
		//找到定位点信息
		if (ic >= 2)
		{
			contours2.push_back(contours[parentIdx]);
			ic = 0;
			parentIdx = -1;
		}
	}
	if (contours2.size() < 3)
	{
		for (int i = 0; i < 4; i++)
		{
			QRPoints[i].x = 0;
			QRPoints[i].y = 0;
		}
	}
	else
	{

		for (int i = 0; i < contours2.size(); i++)
		{
			point[i] = Center_cal(contours2, i);
		}

		//cv::line(src_all, point[0], point[1], cv::Scalar(0, 0, 255), 2);//绘制连接两个点的线段,参数为图像，第一/第二个端点，颜色，线粗细
		//cv::line(src_all, point[1], point[2], cv::Scalar(0, 0, 255), 2);
		//line(src_all, point[2], point[0], Scalar(0, 0, 255), 2);
	}
	//cv::imshow("123", src_all);
}
cv::Point Center_cal(vector<vector<cv::Point> > contours, int i)//第0是左下角，1是右上角，2是左上角
{

	int centerx = 0, centery = 0, n = contours[i].size();
	//cout << i << "size" << n<<endl;
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	cv::Point point1 = cv::Point(centerx, centery);
	cout << "第" << i << "个中心点坐标" <<centerx<<"."<<centery<< endl;
	return point1;
}
//四舍五入的函数
int rounding(double x) {
	if (x - (int)x >= 0.5)return (int)x + 1;
	else return (int)x;
}

//透视变换

//图片有可能是倾斜的，倾斜夹角可以通过pt0与pt1连线与水平线之间的夹角确定。二维码的倾斜角度就是整个图片的倾斜角度，从而可以对整个图片进行水平矫正。
double Angle(cv::Point p1, cv::Point p2, bool& clockwise) {
	if (p1.y > p2.y)clockwise = 1;//为真逆时针转
	else clockwise = 0;
	return cvFastArctan(p1.y - p2.y, p1.x - p2.x);
}
void adjusting(cv::Mat src, cv::Mat& src_all, cv::Point point[]) {
	cv::Point pointAdjust[3];
	pointAdjust[0].x = point[2].x - 35 * (point[1].x - point[2].x) / 730;
	pointAdjust[1].x = point[1].x + 35 * (point[0].y - point[2].y) / 380;
	pointAdjust[0].y = point[2].y - 35 * (point[0].y - point[2].y) / 380;
	pointAdjust[1].y = point[1].y - 35 * (point[1].x - point[2].x) / 730;
	//line(src, pointAdjust[0], pointAdjust[1], cv::Scalar(0, 0, 255), 2);

	//二维码倾斜角度
	bool clockwise;
	double qrAngle = Angle(pointAdjust[1], pointAdjust[0], clockwise);

	//以二维码左上角点为中心 旋转
	src_all = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
	double rotationAngle = clockwise ? -qrAngle : qrAngle;//负值逆时针，为真取负值
	cv::Mat affine_matrix = cv::getRotationMatrix2D(pointAdjust[0], rotationAngle, 1.0);//求得旋转矩阵
	cv::warpAffine(src, src_all, affine_matrix, src_all.size());
	//cv::imshow("1", src);

}
//解码部分
void decoding(cv::Point point[3], cv::Mat InImage, ofstream& file_out) {
	//检测x坐标，y坐标，比例尺，检测从像素块左上角起

	double scalex = (point[1].x - point[2].x) / 730.0; // cout << scalex;                //制作时二者距离（790+730）/2-（0+60）/2=730.
	double scaley = (point[0].y - point[2].y) / 380.0;// cout << scaley;            //(440+380)/2-(0+60)/2=380
	double ereax = scalex * 10.0; double ereay = (scaley * 10.0);
	double detect_x = point[2].x - (35.0 * scalex);
	double detect_y = point[2].y - (35.0 * scaley);
	double totalx = (800.0 * scalex);
	double totaly = (450.0 * scaley);
	double slope_y = (point[1].y - point[2].y)/(double)(point[1].x-point[2].x);
	double slope_x = (point[0].x - point[2].x)/(double)(point[0].y-point[2].y);
	for (double hight = detect_y; hight < detect_y + totaly; hight = hight + ereay)
	{
		//cout << "1";
		int i = 0,j=0;
		for (double width = detect_x; width < detect_x + totalx; width = width + ereax)
		{
			//cout << detect_x+totalx<<endl;
			int black = 0, white = 0;
			if (hight <= detect_y + (70.0 * scaley) && (width <= detect_x + (70.0 * scalex) || width >= detect_x + (720.0 * scalex)));//定位点不解码
			else if (hight >= detect_y + (370.0 * scaley) && width <= detect_x + (70.0 * scalex));//定位点不解码
			else {
				for (int m = 0; m < ereay; m++) {
					for (int n = 0; n < ereax; n++) {
						if (InImage.at<uchar>(hight +m+i*slope_y, width + n+j*slope_x) == 0)black++;
						else if (InImage.at<uchar>(hight + m, width + n) == 255)white++;
					}
				}
				//cout << black;
				if (white > black) {
					file_out << "1"; //cout << "1";
				}
				else if (black > white) {
					file_out << "0";// cout << "0";
				}
			}
			i +=  ereay;
			j += ereax;
		}
	}

}