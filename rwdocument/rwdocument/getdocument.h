// -------------------------------------------------------
// getdocument.h
// 创建者： 黄涛
// 创建时间： 2018/12/03
// 功能描述： 读取复合文档
/*
1.读取复合文档，将内容dump出来，Storage对应为目录，Stream对应为二进制文件。
	所谓dump， 就是将复合文档的层次结构，通过文件系统的目录结构展现出来。 dump 的最终结果是文件系统的一个目录，
	对于复合文档的IStorage, 相应的在文件系统建立一个目录， 名字和IStorage 名一致； 对于IStream 建立对应的一个二进制文件，
	名字和IStream 名一致， 内容为IStream 内容。 注意建立的文件系统目录结构保证和复合文档的层次结构一致。
2.产生复合文档，将上一步的输出组合成一个复合文档，写成复合文档。
	上一个过程的逆向过程， 从文件系统的一个目录产生一个复合文档。 即根据复合文档相关API， 通过第一步产生的目录结构，创建出复合文档，
	保证和最开始的Dump 使用的复合文档内容一致。
3.任务完成过程若有需要，使用之前写好的String类，智能指针。使用STL (Vector\list\map)。 读取的复合文档通过wps office 创建，doc、xls、ppt 格式文件均为复合文档。
*/
// Copyright 2013 Kingsoft
// --------------------------------------------------------
#pragma once
#ifndef   _RWDOCUMENT_GETDOCUMENT_H__
#define  _RWDOCUMENT_GETDOCUMENT_H__
#include<Windows.h>

class Document 
{
public:
	Document();
	Document(const WCHAR *FILENAME);
	~Document();

