/*
 * author: jokhan
 * date: 2015.11.25 20:12
 * function: main.c
 */
#include <stdio.h>
#include "./include/GA.h"
#include <opencv2/core/core_c.h>
#include <time.h>


int main()
{
	const int image_max = 1024;
	const int gene_length = 20;
	const int gene_num = 50;   //基因个数
	fit_area area = {100,100};
	
	srand((unsigned) time(NULL));

	int i = 0;
	int j = 49;
	char ch = j;
	printf("%c\n", ch);
	//print();
	char gene[GENE_LENGTH] = "01100000001111011011";
	gene[GENE_LENGTH] = '\0';
	CvPoint point = {0,0};
	gene_to_point( gene, &point);

	/*********Create a source image************/
	int img_width = 1000;
	int img_height = 1000;
	IplImage* src = cvCreateImage( cvSize(img_width, img_height), IPL_DEPTH_8U, 3 );
	cvZero(src);
	cvCircle(src, cvPoint(500, 500), 50, CV_RGB(255, 255, 255), -1, 8, 0);
	//cvDrawRect(src1, cvPoint(100, 100), cvPoint(300, 300), CV_RGB(255, 255, 255), -1, 8, 0);
	//cvSaveImage("source.jpg", src1, 0);
	//IplImage* src = cvLoadImage("source.jpg", 1);
	IplImage* img = cvCreateImage( cvSize(img_width, img_height), IPL_DEPTH_8U, 1 );
	cvCvtColor(src, img, CV_BGR2GRAY);
	/****************************************/

	Gene *gene1;
	create_gene(gene1, img, &area);
//	create_generation(gene1, img, &area, 1);
	print_gene(gene1);



	return 0;
}
