#include "sobel.h"
#include <cmath>
using namespace std;
using namespace cv;


int sobel_kernel_x[3][3] = {
		{ 1, 0,-1},
		{ 2, 0,-2},
		{ 1, 0,-1}};

int sobel_kernel_x_flip[3][3] = {
		{ 1, 0,-1},
		{ 0, 0,-2},
		{ 1, 0,-1}};

int sobel_kernel_y[3][3] = {
		{ 1, 2, 1},
		{ 0, 0, 0},
		{-1,-2,-1}};

int sobel_kernel_y_flip[3][3] = {
		{-1,-2,-1},
		{ 0, 0, 0},
		{ 1, 2, 1}};


void sobel(const Mat& src, Mat& dst)
{
	// The Sobel operator is used in edge detection where it creates
	// an image emphasising ededges. It uses two 3×3 kernels which are
	// convolved with the original image to calculate approximations
	// of the derivatives – one for horizontal changes, and one for
	// vertical.


	// Used to store the sum of the convlution of the kernal and
	// the input image for the x and y pixels.
	int16_t sum_x, sum_y = 0;
	
	int rows = src.rows;
	int cols = src.cols;
	int kernel_row = 3, kernel_col = 3;
	
	// Used to store the magnintude values of the output image
	Mat magnitude = Mat::zeros(src.size(), CV_8U);
	
	// Origin of impulse response is always located at
	// the center of the sample
	int kCenterX = kernel_col / 2;
	int kCenterY = kernel_row / 2;
	
	// Variables to keep track where we are
	// in the kernel matrix
	int KernelRowIndex = 0;
	int KernelColIndex = 0;
	
	// Variables to keep track where we are
	// in the image matrix
	int imgRowIndex = 0;
	int imgColIndex = 0;
	float temp = 0.0;
	// loop through each row of input image
	printf("Mag Rows & Cols %d , %d\n", magnitude.rows,magnitude.cols);
	for (int x = 0; x < rows; x++)
	{
		// loop through each column of input image
		for (int y = 0; y < cols; y++)
		{
			sum_x = 0;
			sum_y = 0;
			// The 2 loops below loop through the kernel filter
			// This loop is the outer summation of 2D convolution
			for (int i = 0; i < kernel_row; i++)
			{
				// Row index of our kernel calculated so that the
				// our kernel index is referenced left for right
				// which enables us to reference our kernel matrix
				// as if it is flipped top-for-bottom
				KernelRowIndex = kernel_row - 1 - i; 
				
				// This loop is the inner summation of 2D convolution
				for (int j = 0; j < kernel_col; j++)
				{
					// Col index of our kernel calculated so that the
					// our kernel index is referenced left for right
					// which enables us to reference our kernel matrix
					// as if it is flipped top-for-bottom
					KernelColIndex = kernel_col - 1 - j;

					// add current row index of ouput signal(x) +
					// (center of kernel - current index of kernel)
					// to end of kernel row
					imgRowIndex = x + (kCenterX - KernelRowIndex);

					// add current col index of ouput signal(x) +
					// (center of kernel - distance) from current index to
					// end of kernel col
					imgColIndex = y + (kCenterY - KernelColIndex);

					// Check to make sure the current pixel is at the center of the kernel and
					// to ensure that the we only convolve the pixels where the kernel and pixels
				    // overlap.
					if (imgRowIndex >= 0 && imgRowIndex < rows && imgColIndex >= 0 && imgColIndex < cols)
					{
						//if (x==0 && y==0 || x==1 && y==1 )
						if (x==0 && y==0)
						{
							printf("DEBUG Img Row %d\n",imgRowIndex);
							printf("DEBUG Img Col %d\n",imgColIndex);
							printf("DEBUG Sobel Value Row  %d\n",KernelRowIndex);
							printf("DEBUG Sobel Value Col  %d\n",KernelColIndex);

						}
						// Convolution accumulator for x and y
						sum_x += src.at<uint8_t>(imgRowIndex, imgColIndex) *sobel_kernel_x[KernelRowIndex][KernelColIndex];
						sum_y += src.at<uint8_t>(imgRowIndex, imgColIndex) *sobel_kernel_y[KernelRowIndex][KernelColIndex];
					}
				}

			}
			// Find the magnitude of the gradiant to determine if this pixel
			// is an edge.
			temp = sqrt((sum_x*sum_x)+(sum_y*sum_y));

			if (temp<0)
				magnitude.at<uint8_t>(x, y) = (temp < 0) ? 0 : (temp);
			else
				magnitude.at<uint8_t>(x, y) = (temp > 255) ? 255 : (temp);

			temp = 0;

		}
	}
	dst = magnitude;


}

