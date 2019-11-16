
#pragma once
#ifndef  _KSTRING_STRING_H__
#define _KSTRING_STRING_H__

#include<iostream>
#include<string>

template<typename charT>
unsigned int StrLen(const charT* str)
{

	if (str == nullptr)
		return 0;
	unsigned int count = 0;
	while(*str != '\0')
	{
		count++;
		str++;
	}
	return count;
}

template<typename charT>
class String{
public:
	typedef charT			 *iterator;
	typedef const charT *const_iterator;
	typedef unsigned int sizeType;
	String();
	String(const charT *str);
	String(std::string &str);
	String(const String &rhs);
	//String(String &&rhs);
	String(sizeType num, const charT singleChar);
	String(const charT *str, sizeType pos, sizeType count);
	String(const charT *str, sizeType pos);
	
	bool		   empty()		const;
	sizeType size()		const;
	sizeType length()		const;
	sizeType capacity() const;

	void clear();
	void pop_back();
	void resize(sizeType newSize);
	void push_back(const charT &singleChar);
	void append(const char *str);
	void append(sizeType count, const charT singleChar);
	charT *insert(sizeType index, const charT *str);
	String<charT> substr(sizeType pos, sizeType count) const;

	String &operator=(const String &rhs);
	String &operator=(const charT *str);
	String &operator=(const std::string &str);
	String &operator+(const charT *str);
	bool   operator!=(const String &rhs);
	charT &operator[](sizeType pos);

	iterator begin();
	const_iterator cbegin() const;
	iterator end();
	const_iterator cend() const;
	~String();

private:
	charT *m_data;		
	sizeType m_currenSize;
	sizeType m_size;

	void InitWithNull();
};

template<typename charT>
String<charT>::String()
{
	InitWithNull();
}

template<typename charT>
String<charT>::String(const charT* str)
{
	if (!str)
	{
		InitWithNull();
		return;
	}
	m_currenSize = StrLen(str);
	m_size = 2 * m_currenSize;
	m_data = new charT[m_size + 1];			//提供两倍冗余内存空间
	for (int i = 0; i < m_currenSize; i++)
	{
		m_data[i] = str[i];
	}
	m_data[m_currenSize] = '\0';
}

template<typename charT>
String<charT>::String(std::string &str)
{
	if (str.empty())
	{
		InitWithNull();
		return;
	}
	m_currenSize = str.size();
	m_size = 2 * m_currenSize;
	m_data = new charT[m_size + 1];
	for(int i=0; i < m_currenSize; ++i)
	{
		m_data[i] = str[i];
	}
	m_data[m_currenSize] = '\0';
}

template<typename charT>
String<charT>::String(sizeType num, const charT singleChar)
{
	m_currenSize = num;
	m_size = 2 * m_currenSize;
	m_data = new charT[m_size + 1];
	for(int i=0; i < m_currenSize; ++i)
	{
		m_data[i] = singleChar;
	}
	m_data[m_currenSize] = '\0';
}

template<typename charT>
String<charT>::String(const charT *str, sizeType pos)
{
	sizeType len = StrLen(str);
	if ((pos > len) || str == nullptr)
	{
		std::cout << "param is null or pos out of range " << std::endl;
		InitWithNull();
		return;
	}
	
	m_currenSize = len - pos;
	m_size  = 2 * m_currenSize;
	m_data = new charT[m_size + 1];
	for(sizeType i = 0; i < m_currenSize; ++i)
	{
		m_data[i] = str[pos+i];
	}
	m_data[m_currenSize] = '\0';
}

template<typename charT>
String<charT>::String(const charT *str, sizeType pos, sizeType count)
{
	if((pos + count) > StrLen(str))
		String(str,pos);
	else
	{
		m_currenSize = count;
		m_size = 2 * m_currenSize;
		m_data = new charT[m_size + 1];
		for(sizeType i = 0; i < count; ++i)
		{
			m_data[i] = str[pos + i];
		}
		m_data[m_currenSize] = '\0';
	}

}

template<typename charT>
String<charT>::String(const String<charT> &rhs)			//whats diff between String<charT>::String(const String<charT> &rhs)
{
	if (rhs.empty())
	{
		InitWithNull();
		return;
	}
	m_currenSize = rhs.m_currenSize;
	m_size = rhs.m_size;

	m_data = new charT[m_size + 1];
	for(sizeType i=0; i < m_currenSize; ++i)
	{
		m_data[i] = rhs.m_data[i];
	}
	m_data[m_currenSize] = '\0';
}

template<typename charT>
String<charT>::~String()
{
	delete [] m_data;
	m_data = nullptr;
}

template<typename charT>
String<charT>& String<charT>::operator=(const String<charT>& rhs)
{
	if (*this != rhs)		//避免自赋值
	{
		m_currenSize = rhs.m_currenSize;
		m_size = rhs.m_size;
		if (m_data)
			delete[] m_data;
		charT *tmp = nullptr;
		if (!rhs.empty())
		{
			tmp = new charT[m_size + 1];
			for (sizeType i = 0; i < m_currenSize; ++i)
			{
				tmp[i] = rhs.m_data[i];
			}
			tmp[m_currenSize] = '\0';
		}
		m_data = tmp;
	}
	return *this;
}

