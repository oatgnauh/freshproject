// -------------------------------------------------------
// KStream.h
// �����ߣ� ����
// ����ʱ�䣺 2018/11/22
// ���������� ���ڱ�׼C��Windows APIʵ��һ��KStream�࣬ʵ��IStream�ӿ�(IStream != istream)
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
	charT *m_buffer;						//������ָ��
	ULONG m_size;						//��������С
	ULONG m_currenPosition;		//��ǰ��дλ��
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
	//ULONG nSize = 0��
	//	char* c = new char[1024];
	//HRESULT hr = Read(c, 1024, &nSize);
}

template<typename charT>
KStream<charT>::KStream(const charT* str)		//str�����ʼ��
{
	m_size = StrLen(str);
	m_buffer = new charT[m_size];
	//strncpy_s(m_buffer, m_size+1, str, m_size);
	memcpy(m_buffer, str, m_size);
	m_currenPosition = 0;		
	m_cRef = 1;
}

template<typename charT>
STDMETHODIMP KStream<charT>::Read(void *pv, ULONG cb, ULONG *pcbRead)		//��m_buffer������pv,ͬʱ�ƶ�λ��ָ��
{
	//ULONG lenOfPv = *pcbRead;
	//if ((m_size -m_currenPosition) > lenOfPv)		//ʣ�໺������С > Ŀ���С,�������
	//{
	//	if ( cb > lenOfPv)			//Ҫ�����Ĵ�С > Ŀ���С
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
	//else			//ʣ�໺������С < Ŀ���С
	//{
	//	if (cb > (m_size - m_currenPosition))
	//	{
	//		strncpy_s(static_cast<charT*>(pv), lenOfPv, m_buffer + m_currenPosition, m_size - m_currenPosition);			//�ѻ�������ʣ������ȫ������
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
STDMETHODIMP KStream<charT>::Write(const void *pv, ULONG cb, ULONG *pcbWritten)		//��pv��ָ��cb������д�뵽m_buffer
{
	//ULONG lenOfPv = StrLen(static_cast<const charT*>(pv) );
	//if (cb > lenOfPv)
	//	cb = lenOfPv;
	//if (cb > (m_size - m_currenPosition))		//���������
	//{
	//	charT *tmp = new charT[cb + m_size + 1];		//����
	//	strncpy_s(tmp, cb + m_size + 1, m_buffer, m_size);		//ԭ������
	//	strncpy_s(tmp + m_currenPosition, cb+1, static_cast<const charT*>(pv), cb);			//������,�����ӵ�ǰ��дλ�ÿ�ʼд��
	//	if (m_buffer != nullptr)
	//		delete[] m_buffer;
	//	m_buffer = tmp;
	//	m_size += cb;
	//	//m_currenPosition = 0;
	//	m_currenPosition += cb;			//��дָ��Ҫ��Ҫ���㣿(�´�д������ͷ��ʼ) : (�����ϴε�λ��д��)
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
	if (dwOrigin == STREAM_SEEK_SET)		//��������ͷ
	{
		if (dlibMove.QuadPart > m_size || dlibMove.QuadPart < 0)		//��ʱƫ��������Ϊ��
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
	else if (dwOrigin == STREAM_SEEK_CUR)		//��������ǰλ��
	{
		if ((m_currenPosition + dlibMove.QuadPart) > m_size || (m_currenPosition + dlibMove.QuadPart) < 0)		//Խ������
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
	else if (dwOrigin == STREAM_SEEK_END)		//������β�� 
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