void sobel_unroll(const Mat& src, Mat& dst)
{
// TODO
	int16_t sum_x[4]={0}, sum_y[4] = {0};
	int rows = src.rows;
	int cols = src.cols;
	int kernel_row = 3, kernel_col = 3;
	//Mat magnitude = Mat::zeros(src.size(), CV_8U);
	Mat magnitude = Mat::zeros(src.rows+2,src.cols+2, CV_8U);
	// for each image row in input image
	int kCenterX = kernel_col / 2;
	int kCenterY = kernel_row / 2;
	int KernelRowIndex = 0;
	int KernelColIndex = 0;
	int imgRowIndex = 0;
	int imgColIndex = 0;
	int16_t temp[3] = {0};
	//printf("%f, %f, %f, %f",sum_x[0],sum_x[1],sum_x[2],sum_x[3]);
	Mat image_pad = Mat::zeros(src.size(), CV_8U);
	copyMakeBorder(src,image_pad,2,2,2,2,BORDER_CONSTANT,Scalar(0));

	// loop through each row of input image
	for (int x = 0; x < rows; x++)
	{
		//printf("SRC Index, %d\n", x);
		// loop through each column of input image
		for (int y = 0; y <= cols; y+=4)
		{
		    //printf("SRC Index, %i\n", y);
            sum_x[0]+= sobel_kernel_x[0][0] * image_pad.at<uint8_t>(x,y)
                       + sobel_kernel_x[0][1] * image_pad.at<uint8_t>(x,y+1)
                       + sobel_kernel_x[0][2] * image_pad.at<uint8_t>(x,y+2)
                       + sobel_kernel_x[1][0] * image_pad.at<uint8_t>(x+1,y)
                       + sobel_kernel_x[1][1] * image_pad.at<uint8_t>(x+1,y+1)
                       + sobel_kernel_x[1][2] * image_pad.at<uint8_t>(x+1,y+2)
                       + sobel_kernel_x[2][0] * image_pad.at<uint8_t>(x+2,y)
                       + sobel_kernel_x[2][1] * image_pad.at<uint8_t>(x+2,y+1)
                       + sobel_kernel_x[2][2] * image_pad.at<uint8_t>(x+2,y+2);



            sum_x[1]+= sobel_kernel_x[0][0] * image_pad.at<uint8_t>(x,y+1)
                       + sobel_kernel_x[0][1] * image_pad.at<uint8_t>(x,y+2)
                       + sobel_kernel_x[0][2] * image_pad.at<uint8_t>(x,y+3)
                       + sobel_kernel_x[1][0] * image_pad.at<uint8_t>(x+1,y+1)
                       + sobel_kernel_x[1][1] * image_pad.at<uint8_t>(x+1,y+2)
                       + sobel_kernel_x[1][2] * image_pad.at<uint8_t>(x+1,y+3)
                       + sobel_kernel_x[2][0] * image_pad.at<uint8_t>(x+2,y+1)
                       + sobel_kernel_x[2][1] * image_pad.at<uint8_t>(x+2,y+2)
                       + sobel_kernel_x[2][2] * image_pad.at<uint8_t>(x+2,y+3);

            sum_x[2]+= sobel_kernel_x[0][0] * image_pad.at<uint8_t>(x,y+2)
                       + sobel_kernel_x[0][1] * image_pad.at<uint8_t>(x,y+3)
                       + sobel_kernel_x[0][2] * image_pad.at<uint8_t>(x,y+4)
                       + sobel_kernel_x[1][0] * image_pad.at<uint8_t>(x+1,y+2)
                       + sobel_kernel_x[1][1] * image_pad.at<uint8_t>(x+1,y+3)
                       + sobel_kernel_x[1][2] * image_pad.at<uint8_t>(x+1,y+4)
                       + sobel_kernel_x[2][0] * image_pad.at<uint8_t>(x+2,y+2)
                       + sobel_kernel_x[2][1] * image_pad.at<uint8_t>(x+2,y+3)
                       + sobel_kernel_x[2][2] * image_pad.at<uint8_t>(x+2,y+4);

			if (y != 384) 
			{
				sum_x[3]+= sobel_kernel_x[0][0] * image_pad.at<uint8_t>(x,y+3)
						   + sobel_kernel_x[0][1] * image_pad.at<uint8_t>(x,y+4)
						   + sobel_kernel_x[0][2] * image_pad.at<uint8_t>(x,y+5)
						   + sobel_kernel_x[1][0] * image_pad.at<uint8_t>(x+1,y+3)
						   + sobel_kernel_x[1][1] * image_pad.at<uint8_t>(x+1,y+4)
						   + sobel_kernel_x[1][2] * image_pad.at<uint8_t>(x+1,y+5)
						   + sobel_kernel_x[2][0] * image_pad.at<uint8_t>(x+2,y+3)
						   + sobel_kernel_x[2][1] * image_pad.at<uint8_t>(x+2,y+4)
						   + sobel_kernel_x[2][2] * image_pad.at<uint8_t>(x+2,y+5);
			}




            sum_y[0]+= sobel_kernel_y_flip[0][0] * image_pad.at<uint8_t>(x,y)
                       + sobel_kernel_y_flip[0][1] * image_pad.at<uint8_t>(x,y+1)
                       + sobel_kernel_y_flip[0][2] * image_pad.at<uint8_t>(x,y+2)
                       + sobel_kernel_y_flip[1][0] * image_pad.at<uint8_t>(x+1,y)
                       + sobel_kernel_y_flip[1][1] * image_pad.at<uint8_t>(x+1,y+1)
                       + sobel_kernel_y_flip[1][2] * image_pad.at<uint8_t>(x+1,y+2)
                       + sobel_kernel_y_flip[2][0] * image_pad.at<uint8_t>(x+2,y)
                       + sobel_kernel_y_flip[2][1] * image_pad.at<uint8_t>(x+2,y+1)
                       + sobel_kernel_y_flip[2][2] * image_pad.at<uint8_t>(x+2,y+2);



            sum_y[1]+= sobel_kernel_y_flip[0][0] * image_pad.at<uint8_t>(x,y+1)
                       + sobel_kernel_y_flip[0][1] * image_pad.at<uint8_t>(x,y+2)
                       + sobel_kernel_y_flip[0][2] * image_pad.at<uint8_t>(x,y+3)
                       + sobel_kernel_y_flip[1][0] * image_pad.at<uint8_t>(x+1,y+1)
                       + sobel_kernel_y_flip[1][1] * image_pad.at<uint8_t>(x+1,y+2)
                       + sobel_kernel_y_flip[1][2] * image_pad.at<uint8_t>(x+1,y+3)
                       + sobel_kernel_y_flip[2][0] * image_pad.at<uint8_t>(x+2,y+1)
                       + sobel_kernel_y_flip[2][1] * image_pad.at<uint8_t>(x+2,y+2)
                       + sobel_kernel_y_flip[2][2] * image_pad.at<uint8_t>(x+2,y+3);

            sum_y[2]+= sobel_kernel_y_flip[0][0] * image_pad.at<uint8_t>(x,y+2)
                       + sobel_kernel_y_flip[0][1] * image_pad.at<uint8_t>(x,y+3)
                       + sobel_kernel_y_flip[0][2] * image_pad.at<uint8_t>(x,y+4)
                       + sobel_kernel_y_flip[1][0] * image_pad.at<uint8_t>(x+1,y+2)
                       + sobel_kernel_y_flip[1][1] * image_pad.at<uint8_t>(x+1,y+3)
                       + sobel_kernel_y_flip[1][2] * image_pad.at<uint8_t>(x+1,y+4)
                       + sobel_kernel_y_flip[2][0] * image_pad.at<uint8_t>(x+2,y+2)
                       + sobel_kernel_y_flip[2][1] * image_pad.at<uint8_t>(x+2,y+3)
                       + sobel_kernel_y_flip[2][2] * image_pad.at<uint8_t>(x+2,y+4);

			if (y != 384) 
			{
				sum_y[3]+= sobel_kernel_y_flip[0][0] * image_pad.at<uint8_t>(x,y+3)
						   + sobel_kernel_y_flip[0][1] * image_pad.at<uint8_t>(x,y+4)
						   + sobel_kernel_y_flip[0][2] * image_pad.at<uint8_t>(x,y+5)
						   + sobel_kernel_y_flip[1][0] * image_pad.at<uint8_t>(x+1,y+3)
						   + sobel_kernel_y_flip[1][1] * image_pad.at<uint8_t>(x+1,y+4)
						   + sobel_kernel_y_flip[1][2] * image_pad.at<uint8_t>(x+1,y+5)
						   + sobel_kernel_y_flip[2][0] * image_pad.at<uint8_t>(x+2,y+3)
						   + sobel_kernel_y_flip[2][1] * image_pad.at<uint8_t>(x+2,y+4)
						   + sobel_kernel_y_flip[2][2] * image_pad.at<uint8_t>(x+2,y+5);
			}
			//printf("Img X, Img Y = %i, %i\ n", x,y);
			temp[0] = sqrt(pow(sum_x[0],2)+pow(sum_y[0],2));
			temp[1] = sqrt(pow(sum_x[1],2)+pow(sum_y[1],2));
			temp[2] = sqrt(pow(sum_x[2],2)+pow(sum_y[2],2));
			if (y != 384) 
			{
			temp[3] = sqrt(pow(sum_x[3],2)+pow(sum_y[3],2));
			}
			
			if (temp[0]<0)
			{
				magnitude.at<uint8_t>(x, y)   = (temp[0] < 0) ? 0 : (temp[0]);
			}
			else
			{
				magnitude.at<uint8_t>(x, y)   = (temp[0] > 255) ? 255 : (temp[0]);
			}


			if (temp[1]<0)
			{
				magnitude.at<uint8_t>(x, y+1) = (temp[1] < 0) ? 0 : (temp[1]);
			}
			else
			{
				magnitude.at<uint8_t>(x, y+1) = (temp[1] > 255) ? 255 : (temp[1]);
			}


			if (temp[2]<0)
			{
				magnitude.at<uint8_t>(x, y+2) = (temp[2] < 0) ? 0 : (temp[2]);
			}
			else
			{
				magnitude.at<uint8_t>(x, y+2) = (temp[2] > 255) ? 255 : (temp[2]);
			}

			if (y != 384) 
			{
				if (temp[3]<0)
				{
					magnitude.at<uint8_t>(x, y+3) = (temp[3] < 0) ? 0 : (temp[3]);
				}
				else
				{
					magnitude.at<uint8_t>(x, y+3) = (temp[3] > 255) ? 255 : (temp[3]);
				}
			}
			sum_x[0]   = 0;
			sum_x[1] = 0;
			sum_x[2] = 0;
			sum_x[3] = 0;
			sum_y[0]   = 0;
			sum_y[1] = 0;
			sum_y[2] = 0;
			sum_y[3] = 0;
			temp[0]    = 0;
			temp[1]  = 0;
			temp[2]  = 0;
			temp[3]  = 0;
 
		}


	}
	//magnitude.copyTo(dst,);
	//rect = new cv.Rect(3,3,381,381);
	dst=magnitude(Range(1,385),Range(1,385));
	//dst = magnitude;
}

