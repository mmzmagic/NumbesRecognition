// proce.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/legacy/blobtrack.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <stdio.h>
#include "ChttpClient.h"
#include "curl/curl.h"




using namespace std;
using namespace cv;



int _tmain(int argc, _TCHAR* argv[])


{
	string ApiSvrUrl="http://127.0.0.1:18900/api/"; 
	CHttpClient* httper=new CHttpClient();
	string lhttpRsb;				
 
	
	/*Step 1 get the arg =================================================================================================================*/
	//Param1  VideoFilename
	//Param2  aradid_Carmerid
	//param3  recordstarttime
	//param4  recordlentime , could be 0 , auto get from video heads
	CvCapture* capture=cvCreateFileCapture("D:/v/HockeySample1.mp4");
	printf("Step 1 get the args params and Check params is valide\n");

	//Tell APIsvr Record BeginAnalysis  Some Video to DB
	if(httper->Get(ApiSvrUrl+"BeginAnalysis",lhttpRsb)!=CURLE_OK)
	{
		printf("BeginAnalysis To ApiSvr Error!");
	}

	/*Step 2 Define the local varibles====================================================================================================*/	
	printf("Step 2 Define the local varibles\n");
	CvSize czSize;               
	double fScale = 0.314;       
	double m_threshold=30;
	IplImage *pDstImage = NULL;  
	IplImage *pframe = NULL;
	IplImage *m_pFGMask=NULL;
	CvMat *m_pBkMat;
	CvMat *m_pFGMat;
	CvMat *m_pFrameMat;
	int m_framecount=0;
	czSize.width = 352;  
	czSize.height =240;      
  
	IplImage *frame = NULL; //定义帧,每帧就是一张图
	IplImage *B_part = NULL, *G_part = NULL, *R_part = NULL;
	IplImage *pFrImg = NULL; //前景图片
	IplImage *pBkImg = NULL; //背景图片
	IplImage *dst = NULL;

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;	 
	CvRect contourRect = cvRect(0,0,0,0);

 CvGaussBGStatModelParams * gas_params=new CvGaussBGStatModelParams();
		gas_params->n_gauss=5;
		gas_params->bg_threshold=0.7;
		gas_params->std_threshold=2.5;
		gas_params->weight_init=0.1;
		gas_params->variance_init=1;
		gas_params->win_size=100;


	cvNamedWindow("Camera",CV_WINDOW_AUTOSIZE); //设置窗口
 
	cvNamedWindow("foreground",1);
 
	cvMoveWindow("foreground", 690, 0);


	 
	CvGaussBGModel *bg_model=NULL;


	printf("Step 3 Read the Frame and Loop all video \n");
	
	int count = 1;
while (frame = cvQueryFrame(capture))
{
	if(pDstImage==NULL)
	  pDstImage = cvCreateImage(czSize, frame->depth, frame->nChannels);  
	
	printf("Step 4 resize the Frame For after case Speed \n");
	cvResize(frame, pDstImage, CV_INTER_AREA);   
	frame=pDstImage;
	printf("Step 5 case The Frame background and foreground  \n");
	if(count ==1)
	{
		B_part = cvCreateImage(cvGetSize(frame),frame->depth,1);
		G_part = cvCreateImage(cvGetSize(frame),frame->depth,1);
		R_part = cvCreateImage(cvGetSize(frame),frame->depth,1);
		pBkImg = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U,3);
		pFrImg = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U,1);
		dst = cvCreateImage( cvGetSize(frame), 8, 3 );//找到轮廓并绘制轮廓
		bg_model = (CvGaussBGModel*)cvCreateGaussianBGModel(frame, gas_params);
	}
	else
	{
		cvUpdateBGStatModel(frame, (CvBGStatModel *)bg_model );//更新高斯背景模型

		//pFrImg为前景图像，只能为单通道
		//pBkImg为背景图像，可以为单通道或与pFrame通道数相同
		cvCopy(bg_model->foreground,pFrImg,0);
		cvCopy(bg_model->background,pBkImg,0);
		//把图像正过来
		/*pBkImg->origin=1;
		pFrImg->origin=1;*/
		cvSplit(frame , B_part,G_part, R_part,0);
		//cvFlip(B_part,B_part,0);//沿X轴翻转一次

		printf("Step 6 活动二值化图像，并进行图像形态处理  \n");
		cvThreshold(B_part,B_part, 15, 255,CV_THRESH_BINARY );	 
		IplConvKernel * kenel = cvCreateStructuringElementEx(3,1,1,0,CV_SHAPE_RECT,NULL);
		IplConvKernel * kenel2 = cvCreateStructuringElementEx(1,3,0,1,CV_SHAPE_RECT,NULL);
		cvErode(B_part, B_part,kenel,1 );
		cvDilate(B_part,B_part,kenel,1);
		cvErode(B_part, B_part,kenel2, 1);
		cvErode(B_part, B_part,kenel,1 );
		cvDilate(B_part,B_part,kenel2,2);
		cvDilate(B_part,B_part,kenel2,2);
		cvDilate(B_part,B_part,NULL,1);		 
		cvShowImage("Camera", frame); 
		cvShowImage("foreground", pFrImg);

		printf("Step 7 进行目标跟踪处理 \n");


		printf("Step 8 循环对所有跟踪处理的目标进行判断  \n");


		printf("Step 9 判断当目标位于图像下方，则截取目标图像，对图像进行SVM判断，是否包含清晰的数字  \n");


		printf("Step 10 进行图像形态学处理,分割出单个图像存储为合适识别的图片文件\n");


		printf("Step 11 将分割出来的数字图像通过http提交API服务进行检测处理\n");
		
		int key1;
		printf("Step 12 识别完成后继续下一帧图像\n");
		key1 = cvWaitKey (20);
		if (key1 == 'q' || key1 == 'Q')
			break;


	}
	count++;
	    printf("Step 14 每隔几秒提交一次识别进度统一对分析任务进行监测管理\n");
	}

	printf("Step 13 识别完成，释放所占用的对象\n");

	cvDestroyWindow("temp");
	cvReleaseCapture(&capture); //销毁摄像头
	cvDestroyWindow("Camera"); //销毁窗口
	cvReleaseBGStatModel((CvBGStatModel**)&bg_model);
	cvDestroyWindow("background");
	cvDestroyWindow("foreground");
	cvReleaseImage(&pFrImg);
	cvReleaseImage(&pBkImg);
	cvReleaseImage(&dst);
	cvDestroyWindow("contours");
	delete httper;
	//CTrafficsampleDlg::OnOK();
		return 0;
} 

