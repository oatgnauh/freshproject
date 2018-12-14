// stream.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include"KStream.hpp"
#include"KReadArchive.h"
#include"KWriteArchive.h"
#include <iostream>

int main()
{
	//KStream<char> *ks =new KStream<char>("1234567890");
	//KStream<char> *empty = new KStream<char>;
	//char buffer[1024];
	//ULONG ActualLenght = 0;
	//ks->Read(buffer, 20, &ActualLenght);
	//ks->Write("this is a test message.", 100, &ActualLenght);

	//ULARGE_INTEGER CopyNumber, ReadNumber, WriteNumber;
	//CopyNumber.QuadPart = 1024;
	//ks->CopyTo(empty, CopyNumber, &ReadNumber, &WriteNumber);
	//ks->Release();
	//empty->Release();
	//const char *filename = "Text.txt";
	const WCHAR *FILENAME = L"Text.txt";
	std::wifstream ReadStream(FILENAME);
	ReadStream.seekg(0,std::ios::end);
	int len = ReadStream.tellg();
	ReadStream.seekg(0,std::ios::beg);
	WCHAR *buffer0 = new WCHAR[2*len];
	ReadStream.read(buffer0, 256);
	std::wcout << buffer0 << std::endl;
	
	


	KReadArchive *kr = new KReadArchive(FILENAME);
	wchar_t buffer[256] = { 0 };
	ULONG ActualRead = 0;
	kr->Read(buffer, 100, &ActualRead);
	std::wcout << buffer<< std::endl;
	kr->Release();

	//	KWriteArchive *kw = new KWriteArchive(filename);
	//	ULONG ActualWrite;
	//	kw->Write("hello", 10, &ActualWrite);
	//
	//	kw->Release();
	//}
	ReadStream.close();
	delete[] buffer0;
}
