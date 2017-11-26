#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

//печать матрицы
void printmatr(double** a, int n){
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			cout << a[i][j] << "  ";
		}cout << endl;
	}
}

//проверка на диагональное преобладание
int check_diag(double** a, int n){
	int k = 0;
	for (int i = 0; i < n; i++){
		double max = abs(a[i][i]);
		double sum = 0;
		for (int j = 0; j < n; j++){
			if (i != j){
				sum += abs(a[i][j]);
			}
		}
		if (max <= sum) k++;
	}
	return k;
}


//возврат нормы матрицы
float norm(double** a, int n){

	double temp = 0;
	double norm_m = 0;
	for (int i = 0; i < n; i++)
	{
		temp = 0;
		for (int j = 0; j < n; j++){
			temp += a[i][j] * a[i][j];//евклидова норма
		}
	}
	return sqrt(temp);
}

//создание матрицы альфа и b
void change_matr(double **a, double*b, double** alf, int n){

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++){
			if (i != j){
				alf[i][j] = -(a[i][j] / a[i][i]);
			}
			else{
				alf[i][j] = 0;
			}
		}
		b[i] = b[i] / a[i][i];
	}
}

//преобразование матрицы к диагональной
void make_diag(double **a, double*b, int n){

	double d;

	for (int k = 0; k < n; k++) {
		for (int j = k + 1; j < n; j++) {
			d = a[j][k] / a[k][k];
			for (int i = k; i < n; i++) {
				a[j][i] = a[j][i] - d * a[k][i];
			}
			b[j] = b[j] - d*b[k];
		}
	}

	for (int k = n - 1; k > 0; k--) {
		for (int j = k - 1; j >= 0; j--) {
			d = a[j][k] / a[k][k];
			for (int i = k; i >= 0; i--) {
				a[j][i] = a[j][i] - d*a[k][i];
			}
			b[j] = b[j] - d*b[k];
		}
	}
}

//генерирование матрицы с диаг преобладанием
void generate_diag(double**a, double*b, int n)
{
	//рандомная матрица b
	for (int i = 0; i < n; i++){
		b[i] = rand() % 1000;
		int c = rand() % 100;
		if (c>50) b[i] * (-1);
	}

	for (int i = 0; i < n; i++){
		double sum = 0;
		for (int j = 0; j < n; j++){
			a[i][j] = rand() % 500;
			int c = rand() % 100;
			if (c>50) a[i][j] * (-1);
			if (i != j)
				sum += abs(a[i][j]);
		}
		if (abs(a[i][i]) < sum)
		{
			a[i][i] = sum;
			int c = rand() % 100;
			a[i][i] += rand() % 100;
			if (c > 50) a[i][i] * (-1);
		}

	}
}

//проверка диаг коэффицентов
int check_diag_koef(double**a, int n){
	int k = 0;
	for (int i = 0; i < n; i++){
		if (a[i][i] == 0)k++;
	}
	return k;
}
//сравнение векторов k и k+1 итерации. если они укладываются в епсилон - возвращает 1
bool comparison(double* x, double* x1, int n, float eps)
{
	int m = false;
	double temp = 0;
	double norm1=0;
	for (int i = 0; i < n; i++){
		temp = abs(x1[i] - x[i]);
		if (norm1 < temp){
			norm1 = temp;
		}
	}
	if ( norm1 <= eps) m = true;
	return m;
}


int main(){

	int n = 3;

	double **a = new double *[n];

	for (int i = 0; i < n; i++)
		a[i] = new double[n];

	double* b = new double[n];
	float no = 0;

	a[0][0] = 100;
	a[0][1] = 6;
	a[0][2] = -2;

	a[1][0] = 6;
	a[1][1] = 200;
	a[1][2] = -10;

	a[2][0] = 1;
	a[2][1] = -2;
	a[2][2] = -100;

	b[0] = 200;
	b[1] = 600;
	b[2] = 500;

	//for (int i = 0; i < n; i++){
	//	for (int j = 0; j < n; j++)
	//		a[i][j] = rand() % 30;
	//	b[i] = rand() % 30;
	//}

	double* x1 = new double[n];
	for (int i = 0; i < n; i++){
		x1[i] = 0;
	}
	//ЗАДАЮ МАТРИЦУ 

	//генерирую матрицу с диаг преобл-ем
	//generate_diag(a, b, n);

	int check1 = check_diag_koef(a, n);

	if (check1 > 0){ cout << "Generate matrix again, please" << endl; }
	else
	{
		int k = check_diag(a, n);
		if (k > 0){
			//привожу к диагональному виду и считаю просто
			make_diag(a, b, n);
			printmatr(a, n);

			for (int i = 0; i < n; i++)
				x1[i] = b[i] / a[i][i];

			for (int i = 0; i < n; i++){
				printf("%f", b[i]); cout << endl;
			}

			cout << endl;
			cout << " The matrix wasn`t diagonal predominant " << endl;
			//печатаю значения
			for (int i = 0; i < n; i++){
				printf("%f", x1[i]); cout << endl;
			}
		}
		else
		{
			//начальное приближение ==b 
			double* x = new double[n];
			for (int i = 0; i < n; i++){
				x[i] = b[i];
			}

			double **alf = new double *[n];
			for (int i = 0; i < n; i++)
				alf[i] = new double[n];
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++){
					alf[i][j] = 0;
				}
			}
			cout << endl;

			no = norm(alf, n);
			double eps = 0.00001;
			int iter = 0;
			bool m = 0;

			change_matr(a, b, alf, n);
			//начальное приближение - вектор b
			for (int i = 0; i < n; i++){
				x[i] = b[i];
			}

			if (norm(alf, n) < 1){ //проверка на сходимость решения
				while (m != 1)
				{
				//		cout << "Iteration  : " << iter + 1 << endl;			
					for (int i = 0; i < n; i++){
						double sum = 0;
						for (int j = 0; j < n; j++){
							if (i != j){
								sum += alf[i][j] * x[i];
							}
						}
						x1[i] = b[i] + sum;

					}

				/*	cout << "Vector X 1" << endl;
					for (int i = 0; i < n; i++){ printf("%f", x1[i]); cout << endl; }
					cout << endl;
					cout << endl;*/
					m = comparison(x, x1, n, eps);
					for (int i = 0; i < n; i++){
						x[i] = x1[i];
					}
					
					iter++;
				}
			}

			cout << "Amount of Iteration: " << iter << endl;
			//вектор - ответ
			for (int i = 0; i < n; i++)
			{
				printf("%f", x1[i]);
				cout << endl;
			}
		}
	}
	return 0;
}