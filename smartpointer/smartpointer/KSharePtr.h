// -------------------------------------------------------
// KSharedPtr.h
// 创建者： 黄涛
// 创建时间： 2018/11/21
// 功能描述： 智能管理动态内存
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
	KSharedPtr(T *resource, bool flag);		//传入数组对象时必须传入flag参数以标明数组身份
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
	sizeType  *m_reference;		//使用动态内存保存计数值
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
KSharedPtr<T>::KSharedPtr(T* p)		//接受一个new出来的指针
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
//KSharedPtr<T>::KSharedPtr(std::auto_ptr<T> & rhs)		//auto_ptr会转移对象，赋值后自身清零,C++11标准中已抛弃
//{
//	m_pointer = rhs.get();	//获取原始指针
//	m_reference++;
//}

template<typename T>
KSharedPtr<T>::KSharedPtr(const KSharedPtr  & rhs)		//拷贝构造，形参引用计数增加
{
	if (m_pointer != rhs.m_pointer)		
	{
		m_pointer = rhs.m_pointer;
		m_reference = rhs.m_reference;	//指向同一块动态内存
		++*m_reference;						//引用计数增加，由于指向同一块地址，因此变化能被所有对象知晓
		m_isArray = rhs.m_isArray;
	}
	
}

template<typename T>
KSharedPtr<T>&  KSharedPtr<T>::operator=(KSharedPtr<T> const & rhs)
{
	if (m_pointer != rhs.m_pointer)									//避免自我赋值
	{
		if(*m_reference == 0 && m_pointer == nullptr)		//如果被赋值的对象是个空的智能指针
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
			++*m_reference;		//然后增加右操作数所指对象的引用计数（为何增加：因为此时做操作数指向对象即右操作数指向对象）
			m_isArray = rhs.m_isArray;
		}
	}
	return *this;
}

template<typename T>
KSharedPtr<T>& KSharedPtr<T>::operator=(T* resource)		//不允许使用动态数组来赋值
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
T& KSharedPtr<T>::operator*() const		//解引用
{
	return *m_pointer;
}

template<typename T>
T* KSharedPtr<T>::operator->() const		//返回原始指针
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
void KSharedPtr<T>::reset(T* p)			//非智能指针之间的相互赋值
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
		--*m_reference;								//每次析构引用计数减一
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