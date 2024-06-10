#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


void SearchBaz(float** mtrx, int rows, int columns, int** now_baz) {
	int i, j;
	for (i = 0; i < rows-1; i++) {
		for (j = 0; j < columns; j++) {
			if (mtrx[i][j] == 1) {
				int k; int exit = 0;
				for (k = 0; k < rows-1; k++) {
					if (k != i) {
						if (mtrx[k][j] != 0) {
							exit = 1;
						}
					}
					if (exit == 1) 
						break;
				}
				if (exit == 0) {
					(*now_baz)[i] = j;
				}
			}
		}
	}
}


int mincolumn(float** mtrx, int rows, int columns) {
	int min = 999;
	int minc = columns;
	for (int i = 0; i < columns; ++i) {
		if (mtrx[rows-1][i] < min) {
			min = mtrx[rows-1][i];
			minc = i;
		}
	}
	return minc;
}

int minrows(float** mtrx, float* pr, int rows, int columns, int  minc) {
	float min = 999;
	int minr = rows;
	for (int j = 0; j < rows-1; ++j) { 
		if (((float)(pr[j] / mtrx[j][minc]) < min) && (mtrx[j][minc] >= 0) && (mtrx[j][minc] > 0)) {
			min = (float)(pr[j] / mtrx[j][minc]);
			minr = j;
		}
	}
	return minr;
}

void basic_changing(float*** mtrx, int leadingRow, int leadingColumn, int rows, int columns, float** pr, int** now_baz)
{
	int i, j;
	float element = (*mtrx)[leadingRow][leadingColumn];
	(*pr)[leadingRow] = (*pr)[leadingRow] / element;
	for (i = 0; i < columns; i++) {
		(*mtrx)[leadingRow][i] = (*mtrx)[leadingRow][i] / element;
	}
	for (i = 0; i < rows; i++) {
		if (i != leadingRow) {
			element = (*mtrx)[i][leadingColumn];
			(*pr)[i] = (*pr)[i] - element * (*pr)[leadingRow];
			for (j = 0; j < columns; j++) {
				(*mtrx)[i][j] = (*mtrx)[i][j] - element * (*mtrx)[leadingRow][j];
			}
		}
	}
	(*now_baz)[leadingRow] = leadingColumn;
}

void printmtrx(float** mtrx, float* pr, int rows, int columns) {
	printf("Матрица: \n", columns);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			printf("%10.3f|", mtrx[i][j]);
		}
		printf("|%10.3f \n", pr[i]);
		printf("------------------------------------------------------------\n");
	}
}

int ost(float** mtrx, int rows, int columns) {
	int exit = 0;
	for (int i = 0; i < columns; ++i) {
		if (mtrx[rows - 1][i] < 0)
			exit = 1;
		if (exit == 1)
			break;
	}
	if (exit == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

void print_vector(float** mtrx, float* pr, int* now_baz, int rows, int columns) {
	float* vector = (float*)malloc(columns * sizeof(float));
	for (int i = 0; i < columns; i++) {
		vector[i] = 0;
	}
	for (int i = 0; i < rows-1; ++i) {
		vector[now_baz[i]] = pr[i];
	}
	printf("Искомый вектор: \n");
	printf("(");
	for (int i = 0; i < columns; i++) {
		printf("%10.3f ", vector[i]);
	}
	printf(")");
}

void main() {
	setlocale(LC_ALL, "Rus");
	int n, m;
	FILE* fp;
	fp = fopen("Text.txt", "r");
	fscanf(fp, "%d", &n);
	fscanf(fp, "%d", &m);
	/*printf("Введите кол-во переменных: \n");
	scanf("%d", &n);
	printf("Введите кол-во ограничений: \n");
	scanf("%d", &m); */
	m = m++;
	float** mtrx = (float**)malloc(m * sizeof(float*));
	for (int i = 0; i < m; i++) {
		mtrx[i] = (float*)malloc(n * sizeof(float));
	}
	printf("Введите %d*%d матрицу коэффициентов: \n", m - 1, n);
	for (int i = 0; i < m; i++) {
		if (i == m - 1) {
			printf("Введите %d коэффициентов критерия: \n", n);
		}
		for (int j = 0; j < n; j++) {
			if (i < m - 1) {
				fscanf(fp, "%f", &mtrx[i][j]);
				//scanf("%f", &mtrx[i][j]);
			}
			if (i == m - 1) {
				fscanf(fp, "%f", &mtrx[i][j]);
				//scanf("%f", &mtrx[i][j]);
				mtrx[i][j] = 0 - mtrx[i][j];
			}
		}
	}
	printf("Введите %d правых частей: \n", m - 1);
	float* pr = (float*)malloc(m * sizeof(float));
	for (int i = 0; i < m; i++) {
		if (i < m - 1) {
			fscanf(fp, "%f", &pr[i]);
			//scanf("%f", &pr[i]);
		}
		else {
			pr[i] = 0;
		}

	}
	fclose(fp);
	system("cls");
	int* now_baz = (int*)malloc((m-1) * sizeof(int));
	printmtrx(mtrx, pr, m, n);
	SearchBaz(mtrx, m, n, &now_baz);
	for (int i = 0; i < m-1; ++i) {
		printf("x%d - базисная переменная для %d строки\n", now_baz[i]+1, i + 1);
	}
	printf("\n\n\n");
	int flag = ost(mtrx, m, n);
	while (flag == 0) {
		int minc = mincolumn(mtrx, m, n);
		int minr = minrows(mtrx, pr, m, n, minc);
		basic_changing(&mtrx, minr, minc, m, n, &pr,&now_baz);
		printmtrx(mtrx, pr, m, n);
		for (int i = 0; i < m-1; ++i) {
			printf("x%d - базисная переменная для %d строки\n", now_baz[i]+1, i + 1);
		}
		printf("\n\n\n");
		flag = ost(mtrx, m, n);
	}
	print_vector(mtrx, pr, now_baz, m, n);
}