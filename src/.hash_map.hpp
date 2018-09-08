#include <memory>
#include <cassert>
namespace generals {

template <typename Key, typename Value, typename HashFunc, size_t BASE_SIZE, size_t OVERFLOW_SIZE>
class HashMap {

public:
    using ValueType = std::pair<Key, Value>;
    using Reference = ValueType&;
    using ConstReference = const ValueType&;
    using Pointer = ValueType*;
    using ConstPointer = const ValueType*;
    using SizeType = size_t;
    using DifferenceType = std::ptrdiff_t;

    // represents one bucket of hash map
    class HashBucket {
    public:
        class LocalIterator {
        private:
            HashBucket* m_base_bucket;
            HashBucket* m_current_bucket;
        public:
            LocalIterator(HashBucket* base_bucket, HashBucket* current_bucket)
                : m_base_bucket(base_bucket), m_current_bucket(current_bucket)
            {}

            LocalIterator& operator++(int) {
                auto me = *this;
                this->operator++();
                return me;
            }

            LocalIterator& operator++() {
                m_current_bucket = m_current_bucket->m_overflow_bucket.get();
            }

            Reference operator*() {
                assert(m_current_bucket->m_initialized);
                return m_current_bucket->m_key_val;
            }

            Pointer operator->() {
                assert(m_current_bucket->m_initialized);
                return &m_current_bucket.m_key_val;
            }

            bool operator==(const LocalIterator& other) {
                return m_base_bucket == other.m_base_bucket && m_current_bucket == other.m_current_bucket;
            }

            bool operator!=(const LocalIterator& other) {
                return !(*this != other);
            }
        };

        class ConstLocalIterator {
        private:
            const HashBucket* m_base_bucket;
            const HashBucket* m_current_bucket;
        public:
            ConstLocalIterator(const HashBucket* base_bucket, const HashBucket* current_bucket)
                : m_base_bucket(base_bucket), m_current_bucket(current_bucket)
            {}

            LocalIterator& operator++(int) {
                auto me = *this;
                this->operator++();
                return me;
            }

            LocalIterator& operator++() {
                m_current_bucket = m_current_bucket->m_overflow_bucket.get();
            }

            ConstReference operator*() {
                assert(m_current_bucket->m_initialized);
                return m_current_bucket->m_key_val;
            }

            ConstPointer operator->() {
                assert(m_current_bucket->m_initialized);
                return &m_current_bucket.m_key_val;
            }

            bool operator==(const ConstLocalIterator& other) {
                return m_base_bucket == other.m_base_bucket && m_current_bucket == other.m_current_bucket;
            }

            bool operator!=(const ConstLocalIterator& other) {
                return !(*this != other);
            }
        };

        HashBucket(Key key, Value value): m_key_val(std::make_pair(key, value)), m_initialized(true) {
        }
        HashBucket() = default;

        LocalIterator begin() {
            return LocalIterator(*this, *this);
        }

        LocalIterator end() {
            return LocalIterator(*this, nullptr);
        }

        ConstLocalIterator begin() const {
            return ConstLocalIterator(*this, *this);
        }

        ConstLocalIterator end() const {
            return ConstLocalIterator(*this, nullptr);
        }

        ConstLocalIterator cbegin() const {
            return ConstLocalIterator(*this, *this);
        } 

        ConstLocalIterator cend() const {
            return ConstLocalIterator(*this, nullptr);
        }

    private:
        ValueType m_key_val;
        bool m_initialized = false;
        HashBucket* m_overflow_bucket;
    };

    HashMap(const HashFunc& hash_func = HashFunc()) : m_hasher(hash_func) {
    }

    HashMap(const HashMap& other) {
        for (const auto& item : other) {
            insert(item);
        }
    }
    
    HashMap(HashMap&& other) : m_data(other.data), m_size(other.size), m_capacity(other.capacity) {
        other.data = nullptr;
        other.capacity = 0;
    }

    HashMap& operator=(HashMap other) {
        swap(other);
    }

    HashMap operator=(HashMap&& other) {
        swap(other);
    }

