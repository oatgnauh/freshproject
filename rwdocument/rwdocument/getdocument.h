// -------------------------------------------------------
// getdocument.h
// �����ߣ� ����
// ����ʱ�䣺 2018/12/03
// ���������� ��ȡ�����ĵ�
/*
1.��ȡ�����ĵ���������dump������Storage��ӦΪĿ¼��Stream��ӦΪ�������ļ���
	��νdump�� ���ǽ������ĵ��Ĳ�νṹ��ͨ���ļ�ϵͳ��Ŀ¼�ṹչ�ֳ����� dump �����ս�����ļ�ϵͳ��һ��Ŀ¼��
	���ڸ����ĵ���IStorage, ��Ӧ�����ļ�ϵͳ����һ��Ŀ¼�� ���ֺ�IStorage ��һ�£� ����IStream ������Ӧ��һ���������ļ���
	���ֺ�IStream ��һ�£� ����ΪIStream ���ݡ� ע�⽨�����ļ�ϵͳĿ¼�ṹ��֤�͸����ĵ��Ĳ�νṹһ�¡�
2.���������ĵ�������һ���������ϳ�һ�������ĵ���д�ɸ����ĵ���
	��һ�����̵�������̣� ���ļ�ϵͳ��һ��Ŀ¼����һ�������ĵ��� �����ݸ����ĵ����API�� ͨ����һ��������Ŀ¼�ṹ�������������ĵ���
	��֤���ʼ��Dump ʹ�õĸ����ĵ�����һ�¡�
3.������ɹ���������Ҫ��ʹ��֮ǰд�õ�String�࣬����ָ�롣ʹ��STL (Vector\list\map)�� ��ȡ�ĸ����ĵ�ͨ��wps office ������doc��xls��ppt ��ʽ�ļ���Ϊ�����ĵ���
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
