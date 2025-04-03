#pragma once
#include "config.h"

#include <vector>
#include <unordered_map>

namespace tools
{
    template <typename T>
    class MemoryPool
    {
    public:
        MemoryPool(size_t size);

        template <typename U, typename... Args>
            requires std::is_base_of_v<T, U>
        U* allocate(Args&&... args);

        void deallocate(void* ptr);

        template <typename U>
        U* get(size_t index);

        void reset();

        ~MemoryPool();

    private:
        std::vector<uint8_t> _pool;               
        size_t _offset;                           
        std::unordered_map<void*, void(*)(void*)> _destructors; 
    };  

}




namespace tools
{
    template<typename T>
    MemoryPool<T>::MemoryPool(size_t size) 
        : _pool(size), _offset(0)
    {}

    template<typename T>
    inline void MemoryPool<T>::deallocate(void* ptr)
    {
        
        auto it = _destructors.find(ptr);
        if (it == _destructors.end())
        {
            throw std::runtime_error("Pointer not allocated by this pool!");
        }

        it->second(ptr);
        _destructors.erase(it);
        
    }

    template<typename T>
    template<typename U, typename ...Args>
		requires std::is_base_of_v<T, U>
    inline U* MemoryPool<T>::allocate(Args && ...args)
    {
        size_t alignment = alignof(U);
        _offset = (_offset + alignment - 1) & ~(alignment - 1);

        if (_offset + sizeof(U) > _pool.size())
        {
            throw std::bad_alloc();
        }

        void* ptr = _pool.data() + _offset;
        _offset += sizeof(U);

        U* obj = new (ptr) U(std::forward<Args>(args)...);
        _destructors[ptr] = [](void* objPtr)
            {
                static_cast<U*>(objPtr)->~U();
            };

        return obj;
    }

    template<typename T>
    template<typename U>
    inline U* MemoryPool<T>::get(size_t index)
    {
        return reinterpret_cast<U*>(_pool.data() + index);
    }

    template<typename T>
    void MemoryPool<T>::reset()
    {
        for (auto& [ptr, destructor] : _destructors)
        {
            destructor(ptr);
        }
        _destructors.clear();
        _offset = 0;
    }

    template<typename T>
    MemoryPool<T>::~MemoryPool()
    {
        for (auto& [ptr, destructor] : _destructors)
        {
            destructor(ptr);
        }
        _destructors.clear();
        _offset = 0;
    }

}