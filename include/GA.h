#ifndef _GA_H_
#define _GA_H_
#include <stdio.h>
#include "wolf.h"
#include <string.h>
#include <stdbool.h>
#define GENE_LENGTH 20

typedef struct {
	char sequence[GENE_LENGTH];
	int fit_score;
} Gene;

void print();
//二进制编码方式
//20个二进制喂表示横坐标和纵坐标
void gene_split(const char* sequence, char* pointx, char* pointy); 
//一条基因中间切开
int string_to_int(const char* point);
//二进制字符串转换为int
void gene_to_point(const char* sequence, CvPoint* point);
//一条基因转化为坐标点
void cal_fit_score_gene(Gene* gene, const IplImage *img, const fit_area* area);
//计算基因的适应度分数
void create_gene(Gene* gene, const IplImage* img, fit_area* area);
//产生一个基因
void correct_gene(Gene* gene, const IplImage* img, fit_area* area);
//修正一个非法基因
void create_generation(Gene* gene, const IplImage* img, fit_area* area, int num); 
//产生一个种群

void print_gene(Gene* gene);
void str_cpy(char* dest, const char* src, int lenght);
#endif
