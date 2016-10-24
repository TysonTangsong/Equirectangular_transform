#if 1
#include <opencv2\opencv.hpp>
#include <string>


using namespace cv;
using namespace std;

int sign(double x)
{ 
	if (x == 0)
	{
		return 0;
	}
	else
	{
		if (x > 0) return 1; 
		else return -1;
	}
}

int main(int argc, char* argv[])
//int eq_transform(int argc, char* argv[])
{
	double pi = 3.14159265358979323846;


	string filename = "1.jpeg";
	double ar, ap, ay;
	ar = 0/4;
	ap = -pi/4;
	ay = 0/4;
	cv::Mat img_origin = cv::imread(filename);
	cv::Size img_size = img_origin.size();
	cv::Mat img_undist = cv::Mat::zeros(img_size, CV_8UC3);
	int pnwidth = img_size.width;
	int pnheight = img_size.height;
	
	double b = 2 * pi;
	double mpdistance = pnwidth / b;
	
	double rot[2] = { mpdistance*pi, -ay*mpdistance };
	
	double m[3][3] = { cos(ar), sin(ar), 0,
		-cos(ap)*sin(ar), cos(ap)*cos(ar), sin(ap),
		sin(ar)*sin(ap), -sin(ap)*cos(ar), cos(ap)
	};

	
	{


		for (int x = 1; x <= pnheight; ++x)
		{

			for (int y = 1; y <= pnwidth; ++y)
			{
		
				double src[2] = { y , x };
				src[0] += rot[1];
			 		
				double phi = src[0] / mpdistance;
				double theta1 = src[1] / mpdistance - pi / 2;
												
				//double v[3] = { cos(phi)*cos(theta1), sin(phi)*cos(theta1), sin(theta1) };
				double v[3] = { cos(phi)*cos(theta1), sin(theta1), sin(phi)*cos(theta1), };
				double u[3] = { v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0], v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1], v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] };
				
				phi = atan2(u[2], u[0]);
				theta1 = asin(u[1]) + pi / 2;

				src[0] = mpdistance*(phi + sign(abs(u[0]))*pi);
				src[0] = (int)(src[0] - 0.5 + pnwidth / 2) % pnwidth;
				src[1] = (int)(mpdistance*(theta1) - 0.5 );
		
				if (src[1] < 1)
					src[1] = 1;
				if (src[1] > pnwidth / 2 )
					src[1] = pnwidth / 2 ;
				if (src[0] < 1)
					src[0] = 1;
				if (src[0] > pnwidth )
					src[0] = pnwidth ;
				
				img_undist.at<cv::Vec3b>(x-1, y-1) = img_origin.at<cv::Vec3b>(src[1], src[0]);
			}
		}
	}
	cv::imwrite("rota.jpg", img_undist);
	std::system("pause");
	return 0;
}
#endif