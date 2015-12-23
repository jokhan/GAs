/*
 * author: jokhan
 * date: 2015.11.03
 * function:wolf.c
 */
#include "../include/wolf.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

void cal_fit_score( Wolf *wolf, const IplImage *img, const fit_area* area ) {
	wolf->fit_score = 0;
	int i = wolf->position.x;
	int j = wolf->position.y;
	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep/sizeof(uchar);
	for(i = wolf->position.x; i < (area->right + wolf->position.x); i++)
		for(j = wolf->position.y; j < (area->down + wolf->position.y); j++) {
			int tmp;
			//tmp = data[i*step + j];
			tmp = ((uchar *)(img->imageData + i*img->widthStep))[j];
			if (tmp > 0)
			//if (tmp == 255)
				wolf->fit_score ++;
//			if (cvGet2D(img, i, j).val[0] > 0)
//				wolf->fit_score ++;
		}
}
void create_wolves( Wolf *wolf, const IplImage *img, const int num_wolf, const fit_area* area ) {
	int i = 0;
	for(i = 0; i < num_wolf; i++) {
		wolf[i].position.x = rand() % (img->width - area->right);
		wolf[i].position.y = rand() % (img->height - area->down);
		cal_fit_score(&wolf[i], img, area);
	}
}

void print_wolves(const Wolf *wolf, const int num_wolf) {
	FILE *log;
	if (!(log = fopen("wolf.txt", "w"))) {
		fprintf(stderr, "Cannot open the file of log.txt");
		return ;
	}
	int i = 0;
	for (i = 0; i < num_wolf; i++) {
		fprintf(log, "%d\n", i + 1);
		fprintf(log, "position:(%d,%d)\n", wolf[i].position.x, wolf[i].position.y);
		fprintf(log, "fit_score:%d\n", wolf[i].fit_score);
	}
	fclose(log);
}

void draw_wolves(IplImage *img, const Wolf *wolf, const int num_wolf, const fit_area* area, const int num_wolf_sech) {
	int i = 0;
	int r,g,b;
	CvFont font;
	cvInitFont(&font, CV_FONT_VECTOR0, 0.5, 0.5, 0, 1, 8);
	for(i = 0; i < num_wolf; i ++) {
		r = 0;
		g = 0;
		b = 0;
		if (i == 0)
			r = 255;
		else if( i <= num_wolf_sech)
			g = 255;
		else
			b = 255;
		cvDrawRect(img, wolf[i].position, cvPoint(wolf[i].position.x + area->right, wolf[i].position.y + area->down),CV_RGB(r, g, b), 1, 8, 0);
		char num[2];
		sprintf(num, "%d(%d,%d)", i + 1, wolf[i].position.x, wolf[i].position.y);
		cvPutText(img, num, wolf[i].position, &font, CV_RGB(r,g,b) );
		//cvPutText(img, num, cvPoint(wolf[i].position.x + area->right, wolf[i].position.y + area->down), &font, CV_RGB(r,g,b) );
	}
}

int compare_wolf_desc(const void* wolf_a, const void* wolf_b) {
	Wolf* a = (Wolf*) wolf_a;
	Wolf* b = (Wolf*) wolf_b;
	return b->fit_score - a->fit_score;
}

void sort_wolf(Wolf *wolf, int num_wolf) {
	qsort(wolf, num_wolf, sizeof(Wolf), compare_wolf_desc);
}

int cal_num_wolf_sech(const int num_wolf, int rate_sech) {
	int result = 0;
	int min = 0;
	int max = 0;
	min = num_wolf / (rate_sech + 1);
	max = num_wolf / rate_sech;
	result = rand() % ( max - min ) + min;
	return result;
}

void copy_wolf(const Wolf* wolf_src, Wolf* wolf_new) {
	wolf_new->position.x = wolf_src->position.x;
	wolf_new->position.y = wolf_src->position.y;
	wolf_new->fit_score = wolf_src->fit_score;
}

void swap_wolf(Wolf* wolf_one, Wolf* wolf_another) {
	Wolf wolf_tmp;
	copy_wolf(wolf_one, &wolf_tmp);
	copy_wolf(wolf_another, wolf_one);
	copy_wolf(&wolf_tmp, wolf_another);
}

void act_one_sech(Wolf *wolf, int step_sech, const IplImage* img, const fit_area* area) {
	//向上下左右四个方向寻找
	Wolf wolf_tmp[5];
	int i = 0;
	for(i = 0; i < 5; i ++) {
		copy_wolf(wolf, &wolf_tmp[i]);
	}
	wolf_tmp[1].position.y -= step_sech; //the up one
	wolf_tmp[2].position.y += step_sech; //the down one
	wolf_tmp[3].position.x -= step_sech; //the left one
	wolf_tmp[4].position.x += step_sech; //the right one
	for(i = 1; i < 5; i++) {
		correct_out_range(&wolf[i], img, area);
		cal_fit_score(&wolf_tmp[i], img, area);
	}
	sort_wolf(wolf_tmp, 5);
	copy_wolf(&wolf_tmp[0], wolf); //保留最优的探狼

}

