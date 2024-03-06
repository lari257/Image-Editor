// COPYRIGHT: Larisa Florea

#include "image_editor.h"

// Since C switch does not allow strings, a function is implemented
// to associate each command with a number for easier processing.
int convert(char s[])
{
	if (strcmp(s, "LOAD") == 0)
		return 1;
	if (strcmp(s, "SELECT") == 0)
		return 2;
	if (strcmp(s, "HISTOGRAM") == 0)
		return 3;
	if (strcmp(s, "EQUALIZE") == 0)
		return 4;
	if (strcmp(s, "ROTATE") == 0)
		return 5;
	if (strcmp(s, "CROP") == 0)
		return 6;
	if (strcmp(s, "APPLY") == 0)
		return 7;
	if (strcmp(s, "SAVE") == 0)
		return 8;
	if (strcmp(s, "EXIT") == 0)
		return 9;
	return -1;
}

// maximum of 2 numbers
int MAX(int x, int y)
{
	if (x > y)
		return x;
	return y;
}

// minimum of 2 numbers
int MIN(int x, int y)
{
	if (x < y)
		return x;
	return y;
}

// verify if a character is ascii or binary
int ascii_or_binary(char x)
{
	if (x == '2' || x == '3')
		return 1;
	return 0;
}

// type of image: RGB or GREY-SCALE
int photo_type(char x)
{
	if (x == '2' || x == '5')
		return 1; // grey-scale

	if (x == '3' || x == '6')
		return 0; // RGB

	return -1;
}

// copy the values from the original image to the auxiliary image
void photo_copy(image *p, image *aux_p, int x1, int x2, int y1, int y2)
{
	strcpy(aux_p->type, p->type);
	aux_p->color_val = p->color_val;
	aux_p->width = x2 - x1;
	aux_p->height = y2 - y1;

	// copy the matrix
	if (photo_type(p->type[1]) == 1) {
		for (int i = y1; i < y2; i++)
			for (int j = x1; j < x2; j++)
				aux_p->pgm[i - y1][j - x1] = p->pgm[i][j];
	} else {
		for (int i = y1; i < y2; i++)
			for (int j = 3 * x1; j < 3 * x2; j++)
				aux_p->ppm[i - y1][j - 3 * x1] = p->ppm[i][j];
	}
}

// dynamically allocates a matrix for gray-scale images.
int **alloc_mat_grey(int n, int m)
{
	int **mat;
	mat = (int **)malloc(n * sizeof(int *));
	if (!mat)
		return NULL;

	for (int i = 0; i < n; i++) {
		mat[i] = (int *)malloc(m * sizeof(int));
		if (!mat[i]) {
			for (int j = 0; j <= i; j++)
				free(mat[j]);
			free(mat);
			return NULL;
		}
	}
	return mat;
}

// dynamically allocates a matrix for RGB images
int **alloc_mat_color(int n, int m)
{
	int **mat;
	mat = (int **)malloc(n * sizeof(int *));
	if (!mat)
		return NULL;

	for (int i = 0; i < n; i++) {
		mat[i] = (int *)malloc(3 * m * sizeof(int));
		if (!mat[i]) {
			for (int j = 0; j <= i; j++)
				free(mat[j]);
			free(mat);
			return NULL;
		}
	}

	return mat;
}

// Dynamically allocates the proper type of image matrix based on the photo type
void alloc_matrix(image *p, image *aux_p, int n, int m)
{
	if (photo_type(p->type[1]) == 1)
		aux_p->pgm = alloc_mat_grey(n, m);
	else
		aux_p->ppm = alloc_mat_color(n, m);
}

// Reads and loads a gray-scale image matrix from a file.
int **grey_mat(int n, int m, char type[], FILE *f)
{
	int **mat;
	mat = alloc_mat_grey(n, m);

	if (ascii_or_binary(type[1]) == 1) {
		// text file
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				fscanf(f, "%d", &mat[i][j]);
	} else {
		// binary file
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				mat[i][j] = 0;

		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				fread(&mat[i][j], sizeof(unsigned char), 1, f);
	}

	return mat;
}