    ~HashMap() {
        delete m_data;
    }

    class Iterator {
    private:
        HashBucket* m_base_bucket;
        HashBucket* m_current_bucket;

    public:
        Iterator(HashBucket* base_bucket, HashBucket* current_bucket)
            : m_base_bucket(base_bucket), m_current_bucket(current_bucket) {}

        Iterator& operator++(int) {
            auto me = *this;
            this->operator++();
            return me;
        }

        Iterator& operator++() {
            m_current_bucket = m_current_bucket->m_overflow_bucket.get();
            increment_until_valid();
            for (;;) {
                if (m_current_bucket->m_overflow_bucket) {
                    m_current_bucket = m_current_bucket->m_overflow_bucket;
                } else {
                    m_base_bucket++;
                    m_current_bucket = m_base_bucket;
                }
                if (m_current_bucket->m_initialized) {
                    break;
                }
            }
            return *this;
        }

        Reference operator*() {
            assert(m_current_bucket->m_initialized);
            return m_current_bucket->m_value;
        }

        Pointer operator->() {
            assert(m_current_bucket->m_initialized);
            return &m_current_bucket->m_value;
        }

        bool operator==(const Iterator& other) {
            return m_base_bucket == other.m_base_bucket && m_current_bucket == other.m_current_bucket;
        }

        bool operator!=(const Iterator& other) {
            return !(*this == other);
        }
    private:
        void increment_until_valid() {
            while (m_current_bucket != nullptr && !m_current_bucket->m_initialized) {
                m_current_bucket = m_current_bucket->m_overflow_bucket.get();
            }
        }
    };

    class ConstIterator {
    private:
        const HashBucket* m_base_bucket;
        const HashBucket* m_current_bucket;
    public:

        ConstIterator(const HashBucket* base_bucket, const HashBucket* current_bucket)
            : m_base_bucket(base_bucket), m_current_bucket(current_bucket) {
            }

        ConstIterator& operator++(int) {
            auto me = *this;
            this->operator++();
            return me;
        }

        ConstIterator& operator++() {
            for (;;) {
                if (m_current_bucket->m_overflow_bucket) {
                    m_current_bucket = m_current_bucket->m_overflow_bucket;
                } else {
                    m_base_bucket++;
                    m_current_bucket = m_base_bucket;
                }
                if (m_current_bucket->m_initialized) {
                    break;
                }
            }
            return *this;
        }

        ConstReference operator*() {
            assert(m_current_bucket->m_initialized);
            return m_current_bucket->m_value;
        }

        ConstPointer operator->() {
            assert(m_current_bucket->m_initialized);
            return &m_current_bucket->m_value;
        }

        bool operator==(const ConstIterator& other) {
            return m_base_bucket == other.m_base_bucket && m_current_bucket == other.m_current_bucket;
        }

        bool operator!=(const ConstIterator& other) {
            return !(*this == other);
        }
    };


    // Iterator to loop through buckets
    class BucketIterator {
    private:
        HashBucket m_ptr;
    public:
        BucketIterator& operator++(int) {
            auto me = *this;
            this->operator++();
            return me;
        } 

        BucketIterator& operator++() {
            ++m_ptr;
        }

        HashBucket& operator*() {
            return *m_ptr;
        }

        HashBucket* operator->() {
            return m_ptr;
        }

        bool operator==(const BucketIterator& other) {
            return m_ptr == other.m_ptr;
        }

        bool operator !=(const BucketIterator& other) {
            return !(*this == other);
        }
    };

    // Iterator to loop through buckets
    class ConstBucketIterator {
    private:
        const HashBucket m_ptr;
    public:
        ConstBucketIterator& operator++(int) {
            auto me = *this;
            this->operator++();
            return me;
        } 

        ConstBucketIterator& operator++() {
            ++m_ptr;
        }

        const HashBucket& operator*() {
            return *m_ptr;
        }

        const HashBucket* operator->() {
            return m_ptr;
        }

        bool operator==(const ConstBucketIterator& other) {
            return m_ptr == other.m_ptr;
        }

