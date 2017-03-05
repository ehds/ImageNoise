// ImageNoise.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp> 
#include <opencv2/OpenCV.hpp>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <math.h> 
using namespace cv;
using namespace std;
/*
产生高斯噪声的函数，引自维基百科
*/
#define TWO_PI 6.2831853071795864769252866

double generateGaussianNoise()
{
	static bool hasSpare = false;
	static double rand1, rand2;

	if (hasSpare)
	{
		hasSpare = false;
		return sqrt(rand1) * sin(rand2);
	}

	hasSpare = true;

	rand1 = rand() / ((double)RAND_MAX);
	if (rand1 < 1e-100) rand1 = 1e-100;
	rand1 = -2 * log(rand1);
	rand2 = (rand() / ((double)RAND_MAX)) * TWO_PI;

	return sqrt(rand1) * cos(rand2);
}

void addNoise(Mat& picture){
	int channels = picture.channels();
	int Rows = picture.rows;
	int Cols = picture.cols*channels;
	if (picture.isContinuous()){
		Cols *= Rows;
		Rows = 1;
	}
	int i, j;
	uchar *p;//获得picture的行的指针
	for (i = 0; i < Rows; ++i)
	{
		p = picture.ptr<uchar>(i);//赋值为当前行指针
		for (j= 0; j < Cols; ++j)
		{
			if (j%5==0)
			{
				double val = p[j] + generateGaussianNoise() * 128;
				if (val < 0)
					val = 0;
				if (val > 255)
					val = 255;

				p[j] = (uchar)val;
			}
			
			
		}
	}

}
Mat  eliminateNoise(Mat *mypics,int size){
	//获取第一个文件
	Mat one = mypics[0];
	Mat out = one.clone();

	//获得文件信息
	int channels = one.channels();
	int Rows = one.rows;
	int Cols = one.cols*channels;
	if (one.isContinuous()){
		Cols *= Rows;
		Rows = 1;
	}

	int i, j;
	//uchar *p;//获得picture的行的指针
	uchar **p = new uchar *[10];
	uchar *out_p;
	for (i = 0; i < Rows; ++i)
	{
		//输出文件的行指针
		out_p = out.ptr<uchar>(i);
		for (size_t z = 0; z < size; ++z)
			//赋值为当前行指针
			p[z] = mypics[z].ptr<uchar>(i);
		for (j = 0; j < Cols; ++j)
		{
			double val = 0;
			//累加每张图片每个像素点的值
			for (size_t z = 0; z <size; z++)
				val += floor(p[z][j]);
			//cout << val/10<<endl;
			//除以总数得到平均值
			out_p[j] = (uchar)(val / size);

		}
	}
	return out;
}
void ManyImages(vector<Mat> Images, Mat& dst, int imgRows)
{
	int Num = Images.size();//得到Vector容器中图片个数
	//设定包含这些图片的窗口大小
	Mat Window(300 * ((Num - 1) / imgRows + 1), 300 * imgRows, CV_8UC3, Scalar(0, 0, 0));
	Mat Std_Image;//存放标准大小的图片
	Mat imageROI;//图片放置区域
	Size Std_Size = Size(300, 300);//每个图片显示大小300*300
	int x_Begin = 0;
	int y_Begin = 0;
	for (int i = 0; i < Num; i++)
	{
		x_Begin = (i % imgRows)*Std_Size.width;//每张图片起始坐标
		y_Begin = (i / imgRows)*Std_Size.height;
		resize(Images[i], Std_Image, Std_Size, 0, 0, INTER_LINEAR);//将图像设为标准大小
		//将其贴在Window上
		imageROI = Window(Rect(x_Begin, y_Begin, Std_Size.width, Std_Size.height));
		Std_Image.copyTo(imageROI);
	}
	dst = Window;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//string name ="image.jpg";
	//int ImageSize =30;
	int ImageSize = atoi(argv[2]);
	string name = argv[1];

	Mat pic = imread(name);
	//imshow("原始图片", pic);
	//waitKey();
	//根据数量生成图片
	string newname;
	for (size_t i = 0; i < ImageSize; i++)
	{
		Mat newImage = pic.clone();
		addNoise(newImage);
		ostringstream ss;
		ss << "noise" << i << ".jpg";
		newname = ss.str();
		imwrite(newname, newImage);
	}
	//根据生成的噪声图片，进行叠加
	Mat *mypics=new Mat[ImageSize];
	//图片的通道
	string file;
	for (size_t i = 0; i < ImageSize; i++)
	{
		ostringstream name;
		name << "noise" << i << ".jpg";
		 file = name.str();
		mypics[i] = imread(file, 1);
	}
	//获得消除噪声的输出
	Mat && out = std::move(eliminateNoise(mypics, ImageSize));
	ostringstream ss;
	ss << "output" << ImageSize << ".jpg";
	 newname = ss.str();
	imwrite(newname, out);
	vector<Mat> manyimgV;
	manyimgV.push_back(std::move(pic));
	manyimgV.push_back(out);

	Mat dst;
	ManyImages(manyimgV,dst, 2);
	imshow("结果输出,左边为原始图片，右边为结果图片", dst);
	waitKey(0);
	delete mypics;
	
	return 0;
}