// Reads and loads an RGB image matrix from a file.
int **color_mat(int n, int m, char type[], FILE *f)
{
	int **mat;
	mat = alloc_mat_color(n, m);

	if (ascii_or_binary(type[1]) == 1) {
		// text file
		for (int i = 0; i < n; i++)
			for (int j = 0; j < 3 * m; j++)
				fscanf(f, "%d", &mat[i][j]);
	} else {
		// binary file
		for (int i = 0; i < n; i++)
			for (int j = 0; j < 3 * m; j++)
				mat[i][j] = 0;

		for (int i = 0; i < n; i++)
			for (int j = 0; j < 3 * m; j++)
				fread(&mat[i][j], sizeof(unsigned char), 1, f);
	}
	return mat;
}

// Frees allocated memory for an image.
void free_photo(image *p)
{
	if (photo_type(p->type[1]) == 1) {
		for (int i = 0; i < p->height; i++)
			free(p->pgm[i]);
		free(p->pgm);
	} else {
		for (int i = 0; i < p->height; i++)
			free(p->ppm[i]);
		free(p->ppm);
	}
}

// Loads a binary image file
// Opens file, reads metadata and pixel data, then closes the file
void LOAD_B(image *p, char file_name[])
{
	FILE *f;
	f = fopen(file_name, "rb");

	fscanf(f, "%s", p->type); // type of image

	fscanf(f, "%d%d", &p->width, &p->height); // dimensions

	fscanf(f, "%d", &p->color_val); // maximum index

	fgetc(f);
	if (photo_type(p->type[1]) == 1)
		p->pgm = grey_mat(p->height, p->width, p->type, f);
	else
		p->ppm = color_mat(p->height, p->width, p->type, f);

	fclose(f);

	printf("Loaded %s\n", file_name);
}

// Loads an ASCII image file
void LOAD_T(image *p, char file_name[])
{
	FILE *f;
	f = fopen(file_name, "rt");

	fscanf(f, "%s", p->type); // type of image

	fscanf(f, "%d%d", &p->width, &p->height); // dimensions

	fscanf(f, "%d", &p->color_val); // maximum index

	if (photo_type(p->type[1]) == 1)
		p->pgm = grey_mat(p->height, p->width, p->type, f);
	else
		p->ppm = color_mat(p->height, p->width, p->type, f);

	fclose(f);

	printf("Loaded %s\n", file_name);
}

// Loads an image file
int LOAD(image *p, char file_name[])
{
	FILE *f;
	f = fopen(file_name, "r");
	if (!f) {
		printf("Failed to load %s\n", file_name);
		return 0;
	}

	char type[3];
	fscanf(f, "%s", type);
	fclose(f);

	if (photo_type(type[1]) == -1) {
		printf("Failed to load %s\n", file_name);
		return 0;
	}

	if (ascii_or_binary(type[1]) == 1)
		LOAD_T(p, file_name);
	else
		LOAD_B(p, file_name);

	return 1;
}

// verify if LOAD is valid
void CHECK_LOAD(int L, image *p, image *aux_p, select *s, int *ph_loaded)
{
	if (L == 1) {
		*ph_loaded = 1;
		alloc_matrix(p, aux_p, p->height, p->width);
		photo_copy(p, aux_p, 0, p->width, 0, p->height);
		// the initial selection is the entire image
		s->x1 = 0; s->x2 = p->width; s->y1 = 0; s->y2 = p->height;
	} else {
		*ph_loaded = 0;
	}

	getchar();
}

// Saves an image as a text file
void SAVE_T(image *p, char file_name[])
{
	FILE *f;
	f = fopen(file_name, "wt");
	
	// if the loaded image was ascii, 
	// the "magic word" must be changed
	if (ascii_or_binary(p->type[1]) == 0)
		p->type[1] -= 3;

	fprintf(f, "%s\n", p->type); 
	fprintf(f, "%d %d\n", p->width, p->height); 
	fprintf(f, "%d\n", p->color_val); 

	if (photo_type(p->type[1]) == 1) {
		for (int i = 0; i < p->height; i++) {
			for (int j = 0; j < p->width; j++)
				fprintf(f, "%d ", p->pgm[i][j]);
			fprintf(f, "\n");
		}
	} else {
		for (int i = 0; i < p->height; i++) {
			for (int j = 0; j < 3 * p->width; j++)
				fprintf(f, "%d ", p->ppm[i][j]);
			fprintf(f, "\n");
		}
	}

	fclose(f);

	printf("Saved %s\n", file_name);

	getchar();
}

