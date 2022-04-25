#pragma once
#include<memory>
#include<queue>
#include<utility>
#include<iostream>

#include "../GameInfo.h"


template<class T>
class CObjectPool 
{

private:
    std::queue<T*> objectQueue_;
    int Count;

public:
    CObjectPool(size_t initMemorySize);
    ~CObjectPool();
    T* GetMemory();
    void ReturnMemory(T* Memory);

    std::list<std::string> m_listName;

    std::list<std::string> m_listReturnName;
    std::list<void*> m_AddressList;

public:
    void AddName(const std::string& Name)
    {
        m_listName.push_back(Name);
    }

    void AddReturnName(const std::string& Name)
    {
        m_listReturnName.push_back(Name);
    }

    void AddAddress(void* mem)
    {
        m_AddressList.push_back(mem);
    }
};

template<class T>
inline CObjectPool<T>::CObjectPool(size_t initMemorySize)
{
    Count = initMemorySize;

    for (size_t i = 0; i < initMemorySize; ++i) 
    {
        objectQueue_.emplace(new T());
    }
}

template<class T>
inline CObjectPool<T>::~CObjectPool()
{
    size_t Size = objectQueue_.size();

    for (size_t i = 0; i < Size; ++i)
    {
        auto frontObject = objectQueue_.front();
        objectQueue_.pop();
        delete frontObject;
    }
}

template<class T>
inline T* CObjectPool<T>::GetMemory()
{
    if (objectQueue_.empty())
    {
        for(int i = 0 ; i < Count; ++i)
            objectQueue_.emplace(new T());
    }

    T* frontobject = std::move(objectQueue_.front());
    objectQueue_.pop();
    
   /* SharedPtr obj(frontobject.release(), [this](T* t) {
        objectQueue_.emplace(t);
    });*/

    return frontobject;
}

template<class T>
inline void CObjectPool<T>::ReturnMemory(T* Memory)
{
    objectQueue_.push(Memory);
}