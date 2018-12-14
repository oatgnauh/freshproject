// -------------------------------------------------------
// KStream.h
// 创建者： 黄涛
// 创建时间： 2018/11/26
// 功能描述： 基于IStream 实现一个文件读取辅助类KReadArchive
// Copyright 2013 Kingsoft
// --------------------------------------------------------
#pragma once
#ifndef  _STREAM_KREADARCHIVE_H__
#define _STREAM_KREADARCHIVE_H__
#include <windows.h>
#include <fstream>
#include<iostream>
#include<ObjBase.h>
typedef unsigned int pos_type;
typedef unsigned int size_type;
typedef wchar_t			  char_type;

class KReadArchive :public IStream
{
public:
	KReadArchive();
	KReadArchive(const char_type* filename);
	~KReadArchive();
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

	bool				unget();
	void				skipws();					//跳过空白字符
	void				seek_end();
	void				clear_cache();
	void				seek(pos_type dwOffset);
	unsigned		get_digit();		//获得数字字符，直到遇到一个非digit字符为止
	char_type		get();
	char_type		peek();
	char_type		getnws();			//取得下一个非空白字符
	pos_type		tell() const;		// 注意缓存长度
	pos_type		size() const;
	size_type		skip(size_type nMax);
	size_type		get(char_type* lpBuf, size_type nMax);
	//要求： 实现上采用缓存提高读取效率，clear_cache 方法为清空缓存。
protected:
	size_type m_reference;
private:
	char_type* m_cache;			//只读缓存
	pos_type    m_currSeek;
	pos_type    m_size;
};


#endif