// Saves an image as a binary file
void SAVE_B(image *p, char file_name[])
{
	FILE *f;
	f = fopen(file_name, "wb");

	/// if the loaded image was binary,
	// the "magic word" must be changed
	if (ascii_or_binary(p->type[1]) == 1)
		p->type[1] += 3;

	fprintf(f, "%s\n", p->type);
	fprintf(f, "%d %d\n", p->width, p->height);
	fprintf(f, "%d\n", p->color_val);

	if (photo_type(p->type[1]) == 1) {
		for (int i = 0; i < p->height; i++)
			for (int j = 0; j < p->width; j++)
				fwrite(&p->pgm[i][j], sizeof(unsigned char), 1, f);
	} else {
		for (int i = 0; i < p->height; i++)
			for (int j = 0; j < 3 * p->width; j++)
				fwrite(&p->ppm[i][j], sizeof(unsigned char), 1, f);
	}

	fclose(f);

	printf("Saved %s\n", file_name);
}

// conditions for Select
int select_rule(int x1, int x2, int y1, int y2, int n, int m)
{
	if (y1 == y2 || x1 == x2)
		return 0;
	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
		return 0;
	if (y1 > m || y2 > m || x1 > n || x2 > n)
		return 0;
	return 1;
}

int is_number(char x[])
{
	int j = 0;
	if (x[0] == '-')
		j = 1;

	int nr = strlen(x);
	for (int i = j; i < nr; i++)
		if (!(x[i] >= '0' && x[i] <= '9'))
			return 0;
	return 1;
}

int number_convert(char x[])
{
	int sign = 1, j = 0, i;
	if (x[0] == '-') {
		sign = -1;
		j = 1;
	}

	int number = 0;
	int nr = strlen(x);
	for (i = j; i < nr; i++)
		number = number * 10 + (x[i] - '0');
	number *= sign;
	return number;
}

int verif(char x[])
{
	char line[40], c;

	if (is_number(x) == 0) {
		printf("Invalid command\n");
		fgets(line, sizeof(line - 2), stdin);
		return -1;
	}

	c = getchar();
	if (c == '\n') {
		printf("Invalid command\n");
		return 0;
	}
	return 1;
}

// select a region of the image
void SELECT(image *p, select *s, int ph_loaded)
{
	char word[40];
	scanf("%s", word);

	char line[40];
	// verify if we have an image loaded
	if (ph_loaded == 0) {
		printf("No image loaded\n");
		fgets(line, sizeof(line), stdin);
		return;
	}

	// if the command is "SELECT ALL" the entire image is selected
	if (strcmp(word, "ALL") == 0) {
		s->x1 = 0; s->x2 = p->width; s->y1 = 0; s->y2 = p->height;
		printf("Selected ALL\n");
		return;
	}

	int a1 = 0, a2 = 0, b1 = 0, b2 = 0;

	// verify if the command is valid
	if (verif(word) != 1)
		return;
	a1 = number_convert(word);

	scanf("%s", word);
	if (verif(word) != 1)
		return;
	b1 = number_convert(word);

	scanf("%s", word);
	if (verif(word) != 1)
		return;
	a2 = number_convert(word);

	scanf("%s", word);
	if (is_number(word) == 0) {
		printf("Invalid command\n");
		fgets(line, sizeof(line - 2), stdin);
		return;
	}
	getchar();
	b2 = number_convert(word);

	if (select_rule(a1, a2, b1, b2, p->width, p->height) == 0) {
		printf("Invalid set of coordinates\n");
		return;
	}

	s->x1 = MIN(a1, a2); s->x2 = MAX(a1, a2);
	s->y1 = MIN(b1, b2); s->y2 = MAX(b1, b2);

	printf("Selected %d %d %d %d\n", s->x1, s->y1, s->x2, s->y2);
}

