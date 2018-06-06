#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "bmp_header.h"


typedef struct{
	unsigned char b, g, r;
} pixel;


		// Task 1
pixel** Black_white(pixel **image, pixel **black_white, int height, int width, FILE* out){

	int i, j, k;
	char a = 0;
	
	black_white = malloc( height * sizeof(pixel* ));
	for (i = 0; i < height; i++){
		black_white[i] = malloc(width * sizeof(pixel));
			
	}

	int X;
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			X = (image[i][j].b + image[i][j].g + image[i][j].r) / 3;
			black_white[i][j].b = X;
			black_white[i][j].g = X;
			black_white[i][j].r = X;
			fwrite(&black_white[i][j], sizeof(pixel), 1, out);
		}
		
		k = width % 4;
		if( k != 0)
			fwrite(&a, sizeof(char), k, out);
		
	}	
	return black_white ;
}


		// Task 2
pixel** Filtre(pixel **old_image, pixel **new_image, int **F, int height, int width, FILE* out){
	int i, j, k, l, m, n, p, q;
	int Sb, Sg, Sr;
	char a=0;
	
	new_image = malloc( height * sizeof(pixel* ));
	for (i = 0; i < height; i++){
		new_image[i] = malloc(width * sizeof(pixel));
			
	}

	if(F[0][0] != F[2][0]){
		k = F[0][0];
		F[0][0] = F[2][0];
		F[2][0] = k;
	}

	if(F[0][1] != F[2][1]){
		k = F[0][1];
		F[0][1] = F[2][1];
		F[2][1] = k;
	}

	if(F[0][2] != F[2][2]){
		k = F[0][2];
		F[0][2] = F[2][2];
		F[2][2] = k;
	}



	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			Sb = 0;
			Sg = 0;
			Sr = 0;
			if(i == 0)
				m = 1;
			else
				m = 0;

			if(j == 0)
				q = 1;
			else
				q = 0;

			if(i == height - 1)
				n = 2;
			else
				n = 3;

			if(j == width - 1)
				p = 2;
			else
				p = 3;

			for (k = m; k < n; k++)
				for (l = q; l < p; l++){
					Sr = Sr + old_image[i-1+k][j-1+l].r * F[k][l];
					Sg = Sg + old_image[i-1+k][j-1+l].g * F[k][l];
					Sb = Sb + old_image[i-1+k][j-1+l].b * F[k][l];
					}
			if(Sg > 255)
			new_image[i][j].g = 255;
			else if(Sg < 0)
			new_image[i][j].g = 0;
			else
			new_image[i][j].g = (char)Sg;

			if(Sb > 255)
			new_image[i][j].b = 255;
			else if(Sb < 0)
			new_image[i][j].b = 0;
			else
			new_image[i][j].b = (char)Sb;
		
			if(Sr > 255)
			new_image[i][j].r = 255;
			else if(Sr < 0)
			new_image[i][j].r = 0;
			else
			new_image[i][j].r = (char)Sr;

		}


	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			fwrite(&new_image[i][j], sizeof(pixel), 1, out);
		}
	k = width % 4;
		if( k != 0)
			fwrite(&a, sizeof(char), k, out);
		
	}
	
	return new_image;
}


		//Functia modul
int Abs( int a, int b){

	if(a-b > 0)
		return a-b;
	else
		return b-a;
}



			// Task 3
