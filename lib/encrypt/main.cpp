#include "connect.h"
#include "encrypt.h"

#include <cstdio>
#include <string>
#include <iostream>


using namespace std;
int main()
{
	Encrypt enc;
	enc.open("C:\\Users\\Reskip\\Desktop\\123.xlsx", 5);	//文件路径、用户level
	cout << enc.fileLevel << "\n";	//上次打开的文件 的文件等级
	cout << enc.readTimes << "\n";	//上次打开文件的剩余使用次数		如果为负数表示可以无限次打开
	while (1);
	return 0;
}