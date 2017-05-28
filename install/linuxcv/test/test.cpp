#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/video/background_segm.hpp"
#include "opencv2/legacy/blobtrack.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <iostream>
using namespace cv;
using namespace std;

IplImage *g_pGrayImage = NULL;  
IplImage *g_pBinaryImage = NULL;  
const char *pstrWindowsBinaryTitle = "Threshold";  
  
void on_trackbar(int pos)  
{  
 
    cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);  

    cvShowImage(pstrWindowsBinaryTitle, g_pBinaryImage);  
}  
  
int main2(  )  
{     
    const char *pstrWindowsSrcTitle = "source images";  
    const char *pstrWindowsToolBarName = "Threshold";  
  

    IplImage *pSrcImage = cvLoadImage("1.jpg", CV_LOAD_IMAGE_UNCHANGED);  
  

    g_pGrayImage =  cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);  
    cvCvtColor(pSrcImage, g_pGrayImage, CV_BGR2GRAY);  
  

    g_pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);  
  

    cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);  
    cvShowImage(pstrWindowsSrcTitle, pSrcImage);  

    cvNamedWindow(pstrWindowsBinaryTitle, CV_WINDOW_AUTOSIZE);  
  
  
    int nThreshold = 128;  
    cvCreateTrackbar(pstrWindowsToolBarName, pstrWindowsBinaryTitle, &nThreshold, 254, on_trackbar);  
  
    on_trackbar(1);  
  
    cvWaitKey(0);  
  
    cvDestroyWindow(pstrWindowsSrcTitle);  
    cvDestroyWindow(pstrWindowsBinaryTitle);  
    cvReleaseImage(&pSrcImage);  
    cvReleaseImage(&g_pGrayImage);  
    cvReleaseImage(&g_pBinaryImage);  
    return 0;  
}  

int main (int argc, char **argv)
{
	main2();
	return 0;
    Mat image, image_gray;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR );
    if (argc != 2 || !image.data) {
        //cout << "No image data\n";
        return -1;
    }

    cvtColor(image, image_gray, CV_RGB2GRAY);
    namedWindow("image", CV_WINDOW_AUTOSIZE);
    namedWindow("image gray", CV_WINDOW_AUTOSIZE);

    imshow("image", image);
    imshow("image gray", image_gray);

    waitKey(0);
    return 0;
}