void act_sech_wolves(Wolf *wolf, int step_sech, const IplImage* img, const fit_area* area, int num_wolf_sech, int num_step_max) {
	int i = 0;
	bool flag = true; //是否有新的头狼出现,true为没有
	while( i < num_step_max && flag ) {
		int j = 1;
		i ++;
		for ( j = 1; j <= num_wolf_sech; j ++ ) {
			act_one_sech(&wolf[j], step_sech, img, area);
			if(wolf[j].fit_score > wolf[0].fit_score) {
				swap_wolf(&wolf[j], &wolf[0]);
				flag = false;
				break;
			}
		}
	}
}

void correct_out_range(Wolf *wolf,const IplImage* img, const fit_area* area) {
	int range_x = img->width - area->right - 1;
	int range_y = img->height - area->down - 1;
	if (wolf->position.x < 0)
		wolf->position.x = 0;
	else if (wolf->position.x > range_x)
		wolf->position.x = range_x;
	if (wolf->position.y < 0)
		wolf->position.y = 0;
	else if (wolf->position.y > range_y)
		wolf->position.y = range_y;
}

int cal_dist(const Wolf *wolf_one, const Wolf *wolf_another) {
	int dist = 0;
	dist = abs(wolf_one->position.x - wolf_another->position.x) + abs(wolf_one->position.y - wolf_another->position.y);
	return dist;
}

void act_one_run(const Wolf *wolf_leader, Wolf *wolf_run, int step_run){
	int direction_x = 1;
	int direction_y = 1;
	if (wolf_run->position.x > wolf_leader->position.y)
		direction_x = -1;
	if (wolf_run->position.y > wolf_leader->position.y)
		direction_y = -1;
	wolf_run->position.x += (step_run * direction_x);
	wolf_run->position.y += (step_run * direction_y);
}

void act_run_wolves(Wolf *wolf, int step_run, const IplImage* img, const fit_area* area,int num_wolf, int num_wolf_sech, int dist_near) {
	int num_run_finished = 0;
	int num_wolf_run = num_wolf - num_wolf_sech - 1;
	bool flag = true; //是否有新的头狼出现,true为没有
	while( num_run_finished < num_wolf_run && flag ) {
		int j = 0;
		//flag = false;
		for(j = num_wolf_sech; j < num_wolf; j ++) {
			if (cal_dist(&wolf[0], &wolf[j]) <= dist_near) {
				continue;  //该狼已经到位
			} else {
				act_one_run(&wolf[0], &wolf[j], step_run);
				correct_out_range(&wolf[j], img, area);
				cal_fit_score(&wolf[j], img, area);
				if (cal_dist(&wolf[0], &wolf[j]) <= dist_near) {
					num_run_finished ++;
				}
				if(wolf[j].fit_score > wolf[0].fit_score) {
					//头狼更换
					flag = false;
					swap_wolf(&wolf[0], &wolf[j]);
					//act_run_wolves(wolf, step_run, img, area, num_wolf, num_wolf_sech, dist_near); //所有的猛狼重新奔跑
					break;
				}
			}
		}
		
	}
}

void act_one_attack(const Wolf* wolf_leader, Wolf* wolf,const IplImage* img, int step_attack, const fit_area *area) {
	int alpha = rand() / 2 - 1;  //  -1 <= alpha <= 1
	Wolf wolf_tmp;
	wolf_tmp.position.x = wolf->position.x + alpha * step_attack * abs(wolf_leader->position.x - wolf->position.x);
	wolf_tmp.position.y = wolf->position.y + alpha * step_attack * abs(wolf_leader->position.y - wolf->position.y);
	correct_out_range(&wolf_tmp, img, area);
	cal_fit_score(&wolf_tmp, img, area);
	if (wolf_tmp.fit_score > wolf->fit_score) {
		copy_wolf(&wolf_tmp, wolf); //捕食后结果更好，则更新位置
	}

}

void act_attack_wolves(Wolf* wolves, const IplImage* img, int step_attack, const fit_area *area, int num_wolf) {
	int i = 1;
	for(i = 1; i < num_wolf; i ++) {
		//猛狼和探狼发动攻击
		act_one_attack(&wolves[0], &wolves[i], img, step_attack, area);
	}
}

void update_wolves(Wolf* wolves,const IplImage* img, const fit_area* area, int num_wolf, int rate_updt) {
	sort_wolf( wolves, num_wolf );
	int max = num_wolf / rate_updt;
	int min = num_wolf / ( 2 * rate_updt );
	int updt_number = rand() % ( max - min ) + min;
	printf("updating...\n");
	printf("updt_number:%d\n", updt_number);
	int i = updt_number;
	for(i = updt_number; i < num_wolf; i ++) {
		wolves[i].position.x = rand() % (img->width - area->right);
		wolves[i].position.y = rand() % (img->height - area->down);
		cal_fit_score(&wolves[i], img, area);
	}
	/*
	Wolf* wolves_tmp;
	create_wolves( wolves_tmp, img, updt_number, area );
	int i = 0;
	for( i = 0; i < updt_number; i++ ) {
		printf("i:%d\n", i);
		copy_wolf(&wolves_tmp[i], &wolves[i + num_wolf - updt_number]);
	}
	*/
}
