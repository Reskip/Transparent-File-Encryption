#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib> 
#include <map>
#include <iostream>
#include <windows.h>

void getNewCtrl(int &fileLevel, int &readTimes)	//实现这个
{
	fileLevel = 0;
	readTimes = 5;
	return;
}

enum ENC_STATUS {
	ENC_SUCCESS = 0,		//成功
	LOW_LEVEL = 1,		//用户级别过低
	NO_OPEN_TIMES = 2	//么得打开次数了
};

class Ctrol {
public:
	int fileLevel;
	int readTimes;
	Ctrol() {};
	Ctrol(std::stringstream &init) {
		init >> fileLevel >> readTimes;
		return;
	}
};

class File {
public:
	std::string filename;
	std::string realpath;
	std::string filetype;
	std::string fullpath;
	std::string encpath;

	std::string cmd;
	Ctrol *ctrl;

	File(std::string path)
	{
		int split = 0;
		char drive[5];
		char dir[100];
		char fname[100];
		char fileext[10];

		_splitpath(path.c_str(), drive, dir, fname, fileext);
		filename = fname;
		filename += fileext;
		realpath = drive;
		realpath += dir;
		filetype = fileext;

		fullpath = realpath + filename;
		encpath = "enc_" + filename;
		return;
	}
	void print()
	{
		std::cout << "name: " << filename << "\n";
		std::cout << "dir: " << realpath << "\n";
		std::cout << "ext: " << filetype << "\n";
		return;
	}
};

std::string encrypt(std::string file)
{
	std::string ret = "";
	for (char ch : file)
	{
		ret += "/";
		ret += std::to_string(int(ch + 37));
	}
	return ret;
}

std::string decrypt(std::string file)
{
	std::string ret = "";
	std::stringstream ss(file);
	char ch;
	int tmp;
	while (ss >> ch)
	{
		if (ch == '/')
		{
			ss >> tmp;
			tmp -= 37;
			ret += char(tmp);
		}
	}
	return ret;
}

void openAndEncrypt(File *fp) {
	std::cout << "Decrypt_file_open" << "\n";
	system(fp->cmd.c_str());
	std::cout << "Decrypt_file_close" << "\n";

	std::string writted = "ENCRYPT ";
	writted += std::to_string(fp->ctrl->fileLevel);
	writted += " ";
	writted += std::to_string(fp->ctrl->readTimes);
	writted += " ";
	std::ifstream f(fp->encpath.c_str(), std::ios::binary);
	char ch;
	while (f.read(&ch, sizeof(ch)))
	{
		writted += ch;
	}
	writted = encrypt(writted);

	std::ofstream output(fp->fullpath.c_str(), std::ios::binary);
	for (char ch : writted)
	{
		output.write(&ch, sizeof(ch));
	}
	output.close();
	f.close();
	
	remove(fp->encpath.c_str());
	delete(fp->ctrl);
	delete(fp);
	return;
}

class Encrypt {
private:
	std::map<std::string, std::string>openMethod;
public:
	int fileLevel;
	int readTimes;

	Encrypt() {
		openMethod.insert(std::pair<std::string, std::string>(".txt", "notepad"));
		openMethod.insert(std::pair<std::string, std::string>(".py", "notepad"));
		openMethod.insert(std::pair<std::string, std::string>(".xlsx", ""));
		openMethod.insert(std::pair<std::string, std::string>(".docx", ""));
	}
	int open(std::string path, int userLevel)
	{
		File* fp = new File(path);
		File &fileInfo = *fp;

		std::ifstream f(path, std::ios::binary);
		
		std::string file = "";
		std::string tmp;
		char ch;
		while (f.read(&ch, sizeof(ch)))
		{
			file += ch;
		}
		file = decrypt(file);

		std::stringstream ss(file);
		ss >> tmp;
		if (tmp == "ENCRYPT")
		{
			Ctrol *ctr = new Ctrol(ss);
			if (ctr->readTimes == 0)
			{
				return NO_OPEN_TIMES;
			}
			if (ctr->fileLevel > userLevel)
			{
				return LOW_LEVEL;
			}
			std::ofstream output(fileInfo.encpath, std::ios::binary);
			std::string place_holder = "ENCRYPT " + std::to_string(ctr->fileLevel) + " " + std::to_string(ctr->readTimes) + " ";
			for (int i = place_holder.size(); i < file.size(); i++)
			{
				output.write(&file[i], sizeof(file[i]));
			}
			output.close();
			fp->ctrl = ctr;
		}
		else
		{
			Ctrol *ctr = new Ctrol();
			CopyFile(path.c_str(), fileInfo.encpath.c_str(), true);
			getNewCtrl(ctr->fileLevel, ctr->readTimes);
			fp->ctrl = ctr;
		}
		
		fp->ctrl->readTimes--;
		fileLevel = fp->ctrl->fileLevel;
		readTimes = fp->ctrl->readTimes;

		fileInfo.cmd = (openMethod[fileInfo.filetype] + " " + fileInfo.encpath);
		HANDLE h; //线程句柄
		h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)openAndEncrypt, fp, 1, 0); //创建子线程
		ResumeThread(h);  //启动子线程
		
		return ENC_SUCCESS;
	}
};
