// -------------------------------------------------------
// KStream.h
// 创建者： 黄涛
// 创建时间： 2018/11/22
// 功能描述： 基于标准C或Windows API实现一个KStream类，实现IStream接口(IStream != istream)
// Copyright 2013 Kingsoft
// --------------------------------------------------------
#pragma once
#ifndef  _FRESHPROJECT_KSTREAM_H__
#define _FRESHPROJECT_KSTREAM_H__
#include<stdio.h>
#include<string.h>
#include<wchar.h>
#include <windows.h>

	// {6168D37B-B2B5-487F-895D-87767BD00D23}
static const IID IID_KSTREAM =
{ 0x6168d37b, 0xb2b5, 0x487f, { 0x89, 0x5d, 0x87, 0x76, 0x7b, 0xd0, 0xd, 0x23 } };

template<typename charT>
ULONG StrLen(const charT* str)
{
	if (str == nullptr)
		return 0;
	ULONG count = 0;
	while (*str != '\0')
	{
		++count;
		++str;
	}
	return count;
}


template<typename charT>
class KStream: public IStream
{
public:
	KStream();
	KStream(const charT* str);
	virtual HRESULT __stdcall  QueryInterface(const IID& iid,  void **ppv) override;
	virtual ULONG   __stdcall   AddRef() override;
	virtual ULONG   __stdcall   Release() override;

	virtual STDMETHODIMP Read(void *pv, ULONG cb, ULONG *pcbRead) override;			
	virtual STDMETHODIMP Write(const void *pv, ULONG cb, ULONG *pcbWritten) override;		
	virtual STDMETHODIMP Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition) override;
	virtual STDMETHODIMP SetSize(ULARGE_INTEGER libNewSize) override;
	virtual STDMETHODIMP CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten) override;
	virtual STDMETHODIMP Commit(DWORD grfCommitFlags) override;

	virtual STDMETHODIMP Revert(void) override;
	virtual STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) override;
	virtual STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) override;
	virtual STDMETHODIMP Stat(STATSTG *pstatstg, DWORD grfStatFlag) override;
	virtual STDMETHODIMP Clone(IStream  **ppstm) override;

	~KStream();
protected:
	unsigned int m_cRef;
private:
	charT *m_buffer;						//缓冲区指针
	ULONG m_size;						//缓冲区大小
	ULONG m_currenPosition;		//当前读写位置
};

template<typename charT>
HRESULT __stdcall KStream<charT>::QueryInterface(const IID& iid,  void **ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == IID_IStream)
	{
		*ppv = static_cast<IStream*>(this);
	}
	else
	{
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown *>(*ppv)->AddRef();
	return S_OK;
}

template<typename charT>
ULONG __stdcall KStream<charT>::AddRef()
{
	return ++m_cRef;
}

