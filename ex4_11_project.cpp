#include<math.h>
#include<iostream>
#include<random>
#include"ex4_11_project.h"
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

double* x_generator(double v, double p, double x[], int size) {

	random_device rand_device;
	mt19937 mt_Rng(rand_device());
	uniform_real_distribution<double>  uniform_real_dist(0, 1);
	/*
		X ����
	*/
	for (int i = 0; i < size; i++) {
		if (uniform_real_dist(mt_Rng) < p) x[i] = v;
		else x[i] = -v;
	}

	return x;
}

//�ش� �Լ��� marsaglia polar method �� �̿��Ͽ� ���� ������ ������ ������ �����մϴ�.
double* n_generator(double n[], int size) {

	random_device rand_device;
	mt19937 mt_Rng(rand_device());
	uniform_real_distribution<double>  uniform_real_dist(0, 1);


	double u, v, s;

	/*
		���� ������ ������ ���� ����
	*/
	for (int i = 0; i < size; i++) {
		do {
			u = uniform_real_dist(mt_Rng) * 2.0 - 1.0;
			v = uniform_real_dist(mt_Rng) * 2.0 - 1.0;
			s = u * u + v * v;
		} while (s >= 1.0 || s == 0.0);

		n[i] = u * sqrt(-2.0 * log(s) / s);
	}

	return n;
}


// �ش� �Լ��� a�������� b ������ ������ �����ϴ� �Լ��Դϴ�.
// �ռ� simpson ������ �̿��Ͽ� �ٻ簪�� ���մϴ�. 
double SimpsonIntegration(double a, double b, double h, double v, bool zero_to_one_error) {
	double sum = 0;

	for (double i = a; i < b - FLT_EPSILON; i = i + (2 * h)) {
		sum = sum + Given_function(i, v, zero_to_one_error)
			+ (4 * Given_function(i + h, v, zero_to_one_error))
			+ Given_function(i + (2 * h), v, zero_to_one_error);
	}

	return (h / 3) * sum;
}



//8������ �־��� �� �� ���б�ȣ �ȿ� ����ִ� ���� �״�� �ڵ��� �Լ��Դϴ�.
double Given_function(double y, double v, bool zero_to_one_error) {
	if (zero_to_one_error == true) { return exp(-pow(y + v, 2) / 2) / sqrt(2 * PI); }
	else { return exp(-pow(y - v, 2) / 2) / sqrt(2 * PI); }
}