// Crop the selected region of the image
void CROP(image *p, image *aux_p, select *s, int ph_loaded)
{
	// verify if we have an image loaded
	if (ph_loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	free_photo(aux_p);
	aux_p->width = s->x2 - s->x1;
	aux_p->height = s->y2 - s->y1;

	alloc_matrix(p, aux_p, aux_p->height, aux_p->width);

	// copy the selected region of the image to the auxiliary image
	photo_copy(p, aux_p, s->x1, s->x2, s->y1, s->y2);

	free_photo(p);

	alloc_matrix(aux_p, p, aux_p->height, aux_p->width);

	// We load the initial matrix with the auxiliary one
	// in which we have stored the selected area of the image
	photo_copy(aux_p, p, 0, aux_p->width, 0, aux_p->height);
	s->x1 = 0; s->x2 = aux_p->width; s->y1 = 0; s->y2 = aux_p->height;

	printf("Image cropped\n");

	getchar();
}

int convert_apply(char x[])
{
	if (strcmp(x, "EDGE") == 0)
		return 1;
	if (strcmp(x, "SHARPEN") == 0)
		return 2;
	if (strcmp(x, "BLUR") == 0)
		return 3;
	if (strcmp(x, "GAUSSIAN_BLUR") == 0)
		return 4;
	return -1;
}

double clamp(double x)
{
	if (x < 0)
		return 0;
	if (x > 255)
		return 255;
	return round(x);
}

void EDGE(image *p, image *aux_p, select *s)
{
	// the specific matrix
	int kernel[3][3];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1)
				kernel[i][j] = 8;
			else
				kernel[i][j] = -1;
		}

	// vectors of directions to find the 8 neighbors of an element
	int di[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	int dj[9] = { -3, 0, 3, -3, 0, 3, -3, 0, 3};
	double sum;

	free_photo(aux_p);
	aux_p->ppm = alloc_mat_color(p->height, p->width);
	photo_copy(p, aux_p, 0, p->width, 0, p->height);

	for (int i = s->y1 ; i < s->y2; i++)
		for (int j = 3 * s->x1; j < 3 * s->x2; j += 3) {
			if (i > 0 && i < p->height - 1 && j > 2 && j < 3 * (p->width - 1)) {
				for (int t = 0; t < 3; t++) {
					sum = 0;
					for (int k = 0; k < 9; k++) {
						int x = i + di[k];
						int y = j + t + dj[k];
						sum += (double)aux_p->ppm[x][y] * kernel[k / 3][k % 3];
					}
					double rez = sum;
					p->ppm[i][j + t] = (int)clamp(rez);
				}
			}
		}
}

