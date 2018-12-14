// kstring.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<Windows.h>
#include"kstring.h"

String<char>& Upper(String<char>& str)
{
	char diff = 'A' - 'a';
	for (auto it = str.begin(); it != str.end(); it++)
	{
		*it = *it + diff;
	}
	return str;
}

String<char> newString(String<char> &str)
{
	String<char> tmp = str;
	return tmp;
}


int main()
{
	/*
	typedef char charT;
	const charT *TestStr = "123456789";

	//String();
	String<charT> cstr1;

	//String(const charT *str);
	charT *emptyStr = nullptr;
	String<charT> cstr2Null =emptyStr;
	String<charT> cstr2 = TestStr;

	//String(std::string &str);
	std::string StdString;
	String<charT> cstr3Null = StdString;
	StdString = "hello";
	String<charT> cstr3 = StdString;

	//String(const String &rhs);
	String<charT> cstr4Null = cstr1;		//空初始化
	String<charT> cstr4 = cstr3;

	//String(sizeType num, const charT singleChar);
	String<charT> cstr5(10, '6');

	//String(const charT *str, sizeType pos, sizeType count);
	String<charT> cstr6Null(nullptr, 2);
	String<charT> cstr6(TestStr, 2);
	String<charT> cstrOutrange(TestStr, 100);

	//String(const charT *str, sizeType pos);
	String<charT> cstr7Null(nullptr, 2,4);
	String<charT> cstr7(TestStr, 2, 4);

	//String &operator=(const String &rhs);
	String<charT> nullStr;
	cstr2 = nullStr;
	cstr2 = cstr3;

	//String &operator=(const charT *str);
	cstr2 = nullptr;			//其他数据成员未清零
	cstr1 = "new test";

	//String &operator=(const std::string &str);
	std::string empty;
	cstr1 = empty;
	cstr1 = StdString;

	//迭代器
	for (auto it = cstr1.cbegin(); it != cstr1.cend(); it++)
	{
		std::cout << *it;
	}

	auto isempty = cstr1.empty();
	auto Size1 = cstr1.size();
	auto Size2 = cstr1.length();
	auto capacity = cstr1.capacity();
	cstr1[1] = 'Z';

	cstr1.push_back('B');
	cstr1.append("CDFGHIJKLMN");
	cstr1.append(2, 'E');
	cstr1 = cstr1 + "abc";		//测试自赋值
	cstr1.pop_back();
	cstr1.clear();

	cstr1 = "1234";
	cstr1.insert(1, "ab");
	cstr1.insert(1, "abcdefghijkll");	//扩容
	*/

	String<char> str = "hello";
	str = Upper(str);
	String<char> sub0 = newString(str);

	String<char> sub = str.substr(1,3);
	for (auto it = sub.cbegin(); it != sub.cend(); it++)
	{
		std::cout << *it;
	}
}

