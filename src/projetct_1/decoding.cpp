#include"decoding.h"
void decoding() {
	//getPoint存储透视变换的四个顶点
	cv::Mat src = cv::imread(".\\result_pic\\pic0.jpg");
	cv::Mat src2 = cv::imread(".\\result_pic\\pic1.jpg");
	cv::Mat src3 = cv::imread(".\\result_pic\\pic2.jpg");
	cv::Mat src4 = cv::imread(".\\result_pic\\pic3.jpg");
	cv::Mat src5 = cv::imread(".\\result_pic\\pic4.jpg");
	cv::Mat src6 = cv::imread(".\\result_pic\\pic5.jpg");
	cv::Mat dst;
	vector<vector<cv::Point>> qrPoint;
	cv::Point2f adPoint[4];
	cv::Point2f getPoint[4];
	FindQrPoint(src, qrPoint,adPoint,getPoint);
	respective(src, dst, getPoint);
	//imshow("output", dst);
	//imshow("1", src);
	thresd(dst);
	//imshow("output", dst);
	//解码流程
	string dname;
	cout << "请输入解码文件名：";
	cin >> dname;
	ofstream file_out(dname, ios::app);

	decode(dst,file_out);

	//剩下五张图同上
	FindQrPoint(src2, qrPoint, adPoint, getPoint);
	respective(src2, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	FindQrPoint(src3, qrPoint, adPoint, getPoint);
	respective(src2, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	FindQrPoint(src4, qrPoint, adPoint, getPoint);
	respective(src2, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	FindQrPoint(src5, qrPoint, adPoint, getPoint);
	respective(src2, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	FindQrPoint(src6, qrPoint, adPoint, getPoint);
	respective(src6, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	file_out.close();
	cv::waitKey(0);
}

//二值化
void thresd(cv::Mat& src) 
{
	cv::Mat src_gray;
	cvtColor(src, src_gray, CV_BGR2GRAY);
	cv::Mat threshold_output;
	threshold(src_gray, src, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}
int FindQrPoint(cv::Mat& srcImg, vector<vector<cv::Point>>& qrPoint, cv::Point2f adPoint[],cv::Point2f getPoint[])
{
	//彩色图转灰度图
	cv::Mat src_gray;
	cvtColor(srcImg, src_gray, CV_BGR2GRAY);
	//cv::namedWindow("src_gray");
	//imshow("src_gray", src_gray);

	//二值化
	cv::Mat threshold_output;
	threshold(src_gray, threshold_output, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	cv::Mat threshold_output_copy = threshold_output.clone();
	//cv::namedWindow("Threshold_output");
	//cv::imshow("Threshold_output", threshold_output);

	//调用查找轮廓函数
	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));

	//通过黑色定位角作为父轮廓，有两个子轮廓的特点，筛选出三个定位角
	int parentIdx = -1;
	int ic = 0; int count = 0;

	for (int i = 0; i < contours.size(); i++)
	{
	
		if (hierarchy[i][2] != -1 && ic == 0)
		{
			parentIdx = i;
			ic++;
		}
		else if (hierarchy[i][2] != -1)
		{
			ic++;
		}
		else if (hierarchy[i][2] == -1)
		{
			ic = 0;
			parentIdx = -1;
		}

		// 有两个子轮廓才是二维码的顶点
		if (ic >= 2)
		{
			bool isQr = IsQrPoint(contours[parentIdx], threshold_output_copy, i);

			//保存找到的三个黑色定位角
			if (isQr)
			{
				qrPoint.push_back(contours[parentIdx]);
				cv::RotatedRect rotatedRect = cv::minAreaRect(contours[parentIdx]);
				rotatedRect.points(adPoint);
				
				//根据qrPoint改变adPoint顺序
				//根据四个角的不同获取四个点
				//右下角
				if (count == 0) {
					for (int m = 0; m < 4; m++) {

						if (adPoint[m].x + 10 > adPoint[0].x&&adPoint[m].x + 10 > adPoint[1].x&&adPoint[m].x + 10 > adPoint[2].x&&adPoint[m].x + 10 > adPoint[3].x) {
							if (adPoint[m].y + 10 > adPoint[0].y&&adPoint[m].y + 10 > adPoint[1].y&&adPoint[m].y + 10 > adPoint[2].y&&adPoint[m].y + 10 > adPoint[3].y)
							{
								getPoint[count].x = adPoint[m].x;
								getPoint[count].y = adPoint[m].y;
								count++;
							}
						}

					}
				}
				//左下角
				else if (count == 1) {
					for (int m = 0; m < 4; m++) {

						if (adPoint[m].x - 10 < adPoint[0].x&&adPoint[m].x - 10 < adPoint[1].x&&adPoint[m].x - 10 < adPoint[2].x&&adPoint[m].x - 10 < adPoint[3].x) {
							if (adPoint[m].y + 10 > adPoint[0].y&&adPoint[m].y + 10 > adPoint[1].y&&adPoint[m].y + 10 > adPoint[2].y&&adPoint[m].y + 10 > adPoint[3].y)
							{
								getPoint[count].x = adPoint[m].x;
								getPoint[count].y = adPoint[m].y;
								count++;
							}
						}

					}
				}
				//右上角
				else if (count == 2) {
					for (int m = 0; m < 4; m++) {

						if (adPoint[m].x + 10 > adPoint[0].x&&adPoint[m].x + 10 > adPoint[1].x&&adPoint[m].x + 10 > adPoint[2].x&&adPoint[m].x + 10 > adPoint[3].x) {
							if (adPoint[m].y - 10 < adPoint[0].y&&adPoint[m].y - 10 < adPoint[1].y&&adPoint[m].y - 10 < adPoint[2].y&&adPoint[m].y - 10 < adPoint[3].y)
							{
								getPoint[count].x = adPoint[m].x;
								getPoint[count].y = adPoint[m].y;
								count++;
							}
						}

					}
				}
				//左上角
				else if (count == 3) {
					for (int m = 0; m < 4; m++) {

						if (adPoint[m].x - 10 < adPoint[0].x&&adPoint[m].x - 10 < adPoint[1].x&&adPoint[m].x - 10 < adPoint[2].x&&adPoint[m].x - 10 < adPoint[3].x) {
							if (adPoint[m].y - 10 < adPoint[0].y&&adPoint[m].y - 10 < adPoint[1].y&&adPoint[m].y - 10 < adPoint[2].y&&adPoint[m].y - 10 < adPoint[3].y)
							{
								getPoint[count].x = adPoint[m].x;
								getPoint[count].y = adPoint[m].y;
								count++;
							}
						}

					}
				}

			
				//buxu
			}
			ic = 0;
			parentIdx = -1;
		}
	}

	return 0;
}

//调整角位置
void adjusting(vector<vector<cv::Point>> qrPoint,cv::Point adPoint[]) {
	/*adPoint[3].x = qrPoint[3][0].x;
	adPoint[3].y = qrPoint[3][0].y;//左上角
		int centerx = 0, centery = 0, n = qrPoint.size();
		centerx = (qrPoint[2][n / 4].x + qrPoint[2][n * 2 / 4].x + qrPoint[2][3 * n / 4].x + qrPoint[2][n - 1].x) / 4;
		centery = (qrPoint[2][n / 4].y + qrPoint[2][n * 2 / 4].y + qrPoint[2][3 * n / 4].y + qrPoint[2][n - 1].y) / 4;
		adPoint[2].x =centerx+ (centery-qrPoint[2][0].y);
		adPoint[2].y = centery- (centerx-qrPoint[2][0].x);//右上角

		centerx = (qrPoint[1][n / 4].x + qrPoint[1][n * 2 / 4].x + qrPoint[1][3 * n / 4].x + qrPoint[1][n - 1].x) / 4;
		centery = (qrPoint[1][n / 4].y + qrPoint[1][n * 2 / 4].y + qrPoint[1][3 * n / 4].y + qrPoint[1][n - 1].y) / 4;
		adPoint[1].x = centerx - (centery - qrPoint[1][0].y);
		adPoint[1].y = centery + (centerx-qrPoint[1][0].x);//左下角

		centerx = (qrPoint[0][n / 4].x + qrPoint[0][n * 2 / 4].x + qrPoint[0][3 * n / 4].x + qrPoint[0][n - 1].x) / 4;
		centery = (qrPoint[0][n / 4].y + qrPoint[0][n * 2 / 4].y + qrPoint[0][3 * n / 4].y + qrPoint[0][n - 1].y) / 4;
		adPoint[0].x = centerx + (centery - qrPoint[0][0].y);
		adPoint[0].y = centery + (centerx - qrPoint[0][0].x);//右下角

		cout <<qrPoint.size();*/

}
bool IsQrPoint(vector<cv::Point>& contour, cv::Mat& img,int i)
{
	//最小大小限定
	cv::RotatedRect rotatedRect = cv::minAreaRect(contour);
	if (rotatedRect.size.height < 10 || rotatedRect.size.width < 10)
		return false;

	//将二维码从整个图上抠出来
	cv::Mat cropImg = transformCorner(img, rotatedRect);
	int flag = i++;

	//横向黑白比例1:1:3:1:1
	bool result = IsQrColorRate(cropImg, flag);
	return result;
}

cv::Mat transformCorner(cv::Mat src, cv::RotatedRect rect)
{
	cv::Point center = rect.center;   //旋转中心
	//circle(src, center, 2, Scalar(0, 0, 255), 2);
	//Size sz = Size(rect.size.width, rect.size.height);
	cv::Point TopLeft = cv::Point(cvRound(center.x), cvRound(center.y)) - cv::Point(rect.size.height / 2, rect.size.width / 2);  //旋转后的目标位置
	TopLeft.x = TopLeft.x > src.cols ? src.cols : TopLeft.x;
	TopLeft.x = TopLeft.x < 0 ? 0 : TopLeft.x;
	TopLeft.y = TopLeft.y > src.rows ? src.rows : TopLeft.y;
	TopLeft.y = TopLeft.y < 0 ? 0 : TopLeft.y;

	//Point ButtonRight = (Point)center - Point(rect.size.width, rect.size.height);
	cv::Rect RoiRect = cv::Rect(TopLeft.x, TopLeft.y, rect.size.width, rect.size.height);   //抠图必备矩形
	double angle = rect.angle;        //旋转角度
	cv::Mat mask, roi, dst;                //dst是被旋转的图片 roi为输出图片 mask为掩模
	cv::Mat image;						 //被旋转后的图片
	cv::Size sz = src.size();             //旋转后的尺寸
	mask = cv::Mat::zeros(src.size(), CV_8U);

	/************************************
	为掩模上色 一般为白色
	因为RotatedRect 类型的矩形不容易调取内像素 
	因此我把矩形的四个顶点当成轮廓 再用drawContours填充
	************************************/
	vector<cv::Point> contour;
	cv::Point2f points[4];
	rect.points(points);
	for (int i = 0; i < 4; i++)
		contour.push_back(points[i]);
	vector<vector<cv::Point>> contours;
	contours.push_back(contour);
	drawContours(mask, contours, 0, cv::Scalar(1), -1);

	/*抠图，然后围绕中心矩阵中心旋转*/
	src.copyTo(dst, mask);
	//roi = dst(RoiRect);
	cv::Mat M = getRotationMatrix2D(center, angle, 1);
	warpAffine(dst, image, M, sz);
	roi = image(RoiRect);

	//imshow("image", image);
	return roi;
}


//横向和纵向黑白比例判断
bool IsQrColorRate(cv::Mat& image, int flag)
{
	bool x = IsQrColorRateX(image, flag);
	if (!x)
		return false;
	bool y = IsQrColorRateY(image, flag);
	return y;
}
//横向黑白比例判断
bool IsQrColorRateX(cv::Mat& image, int flag)
{
	int nr = image.rows / 2;//行
	int nc = image.cols * image.channels();//列

	vector<int> vValueCount;
	vector<uchar> vColor;
	int count = 0;
	uchar lastColor = 0;

	uchar* data = image.ptr<uchar>(nr);
	for (int i = 0; i < nc; i++)
	{
		vColor.push_back(data[i]);
		uchar color = data[i];
		if (color > 0)
			color = 255;

		if (i == 0)
		{
			lastColor = color;
			count++;
		}
		else
		{
			if (lastColor != color)
			{
				vValueCount.push_back(count);
				count = 0;
			}
			count++;
			lastColor = color;
		}
	}

	if (count != 0)
		vValueCount.push_back(count);

	if (vValueCount.size() < 5)
		return false;

	//横向黑白比例1:1:3:1:1
	int index = -1;
	int maxCount = -1;
	for (int i = 0; i < vValueCount.size(); i++)
	{
		if (i == 0)
		{
			index = i;
			maxCount = vValueCount[i];
		}
		else
		{
			if (vValueCount[i] > maxCount)
			{
				index = i;
				maxCount = vValueCount[i];
			}
		}
	}

	//左边 右边 都有两个值，才行
	if (index < 2)
		return false;
	if ((vValueCount.size() - index) < 3)
		return false;

	//黑白比例1:1:3:1:1
	float rate = ((float)maxCount) / 3.00;

	//cout << "flag:" << flag << " ";

	float rate2 = vValueCount[index - 2] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index - 1] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index + 1] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index + 2] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	return true;
}
//纵向黑白比例判断 省略
bool IsQrColorRateY(cv::Mat& image, int flag){
	int nc = image.cols / 2;
	int nr = image.rows * image.channels();

	vector<int> vValueCount;
	vector<uchar> vColor;
	int count = 0;
	uchar lastColor = 0;

	uchar* data = image.ptr<uchar>(nc);
	for (int i = 0; i < nr; i++)
	{
		vColor.push_back(data[i]);
		uchar color = data[i];
		if (color > 0)
			color = 255;

		if (i == 0)
		{
			lastColor = color;
			count++;
		}
		else
		{
			if (lastColor != color)
			{
				vValueCount.push_back(count);
				count = 0;
			}
			count++;
			lastColor = color;
		}
	}

	if (count != 0)
		vValueCount.push_back(count);

	if (vValueCount.size() < 5)
		return false;

	//横向黑白比例1:1:3:1:1
	int index = -1;
	int maxCount = -1;
	for (int i = 0; i < vValueCount.size(); i++)
	{
		if (i == 0)
		{
			index = i;
			maxCount = vValueCount[i];
		}
		else
		{
			if (vValueCount[i] > maxCount)
			{
				index = i;
				maxCount = vValueCount[i];
			}
		}
	}

	//左边 右边 都有两个值，才行
	if (index < 2)
		return false;
	if ((vValueCount.size() - index) < 3)
		return false;

	//黑白比例1:1:3:1:1
	float rate = ((float)maxCount) / 3.00;

	//cout << "flag:" << flag << " ";

	float rate2 = vValueCount[index - 2] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index - 1] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index + 1] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index + 2] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	return true;
}