template<typename charT>
ULONG __stdcall KStream<charT>::Release()
{
	if (--m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

template<typename charT>
KStream<charT>::KStream()
	: m_buffer(nullptr), 
	m_size(0), 
	m_currenPosition(0),
	m_cRef(0)
{
	//ULONG nSize = 0；
	//	char* c = new char[1024];
	//HRESULT hr = Read(c, 1024, &nSize);
}

template<typename charT>
KStream<charT>::KStream(const charT* str)		//str必须初始化
{
	m_size = StrLen(str);
	m_buffer = new charT[m_size];
	//strncpy_s(m_buffer, m_size+1, str, m_size);
	memcpy(m_buffer, str, m_size);
	m_currenPosition = 0;		
	m_cRef = 1;
}

template<typename charT>
STDMETHODIMP KStream<charT>::Read(void *pv, ULONG cb, ULONG *pcbRead)		//把m_buffer读出到pv,同时移动位置指针
{
	//ULONG lenOfPv = *pcbRead;
	//if ((m_size -m_currenPosition) > lenOfPv)		//剩余缓冲区大小 > 目标大小,避免溢出
	//{
	//	if ( cb > lenOfPv)			//要读出的大小 > 目标大小
	//	{
	//		strncpy_s(static_cast<charT*>(pv), lenOfPv+1, m_buffer+m_currenPosition, lenOfPv);
	//		m_currenPosition = 0;
	//		*pcbRead = lenOfPv;
	//	}
	//	else
	//	{
	//		strncpy_s(static_cast<charT*>(pv), lenOfPv, m_buffer + m_currenPosition, cb);
	//		m_currenPosition += cb;
	//		*pcbRead = cb;
	//	}
	//}
	//else			//剩余缓冲区大小 < 目标大小
	//{
	//	if (cb > (m_size - m_currenPosition))
	//	{
	//		strncpy_s(static_cast<charT*>(pv), lenOfPv, m_buffer + m_currenPosition, m_size - m_currenPosition);			//把缓冲区内剩余数据全部读出
	//		m_currenPosition = 0;
	//		*pcbRead = m_size - m_currenPosition;
	//	}
	//	else
	//	{
	//		strncpy_s(static_cast<charT*>(pv), lenOfPv, m_buffer + m_currenPosition, cb);
	//		m_currenPosition += cb;
	//		*pcbRead = cb;
	//	}
	//}
	ULONG toRead = cb > (m_size - m_currenPosition) ? (m_size - m_currenPosition) : cb;
	memcpy(static_cast<charT*>(pv), m_buffer + m_currenPosition, toRead);
	*pcbRead = toRead;
	return S_OK;
}

template<typename charT>
STDMETHODIMP KStream<charT>::Write(const void *pv, ULONG cb, ULONG *pcbWritten)		//将pv所指的cb个数据写入到m_buffer
{
	//ULONG lenOfPv = StrLen(static_cast<const charT*>(pv) );
	//if (cb > lenOfPv)
	//	cb = lenOfPv;
	//if (cb > (m_size - m_currenPosition))		//缓冲区溢出
	//{
	//	charT *tmp = new charT[cb + m_size + 1];		//扩容
	//	strncpy_s(tmp, cb + m_size + 1, m_buffer, m_size);		//原有数据
	//	strncpy_s(tmp + m_currenPosition, cb+1, static_cast<const charT*>(pv), cb);			//新数据,继续从当前读写位置开始写入
	//	if (m_buffer != nullptr)
	//		delete[] m_buffer;
	//	m_buffer = tmp;
	//	m_size += cb;
	//	//m_currenPosition = 0;
	//	m_currenPosition += cb;			//读写指针要不要归零？(下次写操作从头开始) : (接着上次的位置写入)
	//}
	//else
	//{
	//	strncpy_s(m_buffer + m_currenPosition, m_size - m_currenPosition, static_cast<const charT*>(pv), cb);
	//	m_currenPosition += cb;
	//}
	////*pcbWritten = cb;
	if (m_size == 0)
	{
		m_buffer = new charT[cb];
		memcpy(m_buffer, static_cast<const charT*>(pv), cb);
		m_size = cb;
		AddRef();
		*pcbWritten = cb;
	}
	else
	{
		ULONG toWrite = cb > (m_size - m_currenPosition) ? (m_size - m_currenPosition) : cb;
		memcpy(m_buffer + m_currenPosition, static_cast<const charT*>(pv), toWrite);
		*pcbWritten = toWrite;
	}
	return S_OK;
}

template<typename charT>
STDMETHODIMP KStream<charT>::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
	if (dwOrigin == STREAM_SEEK_SET)		//缓冲区开头
	{
		if (dlibMove.QuadPart > m_size || dlibMove.QuadPart < 0)		//此时偏移量不能为负
		{
			plibNewPosition = nullptr;
			return E_INVALIDARG;
		}
		else
		{
			m_currenPosition = dlibMove.QuadPart;
			return S_OK;
		}
		
	}
	else if (dwOrigin == STREAM_SEEK_CUR)		//缓冲区当前位置
	{
		if ((m_currenPosition + dlibMove.QuadPart) > m_size || (m_currenPosition + dlibMove.QuadPart) < 0)		//越界重置
		{
			m_currenPosition = 0;
			plibNewPosition = nullptr;
			return E_INVALIDARG;
		}
		else
		{
			m_currenPosition += dlibMove.QuadPart;
			return S_OK;
		}
	}
	else if (dwOrigin == STREAM_SEEK_END)		//缓冲区尾部 
	{
		if (dlibMove.QuadPart > 0 || (m_currenPosition + dlibMove.QuadPart) < 0)
		{
			m_currenPosition = 0;
			plibNewPosition = nullptr;
			return E_INVALIDARG;
		}
		else
		{
			m_currenPosition += dlibMove.QuadPart;
			return S_OK;
		}
	}
	return S_OK;
}


template<typename charT>
STDMETHODIMP KStream<charT>::SetSize(ULARGE_INTEGER libNewSize)
{
	if (libNewSize.QuadPart> m_size)
	{
		charT *tmp = new charT[libNewSize.QuadPart + 1];
		//strncpy_s(tmp, libNewSize.QuadPart + 1, m_buffer, m_size);
		memcpy(tmp, m_buffer, m_size);
		memset(tmp + m_size, 0, libNewSize.QuadPart - m_size);
		if (m_buffer != nullptr)
			delete[] m_buffer;
		m_buffer = tmp;
	}
	else
	{
		m_buffer[libNewSize.QuadPart] = '\0';
	}
	m_size = libNewSize.QuadPart;
	return S_OK;
}

template<typename charT>
STDMETHODIMP KStream<charT>::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
	ULONG retVal = 0;
	ULONG toCopy = cb.QuadPart > m_size  ? m_size  : cb.QuadPart;
	pstm->Write(m_buffer, toCopy, &retVal);
	pcbRead->QuadPart =  toCopy;
	pcbWritten->QuadPart = retVal;
	return S_OK;
}

template<typename charT>
STDMETHODIMP KStream<charT>::Commit(DWORD grfCommitFlags)
{
	//TODO
	return S_OK;
}

template<typename charT>
STDMETHODIMP KStream<charT>::Revert(void)
{
	//TODO
	return S_OK;
}

template<typename charT>
STDMETHODIMP KStream<charT>::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	//TODO
	return S_OK;
}

template<typename charT>
STDMETHODIMP KStream<charT>::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	//TODO
	return S_OK;
}
template<typename charT>
STDMETHODIMP KStream<charT>::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
	return S_OK;
}

template<typename charT>
STDMETHODIMP KStream<charT>::Clone(IStream  **ppstm)
{
	//TODO
	return S_OK;
}

template<typename charT>
KStream<charT>::~KStream()
{
	delete [] m_buffer;
}
#endif