// rwdocument.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<windows.h>
#include <locale.h>
#include<Objbase.h>

int main()
{
	std::locale::global(std::locale(""));
	const wchar_t *FILENAME = L"test.doc";
	//IStream *pStream = nullptr;
	//IStorage *pStgRoot = nullptr;
	//DWORD dwMode = STGM_WRITE | STGM_SHARE_EXCLUSIVE;
	//const WCHAR *str = L"测试字符";
	//WCHAR buffer[256];
	//ULONG retVal = 0;
	//HRESULT hr = ::StgCreateDocfile(FILENAME, dwMode, 0, &pStgRoot);
	//if (SUCCEEDED(hr))
	//	pStgRoot->CreateStream(L"firstStream", dwMode, 0, 0 ,&pStream);
	//pStream->Write(str, lstrlen(str), &retVal);
	//pStream->Read(buffer, lstrlen(str), &retVal);
	//pStgRoot->Release();
	//pStream->Release();
	
	DWORD dwMode = STGM_READ | STGM_SHARE_EXCLUSIVE;	// 只读|独占
	IStorage *pStgRoot  = nullptr;
	IStream *pStream = nullptr;

	IEnumSTATSTG *pEnum = NULL; // 枚举器

	HRESULT hr = ::StgOpenStorage(FILENAME,NULL,dwMode,NULL,0,&pStgRoot);
	if (SUCCEEDED(hr))
		HRESULT hr1 = pStgRoot->EnumElements(0, NULL, 0, &pEnum);

	const ULONG need = 10;
	ULONG fetch = 10;
	STATSTG stastg[need] = {};		//返回root节点下面所有的节点类型
	pEnum->Next(need, stastg, &fetch);
	pEnum->Release();

	for (int i = 0; i < fetch;++i)
	{
		ULONG realSize = 0;
		WCHAR buffer[1024] ;
		switch (stastg[i].type)
		{
			case STGTY_STREAM:
			{
				pStgRoot->OpenStream(stastg[i].pwcsName, NULL, dwMode, 0, &pStream);
				//pStream->Read(buffer,lstrlen(buffer), &realSize);
				pStream->Read(buffer, 1024, &realSize);
				std::wcout << buffer << std::endl;
				continue;
			}
			case STGTY_STORAGE:
			{
				STATSTG subStatStg[10];
				IStorage *subStg = nullptr;
				IEnumSTATSTG *pSubEnum = NULL;
				pStgRoot->OpenStorage(stastg[i].pwcsName, NULL, dwMode, 0, 0, &subStg);
				HRESULT hr1 = subStg->EnumElements(0, NULL, 0, &pSubEnum);
				ULONG ActualNum = 0;
				pSubEnum->Next(10, subStatStg, &ActualNum);
				IStream *pSubStream = nullptr;
				subStg->OpenStream(subStatStg[0].pwcsName, NULL, dwMode, 0, &pSubStream);

				WCHAR buff[1024];
				pSubStream->Read(buff, 1024, &ActualNum);

			}
		}
	}

	pStgRoot->Release();

	//IStorage *pstgRoot = nullptr;
	//IStorage *psubStg = nullptr;
	//IStorage *pNewStgRoot = nullptr;
	//StgCreateDocfile(L"newfile.doc", STGM_WRITE | STGM_SHARE_EXCLUSIVE, 0, &pNewStgRoot);
	//StgOpenStorage(FILENAME, NULL, STGM_READ | STGM_SHARE_EXCLUSIVE, NULL, 0, &pstgRoot);
	//pstgRoot->CopyTo(0, NULL, NULL,pNewStgRoot);

	//ILockBytes *pLockByte = nullptr;
	//IStorage *pStgRoot = nullptr;
	//StgOpenStorageOnILockBytes(pLockByte, NULL, STGM_READ | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE, NULL, 0, &pStgRoot);

	//WCHAR buffer[256] = { 0 };
	//ULONG retRead = 0;
	//ULARGE_INTEGER offset = { 0 };
	//pLockByte->ReadAt(offset, buffer, 256, &retRead);

}