void sobel_neon(const Mat& src, Mat& dst)
{
	// Algorithm:
	// Store the image row and multiply
	// Then do for each row 3 times 
	// Then add each 
		
	int16x8_t sum_x[8]={0}, sum_y[8]={0},data_vector_y[6]={0}, data_vector_x[6]={0};
	int16x8_t magnitude={0};
	int16x8_t img_vector_x[6]={0},img_vector_y[6]={0}; // 128 bit / 16 bit = 8 vectors. Image bits are signed 16 because of negative values from kernel 
	int16_t temp[8]={0},accy[8]={0},accx[8] = {0}, x_result = 0, y_result =0;
	
	Mat image_pad = Mat::zeros(src.size(), CV_16S); // Initialize zero pad matrix for image 
	Mat dst_temp = Mat::zeros(src.rows+2,src.cols+2, CV_8U); // Initialize zero pad matrix for image 
	src.convertTo(image_pad, CV_16S); // need to keep intrinsic types same dimensions
	copyMakeBorder(image_pad,image_pad,2,2,2,2,BORDER_CONSTANT,Scalar(0)); // Zero Pad Image to not lose border bits 
	
	int rows = src.rows;
	int cols = src.cols;
	for (int x = 0; x < rows; x++) // loop through each row of input image
	{
		for (int y = 0; y <= cols; y+=8) // loop through each column of input image
		{
			img_vector_x[0] = vld1q_s16((&image_pad.at<int16_t>(x,y)));
			img_vector_x[1] = vld1q_s16((&image_pad.at<int16_t>(x+1,y)));
			img_vector_x[2] = vld1q_s16((&image_pad.at<int16_t>(x+2,y)));
			img_vector_x[3] = vld1q_s16((&image_pad.at<int16_t>(x,y+2)));
			img_vector_x[4] = vld1q_s16((&image_pad.at<int16_t>(x+1,y+2)));
			img_vector_x[5] = vld1q_s16((&image_pad.at<int16_t>(x+2,y+2)));
		
			data_vector_x[0] = vmulq_n_s16(img_vector_x[0],1);
			data_vector_x[1] = vmulq_n_s16(img_vector_x[3],-1);
			data_vector_x[2] = vmulq_n_s16(img_vector_x[1],2);
			data_vector_x[3] = vmulq_n_s16(img_vector_x[4],-2);
			data_vector_x[4] = vmulq_n_s16(img_vector_x[2],1);
			data_vector_x[5] = vmulq_n_s16(img_vector_x[5],-1);

			sum_x[0] = vaddq_s16(data_vector_x[0],data_vector_x[1]);
			sum_x[1] = vaddq_s16(sum_x[0],data_vector_x[2]);
			sum_x[2] = vaddq_s16(sum_x[1],data_vector_x[3]);
			sum_x[3] = vaddq_s16(sum_x[2],data_vector_x[4]);
			sum_x[4] = vaddq_s16(sum_x[3],data_vector_x[5]);
			img_vector_y[0] = vld1q_s16(&image_pad.at<int16_t>(x,y));
			img_vector_y[1] = vld1q_s16(&image_pad.at<int16_t>(x,y+1));
			img_vector_y[2] = vld1q_s16(&image_pad.at<int16_t>(x,y+2));
			img_vector_y[3] = vld1q_s16(&image_pad.at<int16_t>(x+2,y));
			img_vector_y[4] = vld1q_s16(&image_pad.at<int16_t>(x+2,y+1));
			img_vector_y[5] = vld1q_s16(&image_pad.at<int16_t>(x+2,y+2));
			data_vector_y[0] = vmulq_n_s16(img_vector_y[0],1);
			data_vector_y[1] = vmulq_n_s16(img_vector_y[1],2);
			data_vector_y[2] = vmulq_n_s16(img_vector_y[2],1);
			data_vector_y[3] = vmulq_n_s16(img_vector_y[3],-1);
			data_vector_y[4] = vmulq_n_s16(img_vector_y[4],-2);
			data_vector_y[5] = vmulq_n_s16(img_vector_y[5],-1);
			
			sum_y[0] = vaddq_s16(data_vector_y[0],data_vector_y[1]);
			sum_y[1] = vaddq_s16(sum_y[0],data_vector_y[2]);
			sum_y[2] = vaddq_s16(sum_y[1],data_vector_y[3]);
			sum_y[3] = vaddq_s16(sum_y[2],data_vector_y[4]);
			sum_y[4] = vaddq_s16(sum_y[3],data_vector_y[5]);
			
			vst1q_s16(&accx[0],sum_x[4]);
			vst1q_s16(&accy[0],sum_y[4]);
			temp[0]=sqrt(pow(accx[0],2)+pow(accy[0],2));
			temp[1]=sqrt(pow(accx[1],2)+pow(accy[1],2));
			temp[2]=sqrt(pow(accx[2],2)+pow(accy[2],2));
			temp[3]=sqrt(pow(accx[3],2)+pow(accy[3],2));
			temp[4]=sqrt(pow(accx[4],2)+pow(accy[4],2));
			temp[5]=sqrt(pow(accx[5],2)+pow(accy[5],2));
			temp[6]=sqrt(pow(accx[6],2)+pow(accy[6],2));
			//temp[7]=sqrt(pow(accx[7],2)+pow(accy[7],2));

			if (y < 384) 
			{
				temp[7]=sqrt(pow(accx[7],2)+pow(accy[7],2));
			}

			if (temp[0]<0)
			{
				dst_temp.at<uint8_t>(x, y)   = (temp[0] < 0) ? 0 : (temp[0]);
			}
			else
			{
				dst_temp.at<uint8_t>(x, y)   = (temp[0] > 255) ? 255 : (temp[0]);
			}


			if (temp[1]<0)
			{
				dst_temp.at<uint8_t>(x, y+1) = (temp[1] < 0) ? 0 : (temp[1]);
			}
			else
			{
				dst_temp.at<uint8_t>(x, y+1) = (temp[1] > 255) ? 255 : (temp[1]);
			}


			if (temp[2]<0)
			{
				dst_temp.at<uint8_t>(x, y+2) = (temp[2] < 0) ? 0 : (temp[2]);
			}
			else
			{
				dst_temp.at<uint8_t>(x, y+2) = (temp[2] > 255) ? 255 : (temp[2]);
			}

			if (temp[3]<0)
			{
				dst_temp.at<uint8_t>(x, y+3) = (temp[3] < 0) ? 0 : (temp[3]);
			}
			else
			{
				dst_temp.at<uint8_t>(x, y+3) = (temp[3] > 255) ? 255 : (temp[3]);
			}
			if (temp[4]<0)
			{
				dst_temp.at<uint8_t>(x, y+4) = (temp[4] < 0) ? 0 : (temp[4]);
			}
			else
			{
				dst_temp.at<uint8_t>(x, y+4) = (temp[4] > 255) ? 255 : (temp[4]);
			}
			if (temp[5]<0)
			{
				dst_temp.at<uint8_t>(x, y+5) = (temp[5] < 0) ? 0 : (temp[5]);
			}
			else
			{
				dst_temp.at<uint8_t>(x, y+5) = (temp[5] > 255) ? 255 : (temp[5]);
			}
			if (temp[6]<0)
			{
				dst_temp.at<uint8_t>(x, y+6) = (temp[6] < 0) ? 0 : (temp[6]);
			}
			else
			{
				dst_temp.at<uint8_t>(x, y+6) = (temp[6] > 255) ? 255 : (temp[6]);
			}
			if (y < 384) 
			{
				if (temp[7]<0)
				{
					dst_temp.at<uint8_t>(x, y+7) = (temp[7] < 0) ? 0 : (temp[7]);
				}
				else
				{
					dst_temp.at<uint8_t>(x, y+7) = (temp[7] > 255) ? 255 : (temp[7]);
				}
			}
			memset(sum_x, 0, sizeof(sum_x));
			memset(sum_y, 0, sizeof(sum_y));
			memset(temp, 0, sizeof(temp));
			
		}
	}
	dst_temp(Range(1,385),Range(1,385)).convertTo(dst_temp,CV_8U);
	dst = dst_temp;
}	
