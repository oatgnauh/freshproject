// -------------------------------------------------------
// KComPtr.h
// �����ߣ� ����
// ����ʱ�䣺 2018/11/22
// ���������� COM�ӿڶ���ʹ�����ü���ģ��(COM����ʹ�����ü���������ָ��û�����ü�����Ա)
// Copyright 2013 Kingsoft
// --------------------------------------------------------
#pragma once

#ifndef  _FRESHPROJECT_KCOMPTR_H__
#define _FRESHPROJECT_KCOMPTR_H__
#include<Windows.h>
#include<atlbase.h>

template<typename E>			//E��COM�ӿڣ����Ƕ���
class KComPtr
{
public:
	KComPtr();
	~KComPtr();
	KComPtr(E* p);
	KComPtr(const KComPtr<E>& p);
	KComPtr(IUnknown* pUnk);
	template <class Type>  KComPtr(const KComPtr<Type>& p);

	E* operator=(E* p);
	E* operator=(const KComPtr<E>& p);
	template <class Type> E* operator=(const KComPtr<Type>& p);
	E* operator=(IUnknown* pUnk);

	E* get();
	void attach(E* p);
	E* detach();
	void clear();
	E& operator*() const;
	operator E*() const;
	E* operator->() const;
	E** operator&();
	BOOL operator!() const;
	BOOL operator==(E* p) const;
	BOOL operator!=(E* p) const;
protected:
	E *m_data;
};

template<typename E>
KComPtr<E>::~KComPtr()
{
	if (m_data != nullptr)
		m_data->Release();
}

template<typename E>
KComPtr<E>::KComPtr()
	:m_data(nullptr)
{

}

template<typename E>
KComPtr<E>::KComPtr(E *p)
{
	m_data = p;
	if(m_data)	//����жϺ���Ҫ
		m_data->AddRef();
}

template<typename E>
KComPtr<E>::KComPtr(const KComPtr<E>& rhs)
{
	m_data = rhs.m_data;
	if(m_data)
		m_data->AddRef();			//ʹ�ö����AddRef�������������ü���
}

//������COM�ӿ�����ʼ����ʵ�����ͺ��β����Ͳ�һ��ʱ���ô˹��캯��
template<typename E>
KComPtr<E>::KComPtr(IUnknown* pUnk)		//����һ��COM�ӿ�,��IStorage
{
	if(pUnk)
		pUnk->QueryInterface(IID_IUnknown, (void **)&m_data);
	//QueryInterface�����������ü���

}

//ʵ��һ����ʽ����ת��
template<typename E>
template <class Type>
KComPtr<E>::KComPtr(const KComPtr<Type>& rhs)
{
	m_data = nullptr;
	if(rhs)
		rhs->QueryInterface(__uuidof(Type), (void**)&m_data);
}

template<typename E>
E* KComPtr<E>::operator=(E* p)
{
	if (p)
		p->AddRef();
	if (m_data)
		m_data->Release();
	m_data = p;
	return m_data;
}

template<typename E>
E* KComPtr<E>::operator=(const KComPtr<E>& rhs)
{
	//if (m_data != nullptr)
	//{
	//	m_data->Release();
	//	m_data = rhs.m_data;
	//	m_data->AddRef();
	//}
	if (rhs.m_data)
		rhs.m_data->AddRef();
	if (m_data)
		m_data->Release();		//�˶������һ��������
	m_data = rhs.m_data;
	return m_data;
}

template<typename E>
template <class Type> 
E* KComPtr<E>::operator=(const KComPtr<Type>& rhs)
{
	/*
		KComPtr<A> pA;
		KComPtr<B> pB;
		pB = pA;
	*/ 
	if (m_data != nullptr)
	{
		m_data->Release();
		rhs->QueryInterface(__uuidof(Type), (void**)&m_data);
	}
	return m_data;
}

template<typename E>
E* KComPtr<E>::operator=(IUnknown* pUnk)		//TODO
{
	if (m_data != nullptr && pUnk != nullptr)
	{
		m_data->Release();
		pUnk->QueryInterface(IID_IUnknown, (void **)&m_data);
	}
	return m_data;
}


template<typename E>
E* KComPtr<E>::get()
{
	return m_data;
}
template<typename E>
void KComPtr<E>::attach(E* p)
{
	if (p != nullptr)
	{
		if(m_data)
			m_data->Release();
		m_data = p;
		m_data->AddRef();
	}
}

template<typename E>
E* KComPtr<E>::detach()
{
	E* tmp = m_data;
	m_data->Release();		//�����������ʲô������
	m_data = nullptr;
	return tmp;
}

template<typename E>
void KComPtr<E>::clear()
{
	auto count = m_data->Release();
	while (count != 0)
	{
		count = m_data->Release();
	}
	m_data = nullptr;
}

template<typename E>
inline E& KComPtr<E>::operator*() const
{
	return *m_data;
}

template<typename E>
inline KComPtr<E>::operator E*() const
{
	return *this;
}

template<typename E>
inline E* KComPtr<E>::operator->() const
{
	return m_data;
}

template<typename E>
inline E** KComPtr<E>::operator&()
{
	return &m_data;
}

template<typename E>
inline BOOL KComPtr<E>::operator!() const
{
	return (m_data != nullptr);
}

template<typename E>
inline BOOL KComPtr<E>::operator==(E* p) const
{
	return (m_data == p);
}

template<typename E>
inline BOOL KComPtr<E>::operator!=(E* p) const
{
	return (m_data != p);
}


#endif