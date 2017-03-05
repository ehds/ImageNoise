# ImageNoise使用说明
本代码是利用图片叠加的原理消除噪声的实验代码
原理

## 原理说明
图像的加法可用于平均以减少和去除图像采集中混入的噪声，在采集中实际图像的时候，由于各种不同的原因，常会有一些干扰或噪声混入到最后采集的图像中。从这个意义上来说，实际采集到的图像g_i (x,y可看做是由原始场景图像f(x,y)和噪声图像叠加而成的，即

	g(x,y)=f(x,y)+ε(x,y)
	
图像的加法可用于平均以减少和去除图像采集中混入的噪声，在采集中实际图像的时候，由于各种不同的原因，常会有一些干扰或噪声混入到最后采集的图像中。从这个意义上来说，实际采集到的图像g_i (x,y可看做是由原始场景图像f(x,y)和噪声图像叠加而成的，即

	g(x,y)=f(x,y)+ε(x,y)
	
那么可以证明新的图像的期望值为

    E{¯g(x,y)}=f(x,y)

如果考虑新图像和噪声图像各自的均方差的关系，则
	σ_(¯g(x,y))=√(1/M)×σ_(e(x,y))

可见随着平均图像数量M的增加，噪声在每个像素的位置(x,y)的影响越来越小

	
     [高斯噪声原理](https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform)
```
#include <cstdlib>
#include <cmath>
#include <limits>
double generateGaussianNoise(double mu, double sigma)
{
	const double epsilon = std::numeric_limits<double>::min();
	const double two_pi = 2.0*3.14159265358979323846;

	static double z0, z1;
	static bool generate;
	generate = !generate;

	if (!generate)
	   return z1 * sigma + mu;

	double u1, u2;
	do
	 {
	   u1 = rand() * (1.0 / RAND_MAX);
	   u2 = rand() * (1.0 / RAND_MAX);
	 }
	while ( u1 <= epsilon );

	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return z0 * sigma + mu;
}

```
### 使用说明
1. 下载源码
2. 命令行切换到打开build目录
3. 输入

    ImageNoise.exe image.jpg 100

(第二个参数为图片名称，第三个参数为加噪图片数量)
4.结果
![结果展示](http://img1.ph.126.net/0kkj7Bv6K7SAPy3gG-kk8g==/6632140889049987376.png)

5. 目录下会生成一个output100.jpg的文件即为输出文件




原始图片
![原始](http://img1.ph.126.net/FzK2IYex6E1a7cYgBOvhKg==/6632534514210060907.jpg)

加噪图片
![加噪](http://img0.ph.126.net/ZUO3S-CdA-gonG_2Pbtxlg==/6632598285884470377.jpg)

结果图片
![结果](http://img2.ph.126.net/A39tIC_nbdIgpiN0yLm2XA==/6632065022747673989.jpg)








