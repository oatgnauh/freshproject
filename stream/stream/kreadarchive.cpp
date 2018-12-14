#include"pch.h"
#include"KReadArchive.h"

KReadArchive::KReadArchive()
	:m_reference(0),
	m_cache(nullptr),
	m_currSeek(0),
	m_size(0)
{

}

KReadArchive::KReadArchive(const char_type* filename)		//暂时不考虑大文件操作
{
	std::wifstream readStream;
	readStream.open(filename);
	if (!readStream.is_open())
	{
		std::cout << "failed to open " << filename << '\n';
		return;
	}

	readStream.seekg(0, std::ios::end);
	UINT lenght = readStream.tellg();		//的出来是字符数不是字节数
	readStream.seekg(0, std::ios::beg);
	m_cache = new (std::nothrow) char_type[2*lenght + 1];
	if (m_cache == nullptr)
	{
		std::cout << "failed to allocate memory" << '\n';
		return;
	}
	memset(m_cache, 0, lenght + 1);
	readStream.read(m_cache, lenght);
	readStream.close();
	std::wcout << m_cache << std::endl;
	m_reference = 1;
	m_currSeek = 0;
	m_size = lenght;
	m_reference = 1;

}

KReadArchive::~KReadArchive()
{
	delete[] m_cache;
}

STDMETHODIMP KReadArchive::Read(void *pv, ULONG cb, ULONG *pcbRead)		//认为pv的内存大小<=cb
{
	if (pv)
	{
		ULONG toRead = m_size - m_currSeek;
		toRead = (cb > toRead) ? toRead : cb;
		*pcbRead = toRead;
		memcpy((char_type*)pv, m_cache + m_currSeek, toRead);
		m_currSeek += toRead;			//若m_currSeek > m_size ,由调用者重置此指针
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP KReadArchive::Write(const void *pv, ULONG cb, ULONG *pcbWritten)		//禁止写
{
	//	ULONG toWrite = m_size - m_currSeek;
	//	toWrite = (cb > toWrite) ? toWrite : cb;
	//	memcpy(m_cache, (char_type*)pv, toWrite);
	//	*pcbWritten = toWrite;
	//	m_currSeek += toWrite;
	//	return S_OK;
	//}
	std::cout << "can not write into this memory" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KReadArchive::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
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

STDMETHODIMP KReadArchive::SetSize(ULARGE_INTEGER libNewSize)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KReadArchive::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
	//pstm->Write(m_cache, cb.QuadPart, &pcbWritten);
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

STDMETHODIMP KReadArchive::Commit(DWORD grfCommitFlags)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KReadArchive::Revert(void)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KReadArchive::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KReadArchive::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KReadArchive::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

STDMETHODIMP KReadArchive::Clone(IStream  **ppstm)
{
	std::cout << "invalid operation!" << '\n';
	return E_NOTIMPL;
}

HRESULT __stdcall  KReadArchive::QueryInterface(const IID& iid, void **ppv)
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
ULONG   __stdcall   KReadArchive::AddRef()
{
	return ++m_reference;
}

ULONG   __stdcall   KReadArchive::Release()
{
	if (--m_reference == 0)
	{
		delete this;
		return 0;
	}
	return m_reference;
}

bool KReadArchive::unget()		//这个函数要干什么？？
{
	//TODO
	return true;
}

void KReadArchive::skipws()
{
	//TODO
}

size_type	KReadArchive::skip(size_type nMax)
{
	//TODO
	return 0;
}

inline void KReadArchive::seek_end()
{
	m_currSeek = m_size;
}

inline void KReadArchive::clear_cache()
{
	memset(m_cache, 0, m_size);
}

void KReadArchive::seek(pos_type dwOffset)
{
	if (dwOffset <0 || dwOffset > m_size)
	{
		std::cout << "out of range!" << '\n';
		return;
	}
	m_currSeek = dwOffset;
}

unsigned	KReadArchive::get_digit()
{
	while (!isdigit(m_cache[m_currSeek++]))
		;
	return (m_cache[--m_currSeek] - '0');

}

char_type	KReadArchive::getnws()
{
	while (m_cache[m_currSeek++] == ' ')
		;
	return m_cache[--m_currSeek];
}

inline char_type	KReadArchive::peek()
{
	return m_cache[m_currSeek];
}

inline pos_type	KReadArchive::tell() const
{
	return m_currSeek;
}

inline pos_type	KReadArchive::size() const
{
	return m_size;
}

inline char_type	KReadArchive::get()
{
	return m_cache[m_currSeek++];
}

size_type	KReadArchive::get(char_type* lpBuf, size_type nMax)		//取走&&移动指针
{
	size_type toGet = (m_size - m_currSeek) > nMax ? nMax : (m_size - m_currSeek);
	memcpy(lpBuf, m_cache + m_currSeek, toGet);
	m_currSeek += toGet;
	return toGet;		//返回什么好？
}
