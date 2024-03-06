#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	char type[3];
	int width, height;
	int color_val;
	union {
		int **pgm;
		int **ppm;
	};
} image;

typedef struct {
	int x1, x2;
	int y1, y2;
} select;

int convert(char s[]);

int ascii_or_binary(char x);

int photo_type(char x);

void photo_copy(image *p, image *aux_p, int x1, int x2, int y1, int y2);

int **alloc_mat_grey(int n, int m);

int **alloc_mat_color(int n, int m);

void alloc_matrix(image *p, image *aux_p, int n, int m);

int **grey_mat(int n, int m, char type[], FILE *f);

int **color_mat(int n, int m, char type[], FILE *f);

void free_photo(image *p);

void LOAD_B(image *p, char file_name[]);

void LOAD_T(image *p, char file_name[]);

int LOAD(image *p, char file_name[]);

void CHECK_LOAD(int L, image *p, image *aux_p, select *s, int *ph_loaded);

void SAVE_T(image *p, char file_name[]);

void SAVE_B(image *p, char file_name[]);

int select_rule(int x1, int x2, int y1, int y2, int n, int m);

int is_number(char x[]);

int number_convert(char x[]);

int verif(char x[]);

void SELECT(image *p, select *s, int ph_loaded);

void CROP(image *p, image *aux_p, select *s, int ph_loaded);

int convert_apply(char x[]);

double clamp(double x);

void EDGE(image *p, image *aux_p, select *s);

void SHARPEN(image *p, image *aux_p, select *s);

void BLUR(image *p, image *aux_p, select *s);

void GAUSSIAN_BLUR(image *p, image *aux_p, select *s);

void APPLY(image *p, image *aux_p, select *s, char word[]);

void EQUALIZE(image *p, int ph_loaded);

void HISTOGRAM(image *p, int ph_loaded);

int angle_values(int x);

void image_rotate_section(image *p, image *aux_p, select *s);

void ROTATE_SELECTED_AREA(image *p, image *aux_p, select *s, int angle);

void image_rotate_all(image *p, image *aux_p, int angle);

void ROTATE_ALL(image *p, image *aux_p, select *s, int angle);

void ROTATE(image *p, image *aux_p, select *s, int ph_loaded);