void SHARPEN(image *p, image *aux_p, select *s)
{
	// the specific matrix
	int kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

	// vectors of directions to find the 8 neighbors of an element
	int di[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	int dj[9] = {-3, 0, 3, -3, 0, 3, -3, 0, 3};
	double sum;

	free_photo(aux_p);
	aux_p->ppm = alloc_mat_color(p->height, p->width);
	photo_copy(p, aux_p, 0, p->width, 0, p->height);

	for (int i = s->y1 ; i < s->y2; i++)
		for (int j = 3 * s->x1; j < 3 * s->x2; j += 3) {
			if (i > 0 && i < p->height - 1 && j > 2 && j < 3 * (p->width - 1)) {
				for (int t = 0; t < 3; t++) {
					sum = 0;
					for (int k = 0; k < 9; k++) {
						int x = i + di[k];
						int y = j + t + dj[k];
						sum += (double)aux_p->ppm[x][y] * kernel[k / 3][k % 3];
					}
					double rez = sum;
					p->ppm[i][j + t] = (int)clamp(rez);
				}
			}
		}
}

void BLUR(image *p, image *aux_p, select *s)
{
	// the specific matrix
	int kernel[3][3];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = 1;

	// vectors of directions to find the 8 neighbors of an element
	int di[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	int dj[9] = { -3, 0, 3, -3, 0, 3, -3, 0, 3};
	double sum;

	free_photo(aux_p);
	aux_p->ppm = alloc_mat_color(p->height, p->width);
	photo_copy(p, aux_p, 0, p->width, 0, p->height);

	for (int i = s->y1 ; i < s->y2; i++)
		for (int j = 3 * s->x1; j < 3 * s->x2; j += 3) {
			if (i > 0 && i < p->height - 1 && j > 2 && j < 3 * (p->width - 1)) {
				for (int t = 0; t < 3; t++) {
					sum = 0;
					for (int k = 0; k < 9; k++) {
						int x = i + di[k];
						int y = j + t + dj[k];
						sum += (double)aux_p->ppm[x][y] * kernel[k / 3][k % 3];
					}
					double rez = (double)sum / 9;
					p->ppm[i][j + t] = (int)clamp(rez);
				}
			}
		}
}

void GAUSSIAN_BLUR(image *p, image *aux_p, select *s)
{
	// the specific matrix
	int kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

	// vectors of directions to find the 8 neighbors of an element
	int di[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	int dj[9] = { -3, 0, 3, -3, 0, 3, -3, 0, 3};
	double sum;

	free_photo(aux_p);
	aux_p->ppm = alloc_mat_color(p->height, p->width);
	photo_copy(p, aux_p, 0, p->width, 0, p->height);

	for (int i = s->y1 ; i < s->y2; i++)
		for (int j = 3 * s->x1; j < 3 * s->x2; j += 3) {
			if (i > 0 && i < p->height - 1 && j > 2 && j < 3 * (p->width - 1)) {
				for (int t = 0; t < 3; t++) {
					sum = 0;
					for (int k = 0; k < 9; k++) {
						int x = i + di[k];
						int y = j + t + dj[k];
						sum += (double)aux_p->ppm[x][y] * kernel[k / 3][k % 3];
					}
					double rez = (double)sum / 16;
					p->ppm[i][j + t] = (int)clamp(rez);
				}
			}
		}
}

void APPLY(image *p, image *aux_p, select *s, char word[])
{
	// verify is the image is RGB
	if (photo_type(p->type[1]) == 1) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	switch (convert_apply(word)) {
	case 1:
		EDGE(p, aux_p, s);
		printf("APPLY EDGE done\n");
		break;
	case 2:
		SHARPEN(p, aux_p, s);
		printf("APPLY SHARPEN done\n");
		break;
	case 3:
		BLUR(p, aux_p, s);
		printf("APPLY BLUR done\n");
		break;
	case 4:
		GAUSSIAN_BLUR(p, aux_p, s);
		printf("APPLY GAUSSIAN_BLUR done\n");
		break;
	default:
		printf("APPLY parameter invalid\n");
		break;
	}

	getchar();
}

void EQUALIZE(image *p, int ph_loaded)
{
	// verify if we have an image loaded
	if (ph_loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	// the loaded image must be grey-scale
	if (photo_type(p->type[1]) == 0) {
		printf("Black and white image needed\n");
		return;
	}

	// vector of frequency of each pixel
	int frequency[256] = {0};

	for (int i = 0; i < p->height; i++)
		for (int j = 0 ; j < p->width; j++)
			frequency[p->pgm[i][j]]++;

	double area = p->height * p->width;
	double sum, rez;

	// the equalization formula
	for (int i = 0; i < p->height; i++)
		for (int j = 0 ; j < p->width; j++) {
			sum = 0;
			for (int k  = 0; k <= p->pgm[i][j]; k++)
				sum += frequency[k];
			rez = (double)255 * sum;
			rez = (double)rez / area;
			p->pgm[i][j] = (int)clamp(rez);
		}

	printf("Equalize done\n");

	getchar();
}

void HISTOGRAM(image *p, int ph_loaded)
{
	char c, line[40];
	// verify if we have an image loaded
	if (ph_loaded == 0) {
		printf("No image loaded\n");
		fgets(line, sizeof(line), stdin);
		return;
	}

	c = getchar();
	if (c == '\n') {
		printf("Invalid command\n");
		return;
	}

	int x, y;
	scanf("%d", &x);

	c = getchar();
	if (c == '\n') {
		printf("Invalid command\n");
		return;
	}

	scanf("%d", &y);

	c = getchar();
	if (c != '\n') {
		fgets(line, sizeof(line - 2), stdin);
		printf("Invalid command\n");
		return;
	}

	// the loaded image must be grey-scale
	if (photo_type(p->type[1]) == 0) {
		printf("Black and white image needed\n");
		return;
	}

	int nr_pixels = 256 / y, max_ap = 0;
	int frequency[256] = {0};

	for (int i = 0; i < p->height; i++)
		for (int j = 0 ; j < p->width; j++)
			frequency[p->pgm[i][j]]++;

	for (int i = 0; i < 256 ; i += nr_pixels) {
		int sum = 0;
		for (int j = 0; j < nr_pixels; j++)
			sum += frequency[i + j];
		if (sum > max_ap)
			max_ap = sum;
	}

	// histogram
	for (int i = 0; i < 256 ; i += nr_pixels) {
		int sum = 0;
		for (int j = 0; j < nr_pixels; j++)
			sum += frequency[i + j];
		double nr_stars = (double)sum * x;
		nr_stars = (double)nr_stars / max_ap;
		int stars = (int)nr_stars;
		printf("%d\t|\t", stars);
		for (int k = 0; k < stars; k++)
			printf("*");
		printf("\n");
	}
}

int angle_values(int x)
{
	if (x == 0)
		return 1;
	if (x == 90)
		return 1;
	if (x == 180)
		return 1;
	if (x == 270)
		return 1;
	if (x == 360)
		return 1;
	return 0;
}

// 90 degree rotation for an N*N selection
void image_rotate_section(image *p, image *aux_p, select *s)
{
	int x = -1, y;
	if (photo_type(p->type[1]) == 1) {
		for (int i = s->y1 ; i < s->y2; i++) {
			x++;
			y = aux_p->height - 1;
			for (int j = s->x1; j < s->x2; j++) {
				p->pgm[i][j] = aux_p->pgm[y][x];
				y--;
			}
		}
	} else {
		x *= 3;
		for (int i = s->y1 ; i < s->y2; i++) {
			x += 3;
			y = aux_p->height - 1;
			for (int j = s->x1 * 3; j <= (s->x2 - 1) * 3; j += 3) {
				p->ppm[i][j] = aux_p->ppm[y][x];
				p->ppm[i][j + 1] = aux_p->ppm[y][x + 1];
				p->ppm[i][j + 2] = aux_p->ppm[y][x + 2];
				y--;
			}
		}
	}

	photo_copy(p, aux_p, s->x1, s->x2, s->y1, s->y2);
}

// rotating the selected area of the image
void ROTATE_SELECTED_AREA(image *p, image *aux_p, select *s, int angle)
{
	// if the angle is 360/0 degrees, nothing will change
	if (angle == 360 || angle == 0)
		return;

	int dim_height = s->y2 - s->y1;
	int dim_width = s->x2 - s->x1;

	free_photo(aux_p);
	alloc_matrix(p, aux_p, dim_height, dim_width);
	photo_copy(p, aux_p, s->x1, s->x2, s->y1, s->y2);

	switch (angle) {
	case 90:
		image_rotate_section(p, aux_p, s);
		break;

	case 180:
		// 2 * 90 degrees
		image_rotate_section(p, aux_p, s);
		image_rotate_section(p, aux_p, s);
		break;

	case 270:
		// 3 * 90 degrees
		image_rotate_section(p, aux_p, s);
		image_rotate_section(p, aux_p, s);
		image_rotate_section(p, aux_p, s);
		break;

	default:
		break;
	}
}

// rotating the entire image
void image_rotate_all(image *p, image *aux_p, int angle)
{
	int x = -1, y;
	switch (angle) {
	case 90:
		if (photo_type(p->type[1]) == 1) {
			for (int i = 0 ; i < p->width; i++) {
				x++; y = 0;
				for (int j = p->height - 1; j >= 0; j--) {
					aux_p->pgm[x][y] = p->pgm[j][i];
					y++;
				}
			}
		} else {
			for (int i = 0; i <= 3 * (p->width - 1); i += 3) {
				x++; y = 0;
				for (int j = p->height - 1; j >= 0; j--) {
					aux_p->ppm[x][y] = p->ppm[j][i];
					aux_p->ppm[x][y + 1] = p->ppm[j][i + 1];
					aux_p->ppm[x][y + 2] = p->ppm[j][i + 2];
					y += 3;
				}
			}
		}
		break;

	case 180:
		if (photo_type(p->type[1]) == 1) {
			for (int i = p->height - 1; i >= 0; i--) {
				x++; y = 0;
				for (int j = p->width - 1; j >= 0; j--) {
					aux_p->pgm[x][y] = p->pgm[i][j];
					y++;
				}
			}
		} else {
			for (int i = p->height - 1; i >= 0; i--) {
				x++; y = 0;
				for (int j = 3 * (p->width) - 1; j >= 0; j -= 3) {
					aux_p->ppm[x][y] = p->ppm[i][j - 2];
					aux_p->ppm[x][y + 1] = p->ppm[i][j - 1];
					aux_p->ppm[x][y + 2] = p->ppm[i][j];
					y += 3;
				}
			}
		}
		break;

	case 270:
		x = -1;
		if (photo_type(p->type[1]) == 1) {
			for (int i = p->width - 1 ; i >= 0; i--) {
				x++; y = 0;
				for (int j = 0; j < p->height; j++) {
					aux_p->pgm[x][y] = p->pgm[j][i];
					y++;
				}
			}
		} else {
			for (int i =  3 * p->width - 1 ; i >= 0; i -= 3) {
				x++; y = 0;
				for (int j = 0; j < p->height; j++) {
					aux_p->ppm[x][y] = p->ppm[j][i - 2];
					aux_p->ppm[x][y + 1] = p->ppm[j][i - 1];
					aux_p->ppm[x][y + 2] = p->ppm[j][i];
					y += 3;
				}
			}
		}
		break;

	default:
		break;
	}
}

// rotating the entire image
void ROTATE_ALL(image *p, image *aux_p, select *s, int angle)
{
	// if the angle is 360/0 degrees, nothing will change
	if (angle == 360 || angle == 0)
		return;

	free_photo(aux_p);

	if (angle == 90 || angle == 270) {
		aux_p->width = p->height; aux_p->height = p->width;
		s->x2 = p->height; s->y2 = p->width;
	} else {
		aux_p->width = p->width; aux_p->height = p->height;
	}

	alloc_matrix(p, aux_p, aux_p->height, aux_p->width);

	image_rotate_all(p, aux_p, angle);

	free_photo(p);
	alloc_matrix(aux_p, p, aux_p->height, aux_p->width);

	photo_copy(aux_p, p, 0, aux_p->width, 0, aux_p->height);
}

void ROTATE(image *p, image *aux_p, select *s, int ph_loaded)
{
	char line[40];
	// verify if we have an image loaded
	if (ph_loaded == 0) {
		printf("No image loaded\n");
		fgets(line, sizeof(line), stdin);
		return;
	}

	int dim_height = s->y2 - s->y1;
	int dim_width = s->x2 - s->x1;

	int angle;
	scanf("%d", &angle);

	if (dim_height != dim_width) {
		if (p->height != dim_height && p->width != dim_width) {
			printf("The selection must be square\n");
			return;
		}
	}

	int aux_angle = angle;
	if (angle == -270)
		aux_angle = 90;
	if (angle == -180 || angle == -360)
		aux_angle = abs(angle);
	if (angle == -90)
		aux_angle = 270;

	if (angle_values(aux_angle) == 0) {
		printf("Unsupported rotation angle\n");
		return;
	}

	if (dim_height == p->height && dim_width == p->width)
		ROTATE_ALL(p, aux_p, s, aux_angle);
	else
		ROTATE_SELECTED_AREA(p, aux_p, s, aux_angle);

	printf("Rotated %d\n", angle);
}
