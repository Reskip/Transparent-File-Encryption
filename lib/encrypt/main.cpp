#include "connect.h"
#include "encrypt.h"

#include <cstdio>
#include <string>
#include <iostream>


using namespace std;
int main()
{
	Encrypt enc;
	enc.open("C:\\Users\\Reskip\\Desktop\\123.xlsx", 5);	//�ļ�·�����û�level
	cout << enc.fileLevel << "\n";	//�ϴδ򿪵��ļ� ���ļ��ȼ�
	cout << enc.readTimes << "\n";	//�ϴδ��ļ���ʣ��ʹ�ô���		���Ϊ������ʾ�������޴δ�
	while (1);
	return 0;
}