#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <cstdio>
#include <windows.h>

enum STATUS {
	SUCCESS = 0,	//成功
	NO_USER = 1,	//无用户
	PWD_ERROR = 2,	//密码错
	BAD_REQ = 3,	//请求有误
	TIME_OUT = 4	//连接超时
};

class Connect 
{
	std::string connect_path;
public:
	Connect()
	{
		connect_path = "lib/connect/client.py";
		return;
	}
	int login(std::string username, std::string pwd, int& level)
	{
		std::string cmd = "python " + connect_path + " " + username + " " + pwd;
		remove("login_flag");
		system(cmd.c_str());
		FILE* f = fopen("login_flag", "r");
		int waiting = 10;	// 2000ms
		while (f == NULL && waiting)
		{
			f = fopen("login_flag", "r");
			Sleep(200);
			waiting--;
		}
		if (waiting == 0)
		{
			return TIME_OUT;
		}
		FILE* rf = fopen("login_rsp", "r");
		if (rf == NULL)
		{
			return BAD_REQ;
		}
		char status[100];
		fscanf(rf, "%s", status);
		if(strcmp(status, "succeed") == 0)
		{
			fscanf(rf, "%d", &level);
			return SUCCESS;
		}
		if (strcmp(status, "bad_request_format") == 0)
		{
			return BAD_REQ;
		}
		if (strcmp(status, "no_such_user") == 0)
		{
			return NO_USER;
		}
		if (strcmp(status, "pwd_error") == 0)
		{
			return PWD_ERROR;
		}
		return BAD_REQ;
	}
};