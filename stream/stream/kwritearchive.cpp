#include"pch.h"
#include"KWriteArchive.h"

KWriteArchive::KWriteArchive(const char_type *filename)
{
	std::wifstream readStream(filename);
	if (!readStream.is_open())
	{
		std::cout << "failed to open " << filename << '\n';
		return;
	}
	readStream.seekg(0, readStream.end);
	ULONG lenght = readStream.tellg();
	readStream.seekg(0);
	m_cache = new  (std::nothrow) char_type[lenght + lenght / 2];
	if (m_cache == nullptr)
	{
		std::cout << "failed to allocate memory" << '\n';
		return;
	}
	memset(m_cache, 0, lenght + 1);
	readStream.read(m_cache, lenght);		//读到缓存中
	readStream.close();

	m_writeStream.open(filename);
	//m_writeStream.close();
	m_reference = 1;
	m_currSeek = 0;
	m_size = lenght + lenght / 2;					//预留部分空间作为追加写操作的缓冲区
}


STDMETHODIMP KWriteArchive::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
	if (pv)
	{
		if (cb > (m_size - m_currSeek))
			resize(pv, cb);
		else
		{
			memcpy(m_cache + m_currSeek, (char_type*)pv, cb);
			m_currSeek += cb;
		}
		*pcbWritten = cb;
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP KWriteArchive::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
	if (pv)
	{
		ULONG toRead = (m_size - m_currSeek) > cb ? cb : (m_size - m_currSeek);
		*pcbRead = toRead;
		memcpy((char_type*)pv, m_cache + m_currSeek, toRead);		//不改变指针位置
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP  KWriteArchive::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
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
			m_currSeek = static_cast<pos_type>(dlibMove.QuadPart);
			return S_OK;
		}

	}
	else if (dwOrigin == STREAM_SEEK_CUR)		//缓冲区当前位置
	{
		if ((m_currSeek + dlibMove.QuadPart) > m_size || (m_currSeek + dlibMove.QuadPart) < 0)		//越界重置
		{
			m_currSeek = 0;
			plibNewPosition = nullptr;
			return E_INVALIDARG;
		}
		else
		{
			m_currSeek += static_cast<pos_type>(dlibMove.QuadPart);
			return S_OK;
		}
	}
	else if (dwOrigin == STREAM_SEEK_END)		//缓冲区尾部 
	{
		if (dlibMove.QuadPart > 0 || (m_currSeek + dlibMove.QuadPart) < 0)
		{
			m_currSeek = 0;
			plibNewPosition = nullptr;
			return E_INVALIDARG;
		}
		else
		{
			m_currSeek += static_cast<pos_type>(dlibMove.QuadPart);
			return S_OK;
		}
	}
	return S_OK;
}

STDMETHODIMP KWriteArchive::SetSize(ULARGE_INTEGER libNewSize)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KWriteArchive::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
	if (pstm)
	{
		ULONG retVal = 0;
		ULONG toCopy = cb.QuadPart > m_size ? m_size : cb.QuadPart;
		pstm->Write(m_cache, toCopy, &retVal);
		if (pcbRead)
			pcbRead->QuadPart = toCopy;
		if (pcbWritten)
			pcbWritten->QuadPart = retVal;

		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP KWriteArchive::Commit(DWORD grfCommitFlags)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KWriteArchive::Revert(void)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KWriteArchive::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KWriteArchive::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KWriteArchive::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KWriteArchive::Clone(IStream  **ppstm)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

HRESULT __stdcall  KWriteArchive::QueryInterface(const IID& iid, void **ppv)
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

ULONG   __stdcall   KWriteArchive::AddRef()
{
	return ++m_reference;
}

ULONG   __stdcall   KWriteArchive::Release()
{
	if (--m_reference == 0)
	{
		delete this;
		return 0;
	}
	return m_reference;
}

void KWriteArchive::flush()
{
	m_writeStream.write(m_cache, m_currSeek);
	m_writeStream.flush();
	m_writeStream.seekp(0);
	memset(m_cache, 0, m_size);
}

pos_type KWriteArchive::tell() const
{
	return (m_currSeek > m_size) ? 0 : m_currSeek;
}

void KWriteArchive::seek(pos_type dwOffset)
{
	if (dwOffset <0 || dwOffset > m_size)
	{
		std::cout << "out of range!" << '\n';
		return;
	}
	m_currSeek = dwOffset;
}

void KWriteArchive::seek_end()
{
	seek(m_size);
}

void KWriteArchive::reput(size_type offset, const char_type* lpBuf, size_type cch)		//调用者负责保证cch的合理性
{
	memcpy(m_cache + offset, lpBuf, cch);
}

size_type  KWriteArchive::put(const char_type* lpBuf, size_type cch)
{
	if (cch > (m_size - m_currSeek))
		resize(lpBuf, cch);
	else
	{
		memcpy(m_cache + m_currSeek, lpBuf, cch);
		m_currSeek += cch;
	}
	return cch;
}

size_type KWriteArchive::put(const char_type* str)
{
	size_type lenght = wcslen(str);
	if (lenght > (m_size - m_currSeek))
		resize(str, lenght);
	else
	{
		memcpy(m_cache + m_currSeek, str, lenght);
		m_currSeek += lenght;
	}
	return lenght;
}

size_type KWriteArchive::put(char_type ch)
{
	const char_type* tmp = &ch;
	if (m_currSeek >= m_size)
		resize(tmp, 1);
	else
	{
		m_cache[m_currSeek++] = ch;
	}
	return 1;			//返回什么好？
}

void KWriteArchive::resize(const void*pv, ULONG cb)
{
	char_type *tmp = new char_type[(m_size + cb) + (m_size + cb) / 2];
	memcpy(tmp, m_cache, m_size);
	delete[] m_cache;
	m_cache = tmp;
	memcpy(m_cache + m_size, (char_type*)pv, cb);
	m_size = (m_size + cb) + (m_size + cb) / 2;
	m_currSeek += cb;
}

KWriteArchive::~KWriteArchive()		//清空缓冲区，并且将数据刷新到磁盘文件。
{
	delete[] m_cache;
	flush();
	m_writeStream.close();
}