void Ex_Error_Of_Given_Receiver(double v, double p) {

	cout << "\n------------------------------------------start calculating----------------------------------------" << endl;
	cout << "Given condition v: " << v << " / p: " << p << endl;

	// X, N, Y�� F_Y(y)���� ������ csv������ �ʱ� ����--------------------------------------------------
	string str1, str2, FileName;
	str1 = "v=" + to_string(v);
	str2 = "p=" + to_string(p);
	FileName = str1 + ", " + str2 + ".csv";
	//y�� ����, y�� pdf �׷����� �׸������� csv���� �̸��� "v="v��, "p="p��.csv

	ofstream File;
	File.open(FileName);
	//������ ����(�����ϸ� ���� ����� �������� ������ ������ ���)

	File << "X" << "," << "N" << "," << "Y" << "," 
		<< "f_Y(y)(-v)" << "," << "f_Y(y)(+v)" << "," << "f_Y(y)(��)" << "," << endl;
	// csv���Ͽ��� X | N | Y | f_Y(y)(-v) | f_Y(y)(+v) | f_Y(y)(��) �̷������� ����ǵ���
	// ���������Ʈ���� �� ���� ������ �׸���� ����� ������


	// 2�� ����-----------------------------------------------------------------------------------------
	// [0,1] ���̿��� uniformly distributed �� rng�� �̿��Ͽ� X�� 10,000�� ����
	// 10000���� �Ǵ� X���� cmdâ�� ����ϱ� �δ㽺����Ƿ�
	// Ex_Error_Of_Given_Receiver�Լ� �Ʒ��ʿ���
	// CSV���Ϸ� ���� �������� ����ϰ�, ������ �׷����� ��Ÿ������

	double* x = new double[10000];
	x_generator(v, p, x, 10000);

	//0.5�� -0.5�� ���� �˻�
	int num_of_v = 0;
	int num_of_nv = 0;
	for (int i = 0; i < 10000; i++) {
		if (abs(x[i] - v) < DBL_EPSILON) ++num_of_v;
		else ++num_of_nv;
	}

	// �˻������
	cout << "\n-----------------------------------------------2�����---------------------------------------------------\n" << endl;
	cout << "������ X �߿��� " << -v << "�� ���� : " << num_of_nv << endl;
	cout << "������ X �߿��� " << v << "�� ���� : " << num_of_v << endl;
	cout << "1��~������ ��� X�� ��°��� �����׷����� ������ CSV���� ����" << endl;
	cout << "�׷����� CSV������ X���� ������ ������ ��õ��Ʈ���� �л������� �׸��� �ȴ�" << endl;


	//3�� ����---------------------------------------------------------------------------------------
	//[0,1] ���̿��� uniformly distributed �� rng�� �̿��Ͽ� N�� 10,000�� ����
	// 10000���� �Ǵ� X���� cmdâ�� ����ϱ� �δ㽺����Ƿ�
	// Ex_Error_Of_Given_Receiver�Լ� �Ʒ��ʿ���
	// CSV���Ϸ� ���� �������� ����ϰ�, ������ �׷����� ��Ÿ������
	double* n = new double[10000];
	n_generator(n, 10000);


	//����� ǥ���Լ��� ����������� �˻�
	int a = 0;
	int b = 0;
	int c = 0;
	for (int i = 0; i < 10000; i++) {
		if (n[i] <= 1 && n[i] >= -1) ++a;
		if (n[i] <= 2 && n[i] >= -2) ++b;
		if (n[i] <= 3 && n[i] >= -3) ++c;
	}

	// �˻������
	cout << "\n------------------------------------------------3�����-------------------------------------------------------\n" << endl;
	cout << "num of N inside -1 and 1: " << a << endl;
	cout << "num of N inside -2 and 2: " << b << endl;
	cout << "num of N inside -3 and 3: " << c << endl;
	cout << "1��~������ ��� N�� ��°��� �����׷����� ������ CSV���� ����" << endl;
	cout << "�׷����� CSV������ N���� ������ ������ ��õ��Ʈ���� ������׷����� �׸��� �ȴ�" << endl;

	//4��, 6��, 7�� ����------------------------------------------------------------------------------------
	//Y=X+N ��� �� receiver �� �Ǵ� ��� 
	double* y = new double[10000];

	int receiver_0 = 0; // receiver�� 0�̶�� �Ǵ�
	int receiver_1 = 0; // receiver�� 1�̶�� �Ǵ�

	int receiver_error_0 = 0; // sender�� 1�� ���´µ�, receiver�� 0�̶�� �Ǵ�
	int receiver_error_1 = 0; // sender�� 0�� ���´µ�, receiver�� 1�̶�� �Ǵ�

	for (int i = 0; i < 10000; i++) {
		y[i] = x[i] + n[i]; // 4�� 
			// 10000���� �Ǵ� Y���� cmdâ�� ����ϱ� �δ㽺����Ƿ�
			// Ex_Error_Of_Given_Receiver�Լ� �Ʒ��ʿ���
			// CSV���Ϸ� ���� �������� ����ϰ�, ������ �׷����� ��Ÿ������
		if (y[i] < 0) {
			++receiver_0; // 6��
			if (abs(x[i] - v) < DBL_EPSILON) ++receiver_error_0; // 7��
		}
		else {
			++receiver_1; // 6��
			if (abs(x[i] + v) < DBL_EPSILON) ++receiver_error_1; // 7��
		}
	}
	// 4�� ������
	cout << "\n--------------------------------------------4�����----------------------------------------------------\n" << endl;
	cout << "1��~������ ��� Y�� ��°��� �����׷����� ������ CSV���� ����" << endl;
	cout << "�׷����� CSV������ Y���� ������ ������ ��õ��Ʈ���� ������׷����� �׸��� �ȴ�" << endl;

	// 5�� ��� ���
	cout << "\n--------------------------------------------5�����---------------------------------------------------\n" << endl;
	cout << "Y��pdf�� ������ CSV���� ����" << endl;
	cout << "�׷����� CSV������ Y��, f_Y(y)(�Ǽ�)��, f_Y(y)(����)��,f_Y(y)(��)���� ��� ������" << endl;
	cout <<	"������ ��õ��Ʈ���� �л����� ù��°�׸����� �׸��� �ȴ�" << endl;

	// 6�� ������
	cout << "\n--------------------------------------------6�����--------------------------------------------------\n" << endl;
	cout << "receiver_0(receiver�� 0�̶�� �Ǵ��� Ƚ��) : " << receiver_0 << endl;
	cout << "receiver_1(receiver�� 1�̶�� �Ǵ��� Ƚ��) : " << receiver_1 << endl;

	// 7�� ������
	cout << "\n--------------------------------------------7�����--------------------------------------------------\n" << endl;
	cout << "receiver_error_1(sender�� 0�� ���´µ� receiver�� 1�̶� �߸� �Ǵ��� Ƚ��) : " << receiver_error_1 << endl;
	cout << "receiver_error_0(sender�� 1�� ���´µ� receiver�� 0�̶� �߸� �Ǵ��� Ƚ��) : " << receiver_error_0 << endl;

	// CSV�� X, N, Y, Y�� pdf ���� �����ϱ�------------------------------------------------------------------------------

	double* fy_nv = new double[10000]; //-v�� ������� �ϴ� f_Y(y)
	double* fy_v = new double[10000]; // +v�� ������� �ϴ� f_Y(y)
	double* fy_sum = new double[10000]; // ���� ��ģ ���� f_Y(y)

	for (int i = 0; i < 10000; i++) {
		fy_nv[i] = (1 - p) * exp(-pow(y[i] + v, 2) / 2) / sqrt(2 * PI);
		fy_v[i] = (p) * exp(-pow(y[i] - v, 2) / 2) / sqrt(2 * PI);
		fy_sum[i] = fy_nv[i] + fy_v[i];
	}
	// 2��, 3��, 4��, 5���� ������ CSV ���Ϸ� ������
	for (int i = 0; i < 10000; i++){
		File << x[i] << "," << n[i] << "," << y[i] << "," << fy_nv[i]
			<< "," << fy_v[i] << "," << fy_sum[i] << endl;
			
		//�������� �� �ึ�� X�� | N�� | Y�� | f_Y(y)(-v) | f_Y(y)(+v)| f_Y(y)(��) �̷������� ���еǾ� ����
		//����� csv������ ���� �л��� �׷����� ����� 5�� �Ϸ�
	}

	File.close();

	

	//8�� ����------------------------------------------------------------------------------------------------
	cout << "\n---------------------------------------------8�����------------------------------------------------\n" << endl;
	cout << "sender�� 0�� ������ �� receiver�� 1�̶� �Ǵ��� Ȯ��(receiver_error_1�� Ȯ��) : " << 
		(1 - p) * SimpsonIntegration(0, 10000, 0.05, v, true) << endl;
	cout << "sender�� 1�� ������ �� receiver�� 0�̶� �Ǵ��� Ȯ��(receiver_error_0�� Ȯ��) : " << 
		(p)*SimpsonIntegration(-10000, 0, 0.05, v, false) << endl;


	cout << "\n-------------------------------------------end calculating-------------------------------------------\n" << endl;
}