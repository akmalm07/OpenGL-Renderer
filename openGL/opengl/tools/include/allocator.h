#pragma once


#include "config.h"


namespace tools
{

    struct ReallocOneAndHalf {};
    struct ReallocEnough {};

    //Not debug friendly

    template <typename T>
    concept OverLimitPolicyTag =
        std::is_same_v<T, std::bad_alloc> ||
        std::is_same_v<T, std::nothrow_t> ||
        std::is_same_v<T, ReallocOneAndHalf> ||
        std::is_same_v<T, ReallocEnough>;

    template <OverLimitPolicyTag OverLimitPolicy>
    class MemoryPool {
    public:
        MemoryPool(size_t poolSize)
            : _poolSize(poolSize), _offset(0)
        {
            _pool.resize(poolSize);
        }

        void* allocate_bytes(size_t size, size_t alignment) 
        {
            size_t current = reinterpret_cast<size_t>(_pool.data() + _offset);
            size_t aligned = (current + alignment - 1) & ~(alignment - 1);
            size_t padding = aligned - current;

            if (_offset + padding + size <= _pool.size()) 
            {
                _offset += padding;
                void* result = _pool.data() + _offset;
                _offset += size;
                return result;
            }

            if constexpr (std::is_same_v<OverLimitPolicy, std::bad_alloc>) 
            {
                throw std::bad_alloc();
            }
            else if constexpr (std::is_same_v<OverLimitPolicy, std::nothrow_t>) 
            {
                return nullptr;
            }
            else if constexpr (std::is_same_v<OverLimitPolicy, ReallocOneAndHalf>) 
            {
                _pool.resize(_pool.size() * 3 / 2);
            }
            else if constexpr (std::is_same_v<OverLimitPolicy, ReallocEnough>) 
            {
                _pool.resize(_offset + size);
            }

            return allocate_bytes(size, alignment);
        }

        void deallocate_bytes(void* p, size_t, size_t) noexcept 
        {
            (void)p;
        }

        void reset() noexcept 
        { 
            _offset = 0; 
        }

    private:
        std::vector<uint8_t> _pool;
        size_t _poolSize = 0;
        size_t _offset = 0;
    };

    template <typename T, OverLimitPolicyTag Policy>
    class Allocator {
    public:
        using value_type = T;

        Allocator(MemoryPool<Policy>* p = nullptr) noexcept
            : _pool(p) {}

        template <class U>
        Allocator(const Allocator<U, Policy>& other) noexcept
            : _pool(other._pool) 
        {
        }

        T* allocate(std::size_t n) 
        {
            if (!_pool) {
                return static_cast<T*>(::operator new(n * sizeof(T)));
            }
            return static_cast<T*>(_pool->allocate_bytes(n * sizeof(T), alignof(T)));
        }

        void deallocate(T* p, std::size_t n) noexcept
        {
            if (_pool) {
                _pool->deallocate_bytes(p, n * sizeof(T), alignof(T));
            }
            else {
                ::operator delete(p);
            }
        }

        template<class U>
        bool operator==(const Allocator<U, Policy>& other) const noexcept { return _pool == other._pool; }

        template<class U>
        bool operator!=(const Allocator<U, Policy>& other) const noexcept { return !(*this == other); }
    
    private:
        MemoryPool<Policy>* _pool;
    };

	template <typename T, OverLimitPolicyTag Policy = ReallocOneAndHalf>
    std::vector<T> make_vector_with_pool(size_t initialSize, MemoryPool<Policy>* pool) 
    {
        return std::vector<T, Allocator<T, Policy>>(Allocator<T, Policy>(pool));
	}

	template <typename T, OverLimitPolicyTag Policy = ReallocOneAndHalf, typename ... Args>
    std::shared_ptr<T> make_shared_with_pool(MemoryPool<Policy>* pool, Args&&... args) 
    {
		static_assert(!std::is_constructible_v<T, Args...> || std::is_copy_constructible_v<T>, "Type T must be copy constructible if Args are provided.");
		return std::allocate_shared<T, Allocator<T, Policy>>(Allocator<T, Policy>(pool), std::forward<Args>(args)...);
	}
}