/*
 * author: jokhan
 * date: 2015.11.03
 * function:wolf.h
 */
#ifndef _WOLF_H_
#define _WOLF_H_

#include "opencv/cxcore.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <stdio.h>

typedef struct {
	CvPoint position;
	int fit_score;
} Wolf;

typedef struct {
	int right;
	int down;
} fit_area;

void cal_fit_score( Wolf *wolf, const IplImage *img, const fit_area* area);
void create_wolves( Wolf *wolf, const IplImage *img, const int num_wolf, const fit_area* area );
void print_wolves(const Wolf *wolf, const int num_wolf);
void draw_wolves(IplImage *img, const Wolf *wolf, const int num_wolf, const fit_area* area, const int num_wolf_sech);
int compare_wolf_desc(const void* wolf_a, const void* wolf_b);
void sort_wolf(Wolf *wolf, int num_wolf);
int cal_num_wolf_sech(const int num_wolf, int rate_sech);
void copy_wolf(const Wolf* wolf_src, Wolf* wolf_new);
void swap_wolf(Wolf* wolf_one, Wolf* wolf_another);
void act_one_sech(Wolf *wolf, int step_sech, const IplImage* img, const fit_area* area); //一只探狼游走
void act_sech_wolves(Wolf *wolf, int step_sech, const IplImage* img, const fit_area* area, int num_wolf_sech, int num_step_max); //所有探狼的游走活动
void correct_out_range(Wolf *wolf,const IplImage* img, const fit_area* area);
int cal_dist(const Wolf *wolf_one, const Wolf *wolf_another);
void act_one_run(const Wolf *wolf_leader, Wolf *wolf_run, int step_run); //一只猛狼奔跑
void act_run_wolves(Wolf *wolf, int step_run, const IplImage* img, const fit_area* area,int num_wolf, int num_wolf_sech, int dist_near); //所有探狼的奔跑行为
void act_one_attack(const Wolf* wolf_leader, Wolf* wolf,const IplImage* img, int step_attack, const fit_area *area);  //一只狼的攻击行为
void act_attack_wolves(Wolf* wolves, const IplImage* img, int step_attack, const fit_area *area, int num_wolf); //猛狼和探狼的全部攻击行为
void update_wolves(Wolf* wolves,const IplImage* img, const fit_area* area, int num_wolf, int rate_updt);  //更新狼群

#endif