pixel** New_image(pixel **image, int height, int width, int threshold){

	int **verif, S, i, j, k=0, l;
	pixel aux;

	typedef struct{	
		int line, col;				
	}pozitie;

	pozitie *v;

	verif = malloc( height * sizeof(int* ));
	for (i = 0; i < height; i++)
		verif[i] = malloc(width * sizeof(int));

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			verif[i][j] = 0;

	
	
	for (i = height-1; i >=0; i--)
		for (j = 0; j < width; j++){
			k = 0;
			if(verif[i][j] == 0){
				
				aux.b = image[i][j].b;
				aux.g = image[i][j].g;
				aux.r = image[i][j].r;
				v = malloc(sizeof(pozitie));
				k++;
				v[0].line = i;
				v[0].col = j;
				verif[v[0].line][v[0].col] = 1;
						
			}
			while(k > 0){
	
				image[v[0].line][v[0].col] = aux;
				
				
					// vecinul sus
				if(v[0].line > 0){
					S = Abs(aux.r, image[v[0].line-1][v[0].col].r) + Abs(aux.g, image[v[0].line-1][v[0].col].g) + Abs(aux.b, image[v[0].line-1][v[0].col].b);
					if(verif[v[0].line-1][v[0].col] == 0 && S <= threshold){
						k++;
						v = realloc(v, k*sizeof(pozitie));
						v[k-1].line = v[0].line - 1;
						v[k-1].col = v[0].col;
						verif[v[0].line-1][v[0].col] = 1;

					}		
				}
			
					// vecinul stanga
				if(v[0].col > 0){
					S = Abs(aux.r, image[v[0].line][v[0].col-1].r) + Abs(aux.g, image[v[0].line][v[0].col-1].g) + Abs(aux.b, image[v[0].line][v[0].col-1].b);
					if(verif[v[0].line][v[0].col-1] == 0 && S <= threshold){
						k++;
						v = realloc(v, k*sizeof(pozitie));
						v[k-1].line = v[0].line;
						v[k-1].col = v[0].col - 1;
						verif[v[0].line][v[0].col-1] = 1;

					}		
				}

				
					
					// vecinul jos
				if(v[0].line < height - 1){
					S = Abs(aux.r, image[v[0].line+1][v[0].col].r) + Abs(aux.g, image[v[0].line+1][v[0].col].g) + Abs(aux.b, image[v[0].line+1][v[0].col].b);
					if(verif[v[0].line+1][v[0].col] == 0 && S <= threshold){
						k++;
						v = realloc(v, k*sizeof(pozitie));
						v[k-1].line = v[0].line + 1;
						v[k-1].col = v[0].col;
						verif[v[0].line+1][v[0].col] = 1;

					}		
				}

			
					// vecinul dreapta
				if(v[0].col < width - 1){
					S = Abs(aux.r, image[v[0].line][v[0].col+1].r) + Abs(aux.g, image[v[0].line][v[0].col+1].g) + Abs(aux.b, image[v[0].line][v[0].col+1].b);
					if(verif[v[0].line][v[0].col+1] == 0 && S <= threshold){
						k++;
						v = realloc(v, k*sizeof(pozitie));
						v[k-1].line = v[0].line;
						v[k-1].col = v[0].col + 1;
						verif[v[0].line][v[0].col+1] = 1;

					}		
				}

				if(k > 1){
					for(l = 0; l < k; l++)
						v[l] = v[l+1];
					v = realloc(v, (k-1)*sizeof(pozitie));
					}
				else
					free(v);
				k--;
				

			}

				

		}

	

	for(i = 0; i < height; i++)
		free(verif[i]);
	free(verif);

	return image;
	
}


		// Task 4
pixel** Decomprim(pixel **image_dec, int height, int width, FILE* in){

	int **verif, i, j;

	short line=0, col=0;
	pixel color;
	verif = malloc( height * sizeof(int* ));
	for (i = 0; i < height; i++)
		verif[i] = malloc(width * sizeof(int));

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			verif[i][j] = 0;

	while(line <= height && col <= width && verif[height-1][width-1] == 0){

		fread(&line, sizeof(short), 1, in);
		fread(&col, sizeof(short), 1, in);
		fread(&color, sizeof(pixel), 1, in);
		image_dec[height-line][col-1].r = color.b;
		image_dec[height-line][col-1].g = color.g;
		image_dec[height-line][col-1].b = color.r;
		verif[line-1][col-1] = 1;

	}

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++){
		
			if(verif[height-i-1][j] == 1)
				color = image_dec[i][j];
			else
				image_dec[i][j] = color;

		}


	for(i = 0; i < height; i++)
		free(verif[i]);
	free(verif);	

	return image_dec;

}