template<typename charT>
String<charT>& String<charT>::operator=(const charT *str)
{
	if (m_data)
	{
		delete[] m_data;
		m_data = nullptr;
	}
		
	if (!str)
	{
		InitWithNull();
		return *this;
	}
	String tmp(str);
	*this = tmp;
	return *this;
}

template<typename charT>
String<charT> &String<charT>::operator=(const std::string &str)
{
	if (str.empty())
	{
		String tmp;
		*this = tmp;
	}
	else
		*this = str.c_str();
	return *this;
}

template<typename charT>
charT& String<charT>::operator[](sizeType pos)
{
	if(pos >=0 && pos <= m_currenSize)
		return m_data[pos];
	else
		std::exception("outof range");
}

template<typename charT>
bool String<charT>::operator!=(const String<charT> &rhs)
{
	return (m_data != rhs.m_data);
}

template<typename charT>
String<charT>& String<charT>::operator+(const charT* str)
{
	this->append(str);
	return *this;
}

template<typename charT>
bool String<charT>::empty() const
{
	return (m_currenSize == 0);
}

template<typename charT>
typename String<charT>::sizeType String<charT>::size() const
{
	return m_currenSize;
}

template<typename charT>
typename String<charT>::sizeType String<charT>::length() const
{
	return m_currenSize;
}

template<typename charT>
typename String<charT>::sizeType String<charT>::capacity() const
{
	return (m_size - m_currenSize);
}

template<typename charT>
void String<charT>::clear()
{
	for(sizeType i=0; i <= m_currenSize; ++i)
	{
		m_data[i] = ' ';
	}
	m_currenSize = 0;
}

template<typename charT>
typename String<charT>::iterator String<charT>::begin()
{
	return &m_data[0];
}

template<typename charT>
typename String<charT>::iterator String<charT>::end()
{
	return &m_data[m_currenSize];
}

template<typename charT>
typename String<charT>::const_iterator String<charT>::cbegin() const
{
	return &m_data[0];
}

template<typename charT>
typename String<charT>::const_iterator String<charT>::cend() const
{
	return &m_data[m_currenSize];
}

template<typename charT>
void String<charT>::push_back(const charT &singleChar)
{
	if(m_currenSize == m_size)
		resize(2*m_currenSize);
	m_data[m_currenSize] = singleChar;
	m_data[++m_currenSize] = '\0';
	
}

template<typename charT>
void String<charT>::append(const char *str)
{
	if (!str)
		return;
	sizeType len = StrLen(str);
	if((m_currenSize + len) >= m_size)
		resize(2*(m_currenSize + len));
	for(sizeType i=0; i < len; ++i)
	{
		m_data[m_currenSize + i] = str[i];
	}
	m_currenSize += len;
	m_data[m_currenSize] = '\0';
}

template<typename charT>
void String<charT>::append(sizeType count, const charT singleChar)
{
	if((m_currenSize + count) >= m_size)
		resize(2*(m_currenSize + count));

	for(sizeType i=0; i < count; ++i)
	{
		m_data[m_currenSize + i] = singleChar;
	}
	m_currenSize += count;
	m_data[m_currenSize] = '\0';
}

template<typename charT>
void String<charT>::pop_back()
{
	if(empty())
		std::exception("empty");
	m_data[m_currenSize] = ' ';
	m_data[--m_currenSize] = '\0';
}

template<typename charT>
charT* String<charT>::insert(sizeType index, const charT* str)
{
	if (!str)
		return nullptr;
	sizeType len = StrLen(str);
	if (index <0 || index > m_currenSize)
	{
		std::cout << "out of range" << std::endl;
		return nullptr;
	}
	if((m_size - m_currenSize) < len)
		resize(2*(m_size-m_currenSize + len));
	for(sizeType i=m_currenSize+len; i >= index; --i)
		m_data[i] = m_data[i-len];
	
	for (sizeType i = 0; i < len; ++i)
		m_data[index + i] = str[i];
	m_currenSize += len;
	return &m_data[index];
}


template<typename charT>
String<charT> String<charT>::substr(sizeType pos, sizeType count) const
{
	String<charT> tmp(m_data,pos,count);
	return tmp;
}

template<typename charT>
void String<charT>::resize(sizeType newSize)
{
	charT *newData = new charT[newSize + 1];
	for(sizeType i=0; i < m_currenSize; ++i)
	{
		newData[i] = m_data[i];
	}
	newData[m_currenSize] = '\0';
	delete [] m_data;		//回收旧内存
	m_data = newData;
	m_size = newSize;
}

template<typename charT>
void String<charT>::InitWithNull()
{
	m_data = nullptr;
	m_currenSize = m_size = 0;
}

#endif
