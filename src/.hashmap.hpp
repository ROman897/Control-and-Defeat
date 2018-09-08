namespace generals {

template <typename T, typename HashFunc, uint32_t BASE_SIZE, uint32_t OVERFLOW_SIZE>
class Hashmap {
    struct HashBucket {
        T m_value;
        bool m_occupied;
        HashBucket* m_next;
    };

    struct Handle {
        uint64_t m_counter;
        HashBucket* m_bucket;
    };

    struct Iterator {

    };

    HashBucket m_data[BASE_SIZE + OVERFLOW_SIZE];
    uint32_t m_total_count;
    uint32_t m_overflow_count;
    HashFunc m_hash_func;

    Handle insert(const T& value) {
        uint32_t index = m_hash_func(key) % BASE_SIZE;
        HashBucket* last_bucket = m_data[index]; 
        HashBucket* new_bucket = last_bucket;
        if (last_bucket->m_occupied) {
            while (last_bucket->m_next != nullptr) {
                last_bucket = last_bucket->m_next;
            }
            assert(m_overflow_count < OVERFLOW_SIZE);
            HashBucket* new_bucket = &m_data[0] + m_overflow_count;
            last_bucket->m_next = new_bucket;
            ++m_overflow_count;
        }
        new_bucket->m_occupied = true;
        new_bucket->m_key_val = std::make_pair(key, value);
        ++m_total_count;
        return Handle(*this, new_bucket);
    }

    Handle find(const Key& key) {
        uint32_t index = m_hash_func(key) % BASE_SIZE;
        HashBucket* _bucket = m_data[index];
        if (!bucket->m_occupied) {
            return Handle(*this, nullptr);
        }
        for (;;) {
            if (bucket->m_key_val.first == key) {
                return Handle(*this, bucket);
            }
            if (bucket->m_key_val->m_next == nullptr) {
                return handle(*this, nullptr);
            }
            bucket = bucket->m_next;
        }
    }

};
}