	void CreateDocument();
private:
	IStorage *pStgRoot, *pStgSub;
	IStream *pStream;
	void Dump();
//
//public:
//	Document();
//	Document(const wchar_t* filename);
//public:		//IStorage
//	virtual HRESULT STDMETHODCALLTYPE CreateStream(
//		/* [string][in] */ __RPC__in_string const OLECHAR *pwcsName,
//		/* [in] */ DWORD grfMode,
//		/* [in] */ DWORD reserved1,
//		/* [in] */ DWORD reserved2,
//		/* [out] */ __RPC__deref_out_opt IStream **ppstm) override;
//
//	virtual /* [local] */ HRESULT STDMETHODCALLTYPE OpenStream(
//		/* [annotation][string][in] */
//		_In_z_  const OLECHAR *pwcsName,
//		/* [annotation][unique][in] */
//		_Reserved_  void *reserved1,
//		/* [in] */ DWORD grfMode,
//		/* [in] */ DWORD reserved2,
//		/* [annotation][out] */
//		_Outptr_  IStream **ppstm) override;
//
//	virtual HRESULT STDMETHODCALLTYPE CreateStorage(
//		/* [string][in] */ __RPC__in_string const OLECHAR *pwcsName,
//		/* [in] */ DWORD grfMode,
//		/* [in] */ DWORD reserved1,
//		/* [in] */ DWORD reserved2,
//		/* [out] */ __RPC__deref_out_opt IStorage **ppstg) override;
//
//	virtual HRESULT STDMETHODCALLTYPE OpenStorage(
//		/* [string][unique][in] */ __RPC__in_opt_string const OLECHAR *pwcsName,
//		/* [unique][in] */ __RPC__in_opt IStorage *pstgPriority,
//		/* [in] */ DWORD grfMode,
//		/* [unique][in] */ __RPC__deref_opt_in_opt SNB snbExclude,
//		/* [in] */ DWORD reserved,
//		/* [out] */ __RPC__deref_out_opt IStorage **ppstg) override;
//
//	virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo(
//		/* [in] */ DWORD ciidExclude,
//		/* [annotation][size_is][unique][in] */
//		_In_reads_opt_(ciidExclude)  const IID *rgiidExclude,
//		/* [annotation][unique][in] */
//		_In_opt_  SNB snbExclude,
//		/* [annotation][unique][in] */
//		_In_  IStorage *pstgDest) override;
//
//	virtual HRESULT STDMETHODCALLTYPE MoveElementTo(
//		/* [string][in] */ __RPC__in_string const OLECHAR *pwcsName,
//		/* [unique][in] */ __RPC__in_opt IStorage *pstgDest,
//		/* [string][in] */ __RPC__in_string const OLECHAR *pwcsNewName,
//		/* [in] */ DWORD grfFlags) override;
//
//	virtual HRESULT STDMETHODCALLTYPE Commit(
//		/* [in] */ DWORD grfCommitFlags) override;
//
//	virtual HRESULT STDMETHODCALLTYPE Revert(void) override;
//
//	virtual /* [local] */ HRESULT STDMETHODCALLTYPE EnumElements(
//		/* [annotation][in] */
//		_Reserved_  DWORD reserved1,
//		/* [annotation][size_is][unique][in] */
//		_Reserved_  void *reserved2,
//		/* [annotation][in] */
//		_Reserved_  DWORD reserved3,
//		/* [annotation][out] */
//		_Outptr_  IEnumSTATSTG **ppenum) override;
//
//	virtual HRESULT STDMETHODCALLTYPE DestroyElement(
//		/* [string][in] */ __RPC__in_string const OLECHAR *pwcsName) override;
//
//	virtual HRESULT STDMETHODCALLTYPE RenameElement(
//		/* [string][in] */ __RPC__in_string const OLECHAR *pwcsOldName,
//		/* [string][in] */ __RPC__in_string const OLECHAR *pwcsNewName) override;
//
//	virtual HRESULT STDMETHODCALLTYPE SetElementTimes(
//		/* [string][unique][in] */ __RPC__in_opt_string const OLECHAR *pwcsName,
//		/* [unique][in] */ __RPC__in_opt const FILETIME *pctime,
//		/* [unique][in] */ __RPC__in_opt const FILETIME *patime,
//		/* [unique][in] */ __RPC__in_opt const FILETIME *pmtime) override;
//
//	virtual HRESULT STDMETHODCALLTYPE SetClass(
//		/* [in] */ __RPC__in REFCLSID clsid) override;
//
//	virtual HRESULT STDMETHODCALLTYPE SetStateBits(
//		/* [in] */ DWORD grfStateBits,
//		/* [in] */ DWORD grfMask) override;
//
//	virtual HRESULT STDMETHODCALLTYPE Stat(
//		/* [out] */ __RPC__out STATSTG *pstatstg,
//		/* [in] */ DWORD grfStatFlag) override;
//
//public:		//IStream
//	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek(
//		/* [in] */ LARGE_INTEGER dlibMove,
//		/* [in] */ DWORD dwOrigin,
//		/* [annotation] */
//		_Out_opt_  ULARGE_INTEGER *plibNewPosition) override;
//
//	virtual HRESULT STDMETHODCALLTYPE SetSize(
//		/* [in] */ ULARGE_INTEGER libNewSize) override;
//
//	virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo(
//		/* [annotation][unique][in] */
//		_In_  IStream *pstm,
//		/* [in] */ ULARGE_INTEGER cb,
//		/* [annotation] */
//		_Out_opt_  ULARGE_INTEGER *pcbRead,
//		/* [annotation] */
//		_Out_opt_  ULARGE_INTEGER *pcbWritten) override;
//
//	virtual HRESULT STDMETHODCALLTYPE Commit(
//		/* [in] */ DWORD grfCommitFlags) override;
//
//	virtual HRESULT STDMETHODCALLTYPE Revert(void) override;
//
//	virtual HRESULT STDMETHODCALLTYPE LockRegion(
//		/* [in] */ ULARGE_INTEGER libOffset,
//		/* [in] */ ULARGE_INTEGER cb,
//		/* [in] */ DWORD dwLockType) override;
//
//	virtual HRESULT STDMETHODCALLTYPE UnlockRegion(
//		/* [in] */ ULARGE_INTEGER libOffset,
//		/* [in] */ ULARGE_INTEGER cb,
//		/* [in] */ DWORD dwLockType) override;
//
//	virtual HRESULT STDMETHODCALLTYPE Stat(
//		/* [out] */ __RPC__out STATSTG *pstatstg,
//		/* [in] */ DWORD grfStatFlag) override;
//
//	virtual HRESULT STDMETHODCALLTYPE Clone(
//		/* [out] */ __RPC__deref_out_opt IStream **ppstm) override;
//

};


Document::Document()
{

}

Document::Document(const WCHAR *FILENAME)
{

}

Document::~Document()
{

}

void Document::CreateDocument()
{


}

void Document::Dump()
{

}
#endif // !_RWDOCUMENT_GETDOCUMENT_H__
