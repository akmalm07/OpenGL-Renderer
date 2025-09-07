#pragma once

#include "config.h"

#include <vector>
#include "tools\include\memory_pool.h"


namespace tools
{
    template <typename T>
    class MemoryPoolContainer
    {
    public:
        explicit MemoryPoolContainer(MemoryPool<T>& pool);

        template <typename U, typename... Args>
        T* emplace_back(Args&&... args); 

        template <typename U>
        T* emplace_back(U&& child); 

        T* at(size_t index);

        size_t size() const;

		T& operator[](size_t index);

        void clear();

        auto begin();
        auto end();
        auto begin() const;
        auto end() const;

        ~MemoryPoolContainer();

    private:
        MemoryPool<T>& _pool;    
        std::vector<T*> _objects;
    };


}




namespace tools
{
    template <class T>
    MemoryPoolContainer<T>::MemoryPoolContainer(MemoryPool<T>& pool)
        : _pool(pool) {
    }

    template <class T>
    template <typename U, typename... Args>
    T* MemoryPoolContainer<T>::emplace_back(Args&&... args)
    {
        T* obj = _pool.allocate<U>(std::forward<Args>(args)...); 
        _objects.push_back(obj);
        return obj;
    }

    template<typename T>
    template<typename U>
    inline T* MemoryPoolContainer<T>::emplace_back(U&& child)
    {
        T* obj = _pool.allocate<U>(child);
        _objects.push_back(obj); 
		return obj;
    }

    template <class T>
    T* MemoryPoolContainer<T>::at(size_t index)
    {
        return _objects.at(index);
    }

    template <class T>
    size_t MemoryPoolContainer<T>::size() const
    {
        return _objects.size();
    }

    template<typename T>
    inline T& MemoryPoolContainer<T>::operator[](size_t index)
    {
        if (index >= _objects.size())
        {
            throw std::out_of_range("Index out of range");
        }
        return *_objects[index];
    }

    template <class T>
    void MemoryPoolContainer<T>::clear()
    {
        for (auto* obj : _objects)
        {
            _pool.deallocate(obj);
        }
        _objects.clear();
    }

    template<typename T>
    inline auto MemoryPoolContainer<T>::begin()
    {
        return _objects.begin();
    }

    template<typename T>
    inline auto MemoryPoolContainer<T>::end()
    {
        return _objects.end();
    }

    template<typename T>
    inline auto MemoryPoolContainer<T>::begin() const
    {
        return _objects.begin();
    }

    template<typename T>
    inline auto MemoryPoolContainer<T>::end() const
    {
        return _objects.end();
    }

    template <class T>
    MemoryPoolContainer<T>::~MemoryPoolContainer()
    {
        clear();
    }
}

