#ifndef yeti_cache_h
#define yeti_cache_h

#include "class.h"
#include "yetiobject.h"

#include "cache.hpp"
#include "data.hpp"
#include "tensor.hpp"
#include "thread.hpp"
#include "mallocimpl.h"
#include "tuple.h"

#ifdef redefine_size_t
#define size_t custom_size_t
#endif

#define MALLOC_CACHE_BLOCK 0

#define DEBUG_CACHE_USAGE 0

namespace yeti {

class Cachable :
    public YetiRuntimeCountable
{
    protected:
        bool in_destructor_;

    public:
        Cachable();

        Cachable(YetiRuntimeObject::thread_safety_flag_t flag);

        virtual void flush_from_cache() = 0;

        bool in_destructor() const;


};

/**
    @class DataCacheEntry
    A single entry in a data cache which
    owns a particular block of memory of given size.
*/
struct DataCacheEntry :
    public Malloc<DataCacheEntry>,
    public YetiRuntimeCountable
{

    public:
        uli offset;

        char* data;

        /** This holds a pointer to the data location */
        Cachable* owner;

        bool pulled;

    public:
        /**
            Create a data cache entry pointing at the given memory location
            @param d The memory location
        */
        DataCacheEntry(char* d, uli offset);

        ~DataCacheEntry();

        Cachable* get_owner() const;

        /**
            Assign this data cache entry to a new data block owner
            @param owner
        */
        void assign(Cachable* new_owner);

        bool is_pulled() const;

        void flush();

        void print(std::ostream& os) const;


};

#define NCACHE_ENTRIES_MAX 1000

/**
    @class DataCache
    A cache holding a set of memory blocks of uniform size.  This should not allocate
    its own memory, but receive the allocation from a LayeredDataCache.
*/
class DataCache :
    public YetiRuntimeCountable,
    public Malloc<DataCache>
{

    protected:
        char* data_;

        size_t storage_;

        DataCacheEntry* entries_[NCACHE_ENTRIES_MAX];

        DataCacheEntry* all_entries_[NCACHE_ENTRIES_MAX];

        uli blocksize_;

        uli nentries_;
        
        uli* offsets_;

        uli* starts_;

        uli* stops_;

        /**
            Start looking for an open cache entry at the offset
        */
        DataCacheEntry* find_entry(uli offset, uli stop, bool check_occupied);

        void process_entry(DataCacheEntry* entry, CachedStorageBlock* block, uli threadnum);

        DataCacheEntry* pull_unused(uli threadnum);

        DataCacheEntry* pull_any(uli threadnum);


    public:
        /**
            @param Total size in bytes of available cache storage
            @param size_counts Key data size, value number of tiles with that size.
                               The data sizes will be used to determine which data
                               cache sizes are needed
        */
        DataCache(
            size_t storage,
            size_t blocksize
        );

        DataCache(size_t blocksize);

        ~DataCache();

        /**
            This method is not thread-safe and assumes
            the cache entry has been locked.
            @param entry
        */
        void insert(DataCacheEntry* entry);

        /**
            This method is not thread-safe and assumes
            the cache entry has been locked.
            @param entry
        */
        void pull(DataCacheEntry* entry);

        void append(
            char* data,
            size_t blocksize
        );

        /**
            Pull a new data cache entry and assign the given data block
            as an owner. The cache block cannot be cleared while it is
            pulled and there assumes the block has been locked before being called.
            @param block The block to assign as owner to the new cache entry
            @return A new data cache entry
        */
        DataCacheEntry* pull(CachedStorageBlock* block);

        void print_details(std::ostream& os = std::cout);

        void flush();

        /**
            @return The number of available blocks in cache
        */
        uli ncache() const;

        /**
            @return The number of blocks with no linked data
        */
        uli nfree() const;

        /**
            @return The total number of blocks in the cache
        */
        uli ntotal() const;

        /**
            All cache blocks are of uniform size.
            @return The size of each block in the cache
        */
        uli blocksize() const;

        void print(std::ostream& os = std::cout) const;

};


}

#ifdef redefine_size_t
#undef size_t
#endif

#endif