        bool operator !=(const ConstBucketIterator& other) {
            return !(*this == other);
        }
    };

private:
    HashBucket[BASE_SIZE + OVERFLOW_SIZE] m_data;
    // size including overflow items
    size_t m_overflow_count;
    size_t m_total_count;

    HashFunc m_hasher;

    void _insert(std::unique_ptr<HashBucket> new_bucket) {
        auto bucket_id = m_hasher(new_bucket->m_key_val.first) % m_capacity;
        HashBucket* bucket = m_data + bucket_id;
        if (!bucket->m_initialized) {
            *bucket = std::move(new_bucket);
            ++m_size;
            return;
        }

        for (;;) {
            if (bucket->m_key_val.first == new_bucket->m_key_val.first) {
                bucket->m_key_val = std::move(new_bucket->m_key_val);
                break;
            }
            if (!bucket->m_overflow_bucket) {
                bucket->m_overflow_bucket.reset(new_bucket.release());
                ++m_size;
            }
        }
        try_rehash();
    }

    std::pair<HashBucket*, HashBucket*> _find(const Key& key) const {
        // TODO(Roman): wtf is this
        size_t index = m_hasher(key) % m_capacity;
        HashBucket* bucket = m_data + index;
        HashBucket* original_bucket = bucket;
        for (;;) {
            if (bucket->m_initialized && bucket->m_key_val.first == key) {
                return bucket;
            }
            if (!bucket->m_overflow_bucket) {
                return nullptr;
            }
            bucket = bucket->m_overflow_bucket.get();
        }
        return std::make_pair(original_bucket, bucket);
    }

public:

    template <typename... Args>
    void emplace(Args&&... args) {
        auto new_bucket = std::make_unique<HashBucket>(std::forward<Args>(args)...);
        _insert(new_bucket);
    }

    void insert(const Key& key, const Value& value) {
        auto new_bucket = std::make_unique<HashBucket>(key, value);
        _insert(new_bucket);
    }

    void remove(const Key& key) {
        auto bucket_id = m_hasher(key);
        HashBucket* bucket = (m_data + bucket_id);
        if (!bucket->m_initialized) {
            return;
        }
        HashBucket* prev = nullptr;

        for (;;) {
            if (bucket->m_key_val.first == key) {
                if (prev == nullptr) {
                    if (bucket->m_overflow_bucket) {
                        bucket = std::move(*bucket->m_overflow_bucket);
                    }
                } else {
                   HashBucket* next = bucket->m_overflow_bucket.release(); 
                   prev->m_overflow_bucket.reset(next);
                }
            }
            if (!bucket.overflow_bucket()) {
                break;
            }
            prev = bucket;
            bucket = bucket->m_overflow_bucket.get();
        }
    }

    Iterator find(const Key& key) {
        auto buckets = _find(key);
        return Iterator(const_cast<HashBucket*>(buckets.first),
            const_cast<HashBucket*>(buckets.second));
    }

    ConstIterator find(const Key& key) const {
        auto buckets = _find(key);
        return Iterator(buckets.first, buckets.second);
    }

    BucketIterator find_bucket(const Key& key) {
        size_t index = m_hasher(key) % m_capacity;
        return BucketIterator(m_data + index);
    }

    ConstBucketIterator find_bucket(const Key& key) const {
        size_t index = m_hasher(key) % m_capacity;
        return ConstBucketIterator(m_data + index);
    }

    auto size() {
        return m_size;
    }

    bool empty() {
        return size() == 0;
    }

    size_t capacity() {
        return m_capacity;
    }

    Iterator begin() {
        return Iterator(m_data);
    }

    ConstIterator begin() const {
        return ConstIterator(m_data);
    }

    ConstIterator cbegin() const {
        return ConstIterator(m_data);
    }

    Iterator end() {
        return Iterator(m_data + m_capacity);
    }

    ConstIterator end() const {
        return ConstIterator(m_data + capacity);
    }

    ConstIterator cend() const {
        return ConstIterator(m_data + capacity);
    }


    void clear() {
        delete(m_data);
        m_size = 0;
        m_capacity = 0;
        reserve(10);
    }
};

}