int main(void){

	char name_file[] = "input.txt";

	FILE *in_0 = fopen(name_file, "rt");
	if (in_0 == NULL) {
        	fprintf(stderr, "Can't open %s", name_file);
        	return -1;
    	}

	char input_filename[50];
	fscanf(in_0, "%s", input_filename);

	FILE *in = fopen(input_filename, "rb");
	if (in == NULL) {
        	fprintf(stderr, "Can't open %s", input_filename);
        	return -1;
    	}

	struct bmp_fileheader A;
	struct bmp_infoheader B;

	fread(&A, sizeof(struct bmp_fileheader), 1, in);
	fread(&B, sizeof(struct bmp_infoheader), 1, in);
	fseek(in, A.imageDataOffset, SEEK_SET);

	pixel **image;
	int i, j;
	image = malloc( B.height * sizeof(pixel* ));

	for (i = 0; i < B.height; i++){
		image[i] = malloc(B.width * sizeof(pixel));
			
	}
	
	int k;
	for (i = 0; i < B.height; i++){
		fread(image[i], sizeof(pixel), B.width, in);
		k = B.width % 4;
		if( k != 0)
			fseek(in, k, SEEK_CUR);
			
	}


	fclose(in);

					// Black_white
	char output_filename_1[50];
	input_filename[strlen(input_filename) - 4]  = '\0';	
	strcpy(output_filename_1, input_filename);
 	strcat(output_filename_1, "_black_white.bmp");

	FILE *out = fopen(output_filename_1, "wb");
	if (out == NULL) {
        	fprintf(stderr, "Can't open %s", output_filename_1);
        	return -1;
    	}

	fwrite(&A, sizeof(struct bmp_fileheader), 1, out);
	fwrite(&B, sizeof(struct bmp_infoheader), 1, out);
	fseek(out, A.imageDataOffset, SEEK_SET);

	pixel **black_white;
	black_white = Black_white(image, black_white, B.height, B.width, out);

	fclose(out);


					// Filtrul F1
	char output_filename_2[50];
	strcpy(output_filename_2, input_filename);
 	strcat(output_filename_2, "_f1.bmp");
	
	out = fopen(output_filename_2, "wb");
	if (out == NULL) {
        	fprintf(stderr, "Can't open %s", output_filename_2);
        	return -1;
    	}

	fwrite(&A, sizeof(struct bmp_fileheader), 1, out);
	fwrite(&B, sizeof(struct bmp_infoheader), 1, out);
	fseek(out, A.imageDataOffset, SEEK_SET);

	int** F1;
	F1 = malloc( 3 * sizeof(int* ));
	for (i = 0; i < 3; i++){
		F1[i] = malloc(3 * sizeof(int));
			
	}

	F1[0][0] = -1; F1[0][1] = -1; F1[0][2] = -1;
	F1[1][0] = -1; F1[1][1] = 8; F1[1][2] = -1;
	F1[2][0] = -1; F1[2][1] = -1; F1[2][2] = -1;
	
	pixel** image_f1;
	image_f1 = Filtre(black_white, image_f1, F1, B.height, B.width, out);

	for(i = 0; i < B.height; i++)
		free(image_f1[i]);
	free(image_f1);

	for(i = 0; i < 3; i++)
		free(F1[i]);
	free(F1);

	fclose(out);


						// Filtrul F2

	strcpy(output_filename_2, input_filename);
 	strcat(output_filename_2, "_f2.bmp");
	
	out = fopen(output_filename_2, "wb");
	if (out == NULL) {
        	fprintf(stderr, "Can't open %s", output_filename_2);
        	return -1;
    	}

	fwrite(&A, sizeof(struct bmp_fileheader), 1, out);
	fwrite(&B, sizeof(struct bmp_infoheader), 1, out);
	fseek(out, A.imageDataOffset, SEEK_SET);

	int** F2;
	F2 = malloc( 3 * sizeof(int* ));
	for (i = 0; i < 3; i++){
		F1[i] = malloc(3 * sizeof(int));
			
	}

	F2[0][0] = 0; F2[0][1] = 1; F2[0][2] = 0;
	F2[1][0] = 1; F2[1][1] = -4; F2[1][2] = 1;
	F2[2][0] = 0; F2[2][1] = 1; F2[2][2] = 0;
	
	pixel** image_f2;
	image_f2 = Filtre(black_white, image_f2, F2, B.height, B.width, out);

	for(i = 0; i < B.height; i++)
		free(image_f2[i]);
	free(image_f2);

	for(i = 0; i < 3; i++)
		free(F2[i]);
	free(F2);

	fclose(out);

	
						// Filtrul F3

	strcpy(output_filename_2, input_filename);
 	strcat(output_filename_2, "_f3.bmp");
	
	out = fopen(output_filename_2, "wb");
	if (out == NULL) {
        	fprintf(stderr, "Can't open %s", output_filename_2);
        	return -1;
    	}

	fwrite(&A, sizeof(struct bmp_fileheader), 1, out);
	fwrite(&B, sizeof(struct bmp_infoheader), 1, out);
	fseek(out, A.imageDataOffset, SEEK_SET);

	int** F3;
	F3 = malloc( 3 * sizeof(int* ));
	for (i = 0; i < 3; i++){
		F3[i] = malloc(3 * sizeof(int));
			
	}

	F3[0][0] = 1; F3[0][1] = 0; F3[0][2] = -1;
	F3[1][0] = 0; F3[1][1] = 0; F3[1][2] = 0;
	F3[2][0] = -1; F3[2][1] = 0; F3[2][2] = 1;
	

	pixel** image_f3;
	image_f3 = Filtre(black_white, image_f3, F3, B.height, B.width, out);

	for(i = 0; i < B.height; i++)
		free(image_f3[i]);
	free(image_f3);

	for(i = 0; i < 3; i++)
		free(F3[i]);
	free(F3);

	fclose(out);

		
		// Task 3
	int threshold;
	fscanf(in_0, "%d", &threshold);
	
	image = New_image(image, B.height, B.width, threshold);


	out = fopen("compressed.bin", "wb");
	if (out == NULL) {
        	fprintf(stderr, "Can't open %s", output_filename_2);
        	return -1;
    	}

	fwrite(&A, sizeof(struct bmp_fileheader), 1, out);
	fwrite(&B, sizeof(struct bmp_infoheader), 1, out);
	fseek(out, A.imageDataOffset, SEEK_SET);

	for (i = 0; i < B.height; i++)
		for (j = 0; j < B.width; j++){
			
			//k = B.height-i;
			if(i == 0 || j == 0 || i == B.height-1 || j == B.width-1){
			fwrite(&i+1, sizeof(short), 1, out);
			fwrite(&j+1, sizeof(short), 1, out);
			fwrite(&image[i][j].r, sizeof(char), 1, out);
			fwrite(&image[i][j].g, sizeof(char), 1, out);
			fwrite(&image[i][j].b, sizeof(char), 1, out);
			
			}
				
			else 
			if(image[i][j].r != image[i+1][j].r || image[i][j].g != image[i+1][j].g || image[i][j].b != image[i+1][j].b || image[i][j].r != image[i-1][j].r || image[i][j].g != image[i-1][j].g || image[i][j].b != image[i-1][j].b || image[i][j].r != image[i][j+1].r || image[i][j].g != image[i][j+1].g || image[i][j].b != image[i][j+1].b || image[i][j].r != image[i][j-1].r || image[i][j].g != image[i][j-1].g || image[i][j].b != image[i][j-1].b){

			//k = B.height-i;
			fwrite(&i+1, sizeof(short), 1, out);
			fwrite(&j+1, sizeof(short), 1, out);
			fwrite(&image[i][j].r, sizeof(char), 1, out);
			fwrite(&image[i][j].g, sizeof(char), 1, out);
			fwrite(&image[i][j].b, sizeof(char), 1, out);
			}
		}

	fclose(out);	

	out = fopen("compressed.bmp", "wb");
	if (out == NULL) {
        	fprintf(stderr, "Can't open %s", output_filename_2);
        	return -1;
    	}

	fwrite(&A, sizeof(struct bmp_fileheader), 1, out);
	fwrite(&B, sizeof(struct bmp_infoheader), 1, out);
	fseek(out, A.imageDataOffset, SEEK_SET);

	char a=0;
	for (i = 0; i < B.height; i++){
			
		for (j = 0; j < B.width; j++){
			fwrite(&image[i][j].b, sizeof(char), 1, out);
			fwrite(&image[i][j].g, sizeof(char), 1, out);
			fwrite(&image[i][j].r, sizeof(char), 1, out);
		}
		k = B.width % 4;
		if( k != 0)
			fwrite(&a, sizeof(char), k, out);
		
	}

	fclose(out);


		// Task 4
	char input_filename_2[50];
	fscanf(in_0, "%s", input_filename_2);

	in = fopen(input_filename_2, "rb");
	if (in == NULL) {
        	fprintf(stderr, "Can't open %s", input_filename);
        	return -1;
    	}

	fread(&A, sizeof(struct bmp_fileheader), 1, in);
	fread(&B, sizeof(struct bmp_infoheader), 1, in);
	fseek(in, A.imageDataOffset, SEEK_SET);

	pixel **image_dec;
	image_dec = malloc( B.height * sizeof(pixel* ));
	for (i = 0; i < B.height; i++){
		image_dec[i] = malloc(B.width * sizeof(pixel));
			
	}

	image_dec = Decomprim(image_dec, B.height, B.width, in);
	fclose(in);
	
	out = fopen("decompressed.bmp", "wb");
	if (out == NULL) {
        	fprintf(stderr, "Can't open %s", output_filename_2);
        	return -1;
    	}

	fwrite(&A, sizeof(struct bmp_fileheader), 1, out);
	fwrite(&B, sizeof(struct bmp_infoheader), 1, out);
	fseek(out, A.imageDataOffset, SEEK_SET);
	
	
	for (i = 0; i < B.height; i++){
		for (j = 0; j < B.width; j++){
			fwrite(&image_dec[i][j], sizeof(pixel), 1, out);
		}
		k = B.width % 4;
		if( k != 0)
			fwrite(&a, sizeof(char), k, out);
		
	}

	fclose(out);

	for(i = 0; i < B.height; i++)
		free(image_dec[i]);
	free(image_dec);

	return 0;
}