bool IsQrRate(float rate)
{
	//大概比例 不能太严格
	return rate > 0.6 && rate < 1.9;
}
 // pointDest存放调整后的三个点，三个点的顺序如下
 // pt0----pt1
 // 
 // pt2

//透视变换
void respective(cv::Mat src,cv::Mat &dst,cv::Point2f getPoint[]) {
	vector<cv::Point2f> dstTri(4);
	vector<cv::Point2f> srcTri(4);
	//将透视变换的四个顶点给到srcTri
	srcTri[0].x = getPoint[3].x;
	srcTri[0].y = getPoint[3].y;
	srcTri[1].x = getPoint[2].x;
	srcTri[1].y = getPoint[2].y;
	srcTri[2].x = getPoint[0].x;
	srcTri[2].y = getPoint[0].y;
	srcTri[3].x = getPoint[1].x;
	srcTri[3].y = getPoint[1].y;

	//规定目的变换四个顶点控制大小
	dstTri[0].x = 0;
	dstTri[0].y = 0;
	dstTri[1].x = 800;
	dstTri[1].y = 0;
	dstTri[2].x = 800;
	dstTri[2].y = 450;
	dstTri[3].x = 0;
	dstTri[3].y = 450;


	//adjusting(qrPoint,adPoint );
	/*cv::line(src, getPoint[0], getPoint[1], cv::Scalar(0, 0, 255), 2);
	cv::line(src, getPoint[1], getPoint[3], cv::Scalar(0, 0, 255), 2);//测试getPoint是否是二维码四个顶点
	cv::line(src, getPoint[2], getPoint[3], cv::Scalar(0, 0, 255), 2);
	cv::line(src, getPoint[2], getPoint[0], cv::Scalar(0, 0, 255), 2);*/
	cv::Mat H = cv::findHomography(srcTri, dstTri, cv::RANSAC);//计算透视矩阵
	cv::warpPerspective(src, dst, H, cv::Size(800, 450));//图像透视变换
}

void decode(cv::Mat src,ofstream& file_out) {
	for (int hight = 0; hight < 450; hight = hight + 10) {
		for (int width = 0; width < 800; width=width+10) {
			int black = 0, white = 0;
			if (hight <= 70 && (width <= 70.0 || width >= 720.0));//定位点不解码
			else if (hight >= 370  && width <= 70);//定位点不解码
			else {
				for (int m = 0; m <10; m++) {
					for (int n = 0; n < 10; n++) {
						if (src.at<uchar>(hight + m , width + n ) == 0)black++;
						else if (src.at<uchar>(hight + m, width + n) == 255)white++;
					}
				}
				//cout << black;
				if (white >= black) {
					file_out << "1"; //cout << "1";
				}
				else if (black > white) {
					file_out << "0";// cout << "0";
				}
			}
		}
	}
}
