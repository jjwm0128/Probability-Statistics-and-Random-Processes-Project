#include<math.h>
#include<iostream>
#include<random>
#include"ex4_11_project.h"

using namespace std;

int main() {
	// void Ex_Error_Of_Given_Receiver(double v, double p) ���̹Ƿ�
	// v, p ������ ����

	Ex_Error_Of_Given_Receiver(0.5, 0.5); // v=0.5, p=0.5
	Ex_Error_Of_Given_Receiver(0.5, 0.8); // v=0.5, p=0.8
	Ex_Error_Of_Given_Receiver(1, 0.5); // v=1, p=0.5

	return 0;
}