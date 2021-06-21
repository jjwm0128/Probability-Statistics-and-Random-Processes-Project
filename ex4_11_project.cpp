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
		X 생성
	*/
	for (int i = 0; i < size; i++) {
		if (uniform_real_dist(mt_Rng) < p) x[i] = v;
		else x[i] = -v;
	}

	return x;
}

//해당 함수는 marsaglia polar method 를 이용하여 정규 분포를 따르는 난수를 생성합니다.
double* n_generator(double n[], int size) {

	random_device rand_device;
	mt19937 mt_Rng(rand_device());
	uniform_real_distribution<double>  uniform_real_dist(0, 1);


	double u, v, s;

	/*
		정규 분포를 따르는 난수 생성
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


// 해당 함수는 a에서부터 b 까지의 영역을 적분하는 함수입니다.
// 합성 simpson 공식을 이용하여 근사값을 구합니다. 
double SimpsonIntegration(double a, double b, double h, double v, bool zero_to_one_error) {
	double sum = 0;

	for (double i = a; i < b - FLT_EPSILON; i = i + (2 * h)) {
		sum = sum + Given_function(i, v, zero_to_one_error)
			+ (4 * Given_function(i + h, v, zero_to_one_error))
			+ Given_function(i + (2 * h), v, zero_to_one_error);
	}

	return (h / 3) * sum;
}



//8번에서 주어진 식 중 적분기호 안에 들어있는 식을 그대로 코딩한 함수입니다.
double Given_function(double y, double v, bool zero_to_one_error) {
	if (zero_to_one_error == true) { return exp(-pow(y + v, 2) / 2) / sqrt(2 * PI); }
	else { return exp(-pow(y - v, 2) / 2) / sqrt(2 * PI); }
}



void Ex_Error_Of_Given_Receiver(double v, double p) {

	cout << "\n------------------------------------------start calculating----------------------------------------" << endl;
	cout << "Given condition v: " << v << " / p: " << p << endl;

	// X, N, Y와 F_Y(y)값을 저장할 csv파일의 초기 설정--------------------------------------------------
	string str1, str2, FileName;
	str1 = "v=" + to_string(v);
	str2 = "p=" + to_string(p);
	FileName = str1 + ", " + str2 + ".csv";
	//y값 저장, y의 pdf 그래프를 그리기위한 csv파일 이름은 "v="v값, "p="p값.csv

	ofstream File;
	File.open(FileName);
	//파일을 열고(존재하면 새로 덮어쓰고 존재하지 않으면 생성후 기록)

	File << "X" << "," << "N" << "," << "Y" << "," 
		<< "f_Y(y)(-v)" << "," << "f_Y(y)(+v)" << "," << "f_Y(y)(합)" << "," << endl;
	// csv파일에서 X | N | Y | f_Y(y)(-v) | f_Y(y)(+v) | f_Y(y)(합) 이런식으로 저장되도록
	// 스프레드시트에서 각 열에 저장할 항목들의 헤더를 정의함


	// 2번 수행-----------------------------------------------------------------------------------------
	// [0,1] 사이에서 uniformly distributed 된 rng를 이용하여 X를 10,000개 생성
	// 10000개나 되는 X값을 cmd창에 출력하기 부담스러우므로
	// Ex_Error_Of_Given_Receiver함수 아래쪽에서
	// CSV파일로 값을 내보내어 출력하고, 분포를 그래프로 나타내었음

	double* x = new double[10000];
	x_generator(v, p, x, 10000);

	//0.5와 -0.5의 개수 검사
	int num_of_v = 0;
	int num_of_nv = 0;
	for (int i = 0; i < 10000; i++) {
		if (abs(x[i] - v) < DBL_EPSILON) ++num_of_v;
		else ++num_of_nv;
	}

	// 검사결과출력
	cout << "\n-----------------------------------------------2번결과---------------------------------------------------\n" << endl;
	cout << "생성된 X 중에서 " << -v << "의 갯수 : " << num_of_nv << endl;
	cout << "생성된 X 중에서 " << v << "의 갯수 : " << num_of_v << endl;
	cout << "1번~만번의 모든 X의 출력값과 분포그래프는 생성된 CSV파일 참고" << endl;
	cout << "그래프는 CSV파일의 X열을 선택후 엑셀의 추천차트에서 분산형으로 그리면 된다" << endl;


	//3번 수행---------------------------------------------------------------------------------------
	//[0,1] 사이에서 uniformly distributed 된 rng를 이용하여 N을 10,000개 생성
	// 10000개나 되는 X값을 cmd창에 출력하기 부담스러우므로
	// Ex_Error_Of_Given_Receiver함수 아래쪽에서
	// CSV파일로 값을 내보내어 출력하고, 분포를 그래프로 나타내었음
	double* n = new double[10000];
	n_generator(n, 10000);


	//제대로 표준함수가 만들어졌는지 검사
	int a = 0;
	int b = 0;
	int c = 0;
	for (int i = 0; i < 10000; i++) {
		if (n[i] <= 1 && n[i] >= -1) ++a;
		if (n[i] <= 2 && n[i] >= -2) ++b;
		if (n[i] <= 3 && n[i] >= -3) ++c;
	}

	// 검사결과출력
	cout << "\n------------------------------------------------3번결과-------------------------------------------------------\n" << endl;
	cout << "num of N inside -1 and 1: " << a << endl;
	cout << "num of N inside -2 and 2: " << b << endl;
	cout << "num of N inside -3 and 3: " << c << endl;
	cout << "1번~만번의 모든 N의 출력값과 분포그래프는 생성된 CSV파일 참고" << endl;
	cout << "그래프는 CSV파일의 N열을 선택후 엑셀의 추천차트에서 히스토그램으로 그리면 된다" << endl;

	//4번, 6번, 7번 수행------------------------------------------------------------------------------------
	//Y=X+N 계산 및 receiver 의 판단 계산 
	double* y = new double[10000];

	int receiver_0 = 0; // receiver가 0이라고 판단
	int receiver_1 = 0; // receiver가 1이라고 판단

	int receiver_error_0 = 0; // sender는 1을 보냈는데, receiver가 0이라고 판단
	int receiver_error_1 = 0; // sender는 0을 보냈는데, receiver가 1이라고 판단

	for (int i = 0; i < 10000; i++) {
		y[i] = x[i] + n[i]; // 4번 
			// 10000개나 되는 Y값을 cmd창에 출력하기 부담스러우므로
			// Ex_Error_Of_Given_Receiver함수 아래쪽에서
			// CSV파일로 값을 내보내어 출력하고, 분포를 그래프로 나타내었음
		if (y[i] < 0) {
			++receiver_0; // 6번
			if (abs(x[i] - v) < DBL_EPSILON) ++receiver_error_0; // 7번
		}
		else {
			++receiver_1; // 6번
			if (abs(x[i] + v) < DBL_EPSILON) ++receiver_error_1; // 7번
		}
	}
	// 4번 결과출력
	cout << "\n--------------------------------------------4번결과----------------------------------------------------\n" << endl;
	cout << "1번~만번의 모든 Y의 출력값과 분포그래프는 생성된 CSV파일 참고" << endl;
	cout << "그래프는 CSV파일의 Y열을 선택후 엑셀의 추천차트에서 히스토그램으로 그리면 된다" << endl;

	// 5번 결과 출력
	cout << "\n--------------------------------------------5번결과---------------------------------------------------\n" << endl;
	cout << "Y의pdf는 생성된 CSV파일 참고" << endl;
	cout << "그래프는 CSV파일의 Y열, f_Y(y)(실선)열, f_Y(y)(점선)열,f_Y(y)(합)열을 모두 선택후" << endl;
	cout <<	"엑셀의 추천차트에서 분산형의 첫번째항목으로 그리면 된다" << endl;

	// 6번 결과출력
	cout << "\n--------------------------------------------6번결과--------------------------------------------------\n" << endl;
	cout << "receiver_0(receiver가 0이라고 판단한 횟수) : " << receiver_0 << endl;
	cout << "receiver_1(receiver가 1이라고 판단한 횟수) : " << receiver_1 << endl;

	// 7번 결과출력
	cout << "\n--------------------------------------------7번결과--------------------------------------------------\n" << endl;
	cout << "receiver_error_1(sender가 0을 보냈는데 receiver가 1이라 잘못 판단한 횟수) : " << receiver_error_1 << endl;
	cout << "receiver_error_0(sender가 1을 보냈는데 receiver가 0이라 잘못 판단한 횟수) : " << receiver_error_0 << endl;

	// CSV에 X, N, Y, Y의 pdf 값들 저장하기------------------------------------------------------------------------------

	double* fy_nv = new double[10000]; //-v를 평균으로 하는 f_Y(y)
	double* fy_v = new double[10000]; // +v를 평균으로 하는 f_Y(y)
	double* fy_sum = new double[10000]; // 둘을 합친 최종 f_Y(y)

	for (int i = 0; i < 10000; i++) {
		fy_nv[i] = (1 - p) * exp(-pow(y[i] + v, 2) / 2) / sqrt(2 * PI);
		fy_v[i] = (p) * exp(-pow(y[i] - v, 2) / 2) / sqrt(2 * PI);
		fy_sum[i] = fy_nv[i] + fy_v[i];
	}
	// 2번, 3번, 4번, 5번의 값들을 CSV 파일로 내보냄
	for (int i = 0; i < 10000; i++){
		File << x[i] << "," << n[i] << "," << y[i] << "," << fy_nv[i]
			<< "," << fy_v[i] << "," << fy_sum[i] << endl;
			
		//엑셀에서 각 행마다 X값 | N값 | Y값 | f_Y(y)(-v) | f_Y(y)(+v)| f_Y(y)(합) 이런식으로 구분되어 저장
		//저장된 csv파일을 열고 분산형 그래프를 만들면 5번 완료
	}

	File.close();

	

	//8번 수행------------------------------------------------------------------------------------------------
	cout << "\n---------------------------------------------8번결과------------------------------------------------\n" << endl;
	cout << "sender가 0을 보냈을 때 receiver가 1이라 판단할 확률(receiver_error_1의 확률) : " << 
		(1 - p) * SimpsonIntegration(0, 10000, 0.05, v, true) << endl;
	cout << "sender가 1을 보냈을 때 receiver가 0이라 판단할 확률(receiver_error_0의 확률) : " << 
		(p)*SimpsonIntegration(-10000, 0, 0.05, v, false) << endl;


	cout << "\n-------------------------------------------end calculating-------------------------------------------\n" << endl;
}