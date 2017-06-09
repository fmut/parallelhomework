/**
 Parallel Homework
 Copyright (c) 2017 Furkan Mut,Servet Şatıroğlu,Buse Genç,Toprak Eğin,Bilal Davutoğlu
 **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ASatir 1000
#define AKolon 1000
#define BSatir 1000
#define BKolon 1000
#define PTHREADS 4

/* Matrix veri tipini tanımlama */
typedef struct Matrix {
	int r, c;
	int *contents;
	int **rows;
}Matrix;
// Süreyi hesaplamak için
clock_t begin,end;
double time_spent;


int rows_per_proc;
Matrix A, B, C;
pthread_t tid[PTHREADS];

/* Fonksiyon tanımlamaları */
Matrix createMatrix(int rows, int cols);
void *MatrixMult(void* tid);
void printMatrices(Matrix A, Matrix B, Matrix C);

void main() {
	int rw, cl, i, err;
  begin = clock();
	/* 3 matris yaratıyoruz ve initialize ediyoruz.Birinci ve ikinci matris çarpılıyor ve üçüncü matris sonuçları tutuyor. */
	A = createMatrix(ASatir, AKolon);
	B = createMatrix(BSatir, BKolon);
	C = createMatrix(ASatir, BKolon);

	/* A ve B matrislerini 0 ile 100 arasında rastgele intlerle doldurun*/
	srand(time(NULL));

	for (rw = 0; rw < ASatir; rw++){
		for (cl = 0; cl < AKolon; cl++){
			A.rows[rw][cl] = 1;
		}
	}

	for (rw = 0; rw < BSatir; rw++){
		for (cl = 0; cl < BKolon; cl++){
			B.rows[rw][cl] = 1;
		}
	}

	/* C'nin satırları her Thread tarafından hesaplanacak.*/
	rows_per_proc = ASatir / PTHREADS;

	/* Initialize etme ve pthreads'i başlatıyoruz */
	for (i = 0; i < PTHREADS; i++){
		err = pthread_create(&tid[i], NULL, MatrixMult, (void *)i);
		if (err == 0) printf("Thread %d started successfully!\n", i);
		else printf("Thread %d did not start successfully.\n", i);
	}

	for (i = 0; i < PTHREADS; i++){
		err = pthread_join(tid[i], NULL);
		if (err == 0) printf("Thread %d joined successfully!\n", i);
		else printf("Thread %d did not join successfully.\n", i);
	}

	printMatrices(A, B, C);
	printf("Press Enter to Exit: ");
	//scanf("Exit");
	getchar();
}

Matrix createMatrix(int rows, int cols){
	int i;
	Matrix mtx;
	mtx.r = rows;
	mtx.c = cols;

	/*mtx için bellek tahsisi */
	mtx.contents = (int *)malloc(sizeof(int)* rows * cols);
	mtx.rows = (int **)malloc(sizeof(int *)* rows);

	/* Matrisin her satırının başlangıç ​​bellek adresini bul */
	for (i = 0; i < rows; i++){
		mtx.rows[i] = mtx.contents + i*cols;
	}

	return mtx;
}

void *MatrixMult(void* tid){
	int i, j, k, total;
	int id = (int)tid;
	int row_start = id*rows_per_proc;

	if (id < PTHREADS - 1){
		for (i = row_start; i < row_start + rows_per_proc; i++){
			for (j = 0; j < B.c; j++){
				total = 0;
				for (k = 0; k < A.c; k++){
					total += A.rows[i][k] * B.rows[k][j];
				}
				C.rows[i][j] = total;
			}
		}
	}
	else{ //Son Thread kalan tüm satırları alır
		for (i = row_start; i < row_start + (A.r - ((PTHREADS - 1)*rows_per_proc)); i++){
			for (j = 0; j < B.c; j++){
				total = 0;
				for (k = 0; k < A.c; k++){
					total += A.rows[i][k] * B.rows[k][j];
				}
				C.rows[i][j] = total;
			}
		}
	}

	return 0;
}

void printMatrices(Matrix A, Matrix B, Matrix C){
	int i, j;
  FILE *f;
     f = fopen("programcikti.txt", "a");
	// Dosyaya Matrix A'yı yazdırma
	fprintf(f,"Matrix A:\n");
	for (i = 0; i < A.r; i++){
		for (j = 0; j < A.c; j++){
			fprintf(f,"%6d ", A.rows[i][j]);
		}
		fprintf(f,"\n\n");
	}

	// Dosyaya Matrix B'yi yazdırma
	fprintf(f,"Matrix B:\n");
	for (i = 0; i < B.r; i++){
		for (j = 0; j < B.c; j++){
			fprintf(f,"%6d ", B.rows[i][j]);
		}
		fprintf(f,"\n\n");
	}

	//  Dosyaya Matrix C 'yi(sonuç matrisi) yazdırma
	fprintf(f,"Matrix C (A * B):\n");
	for (i = 0; i < C.r; i++){
		for (j = 0; j < C.c; j++){
			fprintf(f,"%6d ", C.rows[i][j]);
		}
		fprintf(f,"\n\n");
	}
  fclose(f);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("%lf",time_spent);
}
