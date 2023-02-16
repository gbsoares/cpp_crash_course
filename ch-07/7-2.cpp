#include <cstddef>
#include <cstdio>
#include <new>

struct LargeBucket
{
    const static size_t data_size{ 1024 * 1024 };
    std::byte data[data_size];  
};

struct Bucket
{
    const static size_t data_size{ 4096 };
    std::byte data[data_size];
};

struct Heap
{
    void* allocate(size_t bytes) 
    {
        if(bytes > LargeBucket::data_size)
        {
            throw std::bad_alloc();
        }
        else
        {
            if(bytes > Bucket::data_size)
            {
                /* allocate a single 1MB Large bucket */
                is_large_bucket_alloced = true;
                return large_bucket.data;
            }
        }

        /* can return a small bucket */
        for(size_t i{}; i < n_heap_buckets; i++)
        {
            if(!bucket_used[i])
            {
                bucket_used[i] = true;
                return buckets[i].data;
            }
        }
        throw std::bad_alloc();
    }

    void free(void* p)
    {
        if(p == large_bucket.data)
        {
            is_large_bucket_alloced = false;
            return;    
        }

        for(size_t i{}; i < n_heap_buckets; i++)
        {
            if(buckets[i].data == p)
            {
                bucket_used[i] = false;
                return;
            }
        }
    }

    static const size_t n_heap_buckets{ 10 };
    Bucket buckets[n_heap_buckets]{};
    bool bucket_used[n_heap_buckets]{};

    LargeBucket large_bucket{};
    bool is_large_bucket_alloced{};
};

Heap heap;

void* operator new(size_t n_bytes)
{
    return heap.allocate(n_bytes);
}

void operator delete(void* p)
{
    return heap.free(p);
}

int main()
{
    printf("Buckets:   %p\n", heap.buckets);
    auto breakfast = new unsigned int{ 0xC0FFEE };
    auto dinner = new unsigned int{ 0xDEADBEEF };
    printf("Breakfast: %p 0x%x\n", breakfast, *breakfast);
    printf("Dinner:    %p 0x%x\n", dinner, *dinner);
    delete breakfast;
    delete dinner;

    auto test = new LargeBucket;
    printf("LG Bucket: %p\n", heap.large_bucket.data);
    printf("Alloc'ed : %p\n", test);
    delete test;
}