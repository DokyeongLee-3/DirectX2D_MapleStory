#pragma once
#include<memory>
#include<queue>
#include<utility>
#include<iostream>

#include "GameInfo.h"

template<class T>
class CObjectPool 
{
private:
    std::queue<T*> m_ObjectQueue;
    int Count;

public:
    CObjectPool(size_t initMemorySize);
    ~CObjectPool();
    T* GetMemory();
    void ReturnMemory(T* Memory);


public:
    void ClearAndReAllocPool()
    {
        size_t Size = m_ObjectQueue.size();

        for (size_t i = 0; i < Size; ++i)
        {
            auto frontObject = m_ObjectQueue.front();
            m_ObjectQueue.pop();
            delete frontObject;
        }

        for (size_t i = 0; i < Size; ++i)
        {
            m_ObjectQueue.emplace(new T());
        }
    }

};

template<class T>
inline CObjectPool<T>::CObjectPool(size_t initMemorySize)
{
    Count = (int)initMemorySize;

    for (size_t i = 0; i < initMemorySize; ++i) 
    {
        m_ObjectQueue.emplace(new T());
    }
}

template<class T>
inline CObjectPool<T>::~CObjectPool()
{
    size_t Size = m_ObjectQueue.size();

    for (size_t i = 0; i < Size; ++i)
    {
        auto frontObject = m_ObjectQueue.front();
        m_ObjectQueue.pop();
        delete frontObject;
    }
}

template<class T>
inline T* CObjectPool<T>::GetMemory()
{
    if (m_ObjectQueue.empty())
    {
        for(int i = 0 ; i < Count; ++i)
            m_ObjectQueue.emplace(new T());
    }

    T* frontobject = std::move(m_ObjectQueue.front());
    m_ObjectQueue.pop();

    return frontobject;
}

template<class T>
inline void CObjectPool<T>::ReturnMemory(T* Memory)
{
    m_ObjectQueue.push(Memory);
}