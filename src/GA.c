#include "../include/GA.h"

void print()
{
	printf("nice to meet you\n");
}

void str_cpy(char* dest, const char* src, int lenght){
	int i = 0;
	for( i = 0; i < lenght; i ++ ) {
		dest[i] = src[i];
	}
}

void gene_split(const char* gene, char* pointx, char* pointy) {
	int tmp = GENE_LENGTH / 2;
	strncpy(pointx, gene, tmp);
	pointx[tmp] = '\0';
	strncpy(pointy, &gene[tmp], tmp);
	pointy[tmp] = '\0';
}


int string_to_int(const char* point) {
	int result = 0;
	int i = 0;
	int bit = 1;
	int pointInt = atoi(point);
	while( pointInt ) {
		result += (pointInt % 10) * bit;
		pointInt /= 10;
		bit *= 2;
	}
	return result;
}

void gene_to_point(const char* sequence, CvPoint* point){
	int tmp = GENE_LENGTH / 2;
	char pointx[tmp];
	char pointy[tmp];
	//str_cpy(pointx, gene, tmp);
	gene_split(sequence, pointx, pointy);
	printf("x:%s\n", pointx);
	printf("y:%s\n", pointy);
	int tmpx = string_to_int(pointx);
	int tmpy = string_to_int(pointy);
	point->x = tmpx;
	point->y = tmpy;
	//printf("0:%d, 1:%d\n", '0','1');
	point->x = string_to_int(pointx);
	point->y = string_to_int(pointy);
	printf("x:%d, y:%d\n", tmpx, tmpy);
}

void cal_fit_score_gene(Gene* gene, const IplImage *img, const fit_area* area) {
	Wolf wolf;
	//wolf->position.x = 0;
	//wolf->position.y = 0;
	//wolf->fit_score = 0;
	CvPoint pointTmp;
	pointTmp.x = 0;
	pointTmp.y = 0;
	gene_to_point(gene->sequence, &pointTmp);
	/*
	int i = pointTmp.x;
	int j = pointTmp.y;
	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep/sizeof(uchar);
	for(i = pointTmp.x; i < (area->right + pointTmp.x); i++)
		for(j = pointTmp.y; j < (area->down + pointTmp.y); j++) {
			int tmp;
			//tmp = data[i*step + j];
			tmp = ((uchar *)(img->imageData + i*img->widthStep))[j];
			if (tmp > 0)
			//if (tmp == 255)
				gene->fit_score ++;
//			if (cvGet2D(img, i, j).val[0] > 0)
//				wolf->fit_score ++;
		}
		*/
	wolf.position.x = pointTmp.x;
	wolf.position.y = pointTmp.y;
	cal_fit_score(&wolf, img, area);
	gene->fit_score = wolf.fit_score;
}

void create_gene(Gene* gene, const IplImage* img, fit_area* area) {
	while(1) {
		char ch;
		int i = 0;
		for( i = 0; i < GENE_LENGTH; i ++ ) {
			gene->sequence[i] = rand() % 2 + 48;
		}
		gene->sequence[GENE_LENGTH] = '\0';
		CvPoint point;
		point.x = 0;
		point.y = 0;
		gene_to_point(gene->sequence, &point);
		if( (point.x < (img->width - area->right) && (point.y < (img->height - area->down))) )
			//防止非法基因
			break;
	}
	cal_fit_score_gene(gene, img, area);
}

void correct_gene(Gene* gene, const IplImage* img, fit_area* area) {
	while(1) {
		CvPoint *point;
		point->x = 0;
		point->y = 0;
		gene_to_point(gene->sequence, point);
		if( (point->x < (img->width - area->right) && (point->y < (img->height - area->down))) )
			break;
		create_gene(gene, img, area);
	}
}

void create_generation(Gene* gene, const IplImage* img, fit_area* area, int num) {
	int i = 0;
	for( i = 0; i < num; i ++ ) {
		create_gene(&gene[i], img, area);
		//correct_gene(&gene[i], img, area);
	}
}

void print_gene(Gene* gene) {
	printf("seq:%s\n", gene->sequence);
	printf("fit_score:%d\n", gene->fit_score);
}
