// -------------------------------------------------------
// KStream.h
// 创建者： 黄涛
// 创建时间： 2018/11/26
// 功能描述： 基于IStream 实现一个文件读取辅助类KReadArchive
// Copyright 2013 Kingsoft
// --------------------------------------------------------
#pragma once
#ifndef  _STREAM_WRITEARCHIVE_H__
#define _STREAM_WRITEARCHIVE_H__
#include<windows.h>
#include<fstream>
#include<iostream>
typedef unsigned int pos_type;
typedef unsigned int size_type;
typedef wchar_t			  char_type;

class KWriteArchive : public IStream
{
public:
	explicit KWriteArchive(const char_type *filename);
	~KWriteArchive();
	virtual HRESULT __stdcall  QueryInterface(const IID& iid, void **ppv) override;
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

	void flush();						
	pos_type tell() const;		// 取得当前文件写指针的偏移量, 包括缓存写长度
	void seek(pos_type dwOffset);
	void seek_end();
	void reput(size_type offset, const char_type* lpBuf, size_type cch);
	size_type put(const char_type* lpBuf, size_type cch);
	size_type put(const char_type* str);
	size_type put(char_type ch);
protected:
	size_type m_reference;
private:
	std::wofstream m_writeStream;
	char_type* m_cache;			
	pos_type    m_currSeek;
	pos_type    m_size;
private:
	void resize(const void*str, ULONG newsize);
};


#endif