// smartpointer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include"KSharePtr.h"
#include"KComPtr.h"
#include"KStream.h"
#include <iostream>

int main()
{
	KSharedPtr<int> smart1 = new int(256);
	KSharedPtr<int> smart2 = smart1;

	KSharedPtr<int> smartArray1(new int[10], 1);
	KSharedPtr<int> smartArray2 = smartArray1;

	smart1 = smartArray1;
}

/*
int main()
{
	const WCHAR *str1 = L"宽体字符测试";
	const char		*str2 = "ascii character";
	ULONG retVal = 9;

	KStream<WCHAR> *ksw = new KStream<WCHAR>;
	ksw->Write(str1, lstrlen(str1), &retVal);

	KStream<char> *ksc = new KStream<char>;
	ksc->Write(str2, strlen(str2), &retVal);

	//isc->AddRef();
	//给KComPtr类模板传入的模板类型只能是COM接口，不能是对象
	//默认构造函数
	KComPtr<IStream> empty;

	//测试构造函数:KComPtr(E *p)
	KComPtr<IStream> pcomptr1 = ksw;		//ksw:ref=2

	//测试构造函数:KComPtr(IUnknown* pUnk)
	IUnknown *iunk = ksc;
	iunk->AddRef();
	KComPtr<IStream> pcomptr3 = iunk;	//ksc:ref = 3

	//测试拷贝构造函数:KComPtr(const KComPtr<E>& rhs)
	KComPtr<IStream> pcomptr2 = pcomptr1;		//ksc: ref = 3

	//测试构造函数:template <class Type>  KComPtr(const KComPtr<Type>& p) //TODO

	//测试E* KComPtr<E>::operator=(E* p)
	KComPtr<IStream> pcom = ksc;		//ksc: ref = 4
	pcom = ksw;				//ksc: ref = 3,  ksw:ref=4

	//E* KComPtr<E>::operator=(const KComPtr<E>& rhs)
	pcomptr1 = pcom;

	//template <class Type> E* operator=(const KComPtr<Type>& p);  //TODO
	
}	//ref  = 0;


*/