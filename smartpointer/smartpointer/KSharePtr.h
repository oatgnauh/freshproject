// -------------------------------------------------------
// KSharedPtr.h
// �����ߣ� ����
// ����ʱ�䣺 2018/11/21
// ���������� ���ܹ���̬�ڴ�
// Copyright 2013 Kingsoft
// --------------------------------------------------------

#pragma once
#ifndef  _FRESHPROJECT_KSHAREPTR_H__
#define _FRESHPROJECT_KSHAREPTR_H__

typedef  unsigned int sizeType;
template<typename T>
class KSharedPtr{
public:
	KSharedPtr();
	~KSharedPtr();
	KSharedPtr(T *resource);
	KSharedPtr(T *resource, bool flag);		//�����������ʱ���봫��flag�����Ա����������
	KSharedPtr(KSharedPtr const &rhs);
	KSharedPtr& operator=(KSharedPtr const &rhs);
	KSharedPtr& operator=(T *resource);
	//explicit KSharedPtr(std::auto_ptr<T> & rhs);
	//KSharedPtr& operator=(std::auto_ptr<T> & rhs);
	T&	operator*()   const;
	T*		operator->() const;
	T&	operator[](size_t pos) const;

	void			reset(T * p = 0);
	T*				get() const;
	sizeType	use_count() const;
private:
	T				* m_pointer;
	sizeType  *m_reference;		//ʹ�ö�̬�ڴ汣�����ֵ
	bool			m_isArray;

	void			garbageCollection();
};

template<typename T>
KSharedPtr<T>::KSharedPtr()
	:m_pointer(nullptr),
	m_reference(new sizeType(0)),
	m_isArray(0)
{

}

template<typename T>
KSharedPtr<T>::KSharedPtr(T* p)		//����һ��new������ָ��
	:m_pointer(p),
	m_reference(new sizeType(1)),
	m_isArray(0)		
{
}

template<typename T>
KSharedPtr<T>::KSharedPtr(T* resource, bool flag)
	:m_pointer(resource),
	m_reference(new sizeType(1)),
	m_isArray(1)
{

}

//template<typename T>
//KSharedPtr<T>::KSharedPtr(std::auto_ptr<T> & rhs)		//auto_ptr��ת�ƶ��󣬸�ֵ����������,C++11��׼��������
//{
//	m_pointer = rhs.get();	//��ȡԭʼָ��
//	m_reference++;
//}

template<typename T>
KSharedPtr<T>::KSharedPtr(const KSharedPtr  & rhs)		//�������죬�β����ü�������
{
	if (m_pointer != rhs.m_pointer)		
	{
		m_pointer = rhs.m_pointer;
		m_reference = rhs.m_reference;	//ָ��ͬһ�鶯̬�ڴ�
		++*m_reference;						//���ü������ӣ�����ָ��ͬһ���ַ����˱仯�ܱ����ж���֪��
		m_isArray = rhs.m_isArray;
	}
	
}

template<typename T>
KSharedPtr<T>&  KSharedPtr<T>::operator=(KSharedPtr<T> const & rhs)
{
	if (m_pointer != rhs.m_pointer)									//�������Ҹ�ֵ
	{
		if(*m_reference == 0 && m_pointer == nullptr)		//�������ֵ�Ķ����Ǹ��յ�����ָ��
		{
			m_pointer = rhs.m_pointer;
			m_reference = rhs.m_reference;
			++*m_reference;
			m_isArray = rhs.m_isArray;
		}
		else
		{
			garbageCollection();
			m_pointer = rhs.m_pointer;
			m_reference = rhs.m_reference;
			++*m_reference;		//Ȼ�������Ҳ�������ָ��������ü�����Ϊ�����ӣ���Ϊ��ʱ��������ָ������Ҳ�����ָ�����
			m_isArray = rhs.m_isArray;
		}
	}
	return *this;
}

template<typename T>
KSharedPtr<T>& KSharedPtr<T>::operator=(T* resource)		//������ʹ�ö�̬��������ֵ
{
	if (resource != nullptr && m_isArray == false)
	{
		garbageCollection();
		m_pointer = resource;
		++*m_reference;
	}
	else
	{
		delete resource;
		resource = nullptr;
	}
	return *this;
}

template<typename T>
T& KSharedPtr<T>::operator*() const		//������
{
	return *m_pointer;
}

template<typename T>
T* KSharedPtr<T>::operator->() const		//����ԭʼָ��
{
	return m_pointer;
}

template<typename T>
T& KSharedPtr<T>::operator[](size_t pos) const
{
	if( ! m_isArray)
	{
		std::cout<<"invalid operation!"<<std::endl;
		//exit(0);	
		return 0;
	}
	return *(m_pointer + pos);
}

template<typename T>
T* KSharedPtr<T>::get() const
{
	return m_pointer;
}

template<typename T>
unsigned int KSharedPtr<T>::use_count() const
{
	return *m_reference;
}

template<typename T>
void KSharedPtr<T>::reset(T* p)			//������ָ��֮����໥��ֵ
{
	garbageCollection();

	if(p != nullptr)
	{
		m_pointer = p;
		m_reference = new sizeType(1);
	}
	else
	{
		m_pointer = nullptr;
		//*m_reference  = 0;
	}
}

template<typename T>
void KSharedPtr<T>::garbageCollection()
{
	if(m_pointer != nullptr)
	{
		--*m_reference;								//ÿ���������ü�����һ
		if (*m_reference == 0)
		{
			if(m_isArray)
				delete [] m_pointer;
			else
				delete m_pointer;
			delete m_reference;
			m_pointer = nullptr;
			m_reference = nullptr;
		}
	}
}

template<typename T>
KSharedPtr<T>::~KSharedPtr()
{
	garbageCollection();
}

template<typename T>
bool operator==(const KSharedPtr<T>& lhs, const KSharedPtr<T>& rhs) 
{
	return (lhs.m_pointer == rhs.m_pointer);
}

template<typename T>
bool operator!=(const KSharedPtr<T>& lhs, const KSharedPtr<T>& rhs)
{
	return (lhs.m_pointer != rhs.m_pointer);
}


#endif