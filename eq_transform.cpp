#if 0
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

int temain(int argc, char* argv[])
//int eq_transform(int argc, char* argv[])
{
	double pi = 3.14159265358979323846;


	string filename = "1.jpeg";
	double ar, ap, ay;
	ar = -pi/4;
	ap = 0;
	ay = 0;
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
		//	int x = l1;

			for (int y = 1; y <= pnwidth; ++y)
			{
				
				//int y = l2;

				double src[2] = { y - pnwidth/2, x - pnheight/2 };
				src[0] += rot[1];
			 
				src[0] = sign(abs(src[0] - pnwidth / 2))*((sign(src[0] - pnwidth / 2) + 1)*(src[0] - pnwidth / 2) - sign(src[0] - pnwidth / 2)*src[0]) + (1 - sign(abs(src[0] - pnwidth / 2)))*src[0];
			
				src[0] = sign(abs(src[0] + pnwidth / 2))*((1 - sign(src[0] + pnwidth / 2))*(src[0] + pnwidth / 2) + sign(src[0] + pnwidth / 2)*src[0]) + (1 - sign(abs(src[0] + pnwidth / 2)))*src[0];


				double phi = src[0] / mpdistance;

				double theta1 = -src[1] / mpdistance + pi / 2;

			
				double s1 = sin(theta1);
				double c1 = cos(theta1);
				double s2 = sin(phi);
				double c2 = cos(phi);

			

				double v1[2] = { s1*s2, c1 };
				double r1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
				double theta2 = mpdistance*atan2f(r1, s1*c2);

				double v[3] = { (abs(sign(theta2))*s1*s2), (abs(sign(theta2))*c1), (s1*c2) };
				
				double u[3] = { v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0], v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1], v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] };
				
			
				double r = sqrt(u[0] * u[0] + u[1] * u[1]);
				src[0] = abs(sign(r)*mpdistance*atan2(r, u[2])) / r*u[0];
				src[1] = abs(sign(r)*mpdistance*atan2(r, u[2])) / r*u[1];

				r = sqrt(src[0] * src[0] + src[1] * src[1]);
				double theta = r / mpdistance;
				
			
				double s = abs(sign(theta))*sin(theta) / r + (1 - abs(sign(theta))) / mpdistance;
				v[1] = s*src[0];
				v[0] = cos(theta);

				
				src[0] = mpdistance*atan2(v[1], v[0]);
				src[1] = mpdistance*atan(s*src[1] / sqrt(v[0] * v[0] + v[1] * v[1]));
				
				
				src[0] = src[0] + pnwidth / 2;
				src[1] = src[1] + pnheight / 2;

			
				if (src[1] < 1)
					src[1] = 1;
				if (src[1] > pnwidth / 2 )
					src[1] = pnwidth / 2 ;
				if (src[0] < 1)
					src[0] = 1;
				if (src[0] > pnwidth )
					src[0] = pnwidth ;
				
				img_undist.at<cv::Vec3b>(x-1, y-1) = img_origin.at<cv::Vec3b>(src[1]-1, src[0]-1);
			}
		}
	}
	cv::imwrite("rota.jpg", img_undist);
	std::system("pause");
	return 0;
}

#endif