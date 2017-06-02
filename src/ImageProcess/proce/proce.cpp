// proce.cpp : �������̨Ӧ�ó������ڵ㡣
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
  
	IplImage *frame = NULL; //����֡,ÿ֡����һ��ͼ
	IplImage *B_part = NULL, *G_part = NULL, *R_part = NULL;
	IplImage *pFrImg = NULL; //ǰ��ͼƬ
	IplImage *pBkImg = NULL; //����ͼƬ
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


	cvNamedWindow("Camera",CV_WINDOW_AUTOSIZE); //���ô���
 
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
		dst = cvCreateImage( cvGetSize(frame), 8, 3 );//�ҵ���������������
		bg_model = (CvGaussBGModel*)cvCreateGaussianBGModel(frame, gas_params);
	}
	else
	{
		cvUpdateBGStatModel(frame, (CvBGStatModel *)bg_model );//���¸�˹����ģ��

		//pFrImgΪǰ��ͼ��ֻ��Ϊ��ͨ��
		//pBkImgΪ����ͼ�񣬿���Ϊ��ͨ������pFrameͨ������ͬ
		cvCopy(bg_model->foreground,pFrImg,0);
		cvCopy(bg_model->background,pBkImg,0);
		//��ͼ��������
		/*pBkImg->origin=1;
		pFrImg->origin=1;*/
		cvSplit(frame , B_part,G_part, R_part,0);
		//cvFlip(B_part,B_part,0);//��X�ᷭתһ��

		printf("Step 6 ���ֵ��ͼ�񣬲�����ͼ����̬����  \n");
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

		printf("Step 7 ����Ŀ����ٴ��� \n");


		printf("Step 8 ѭ�������и��ٴ����Ŀ������ж�  \n");


		printf("Step 9 �жϵ�Ŀ��λ��ͼ���·������ȡĿ��ͼ�񣬶�ͼ�����SVM�жϣ��Ƿ��������������  \n");


		printf("Step 10 ����ͼ����̬ѧ����,�ָ������ͼ��洢Ϊ����ʶ���ͼƬ�ļ�\n");


		printf("Step 11 ���ָ����������ͼ��ͨ��http�ύAPI������м�⴦��\n");
		
		int key1;
		printf("Step 12 ʶ����ɺ������һ֡ͼ��\n");
		key1 = cvWaitKey (20);
		if (key1 == 'q' || key1 == 'Q')
			break;


	}
	count++;
	    printf("Step 14 ÿ�������ύһ��ʶ�����ͳһ�Է���������м�����\n");
	}

	printf("Step 13 ʶ����ɣ��ͷ���ռ�õĶ���\n");

	cvDestroyWindow("temp");
	cvReleaseCapture(&capture); //��������ͷ
	cvDestroyWindow("Camera"); //���ٴ���
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

