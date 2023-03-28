#ifndef DEFAULTDICT_HPP_INCLUDED
#define DEFAULTDICT_HPP_INCLUDED

#include <unordered_map>

template <typename K, typename V>
class DefaultDict : public std::unordered_map<K, V> {
   private:
    V defaultValue;

   public:
    DefaultDict(V defaultValue) : defaultValue{defaultValue} {}

    V &operator[](const K &key) {
        if (!this->count(key)) {
            this->insert({key, defaultValue});
        }
        return std::unordered_map<K, V>::operator[](key);
    }
    V operator[](K &&key) {
        if (!this->count(key)) {
            this->insert({key, defaultValue});
        }
        return std::unordered_map<K, V>::operator[](key);
    }

    const V &at(const K &key) const {
        if (!this->count(key)) {
            return defaultValue;
        }
        return std::unordered_map<K, V>::at(key);
    }
    V &at(const K &key) {
        if (!this->count(key)) {
            return defaultValue;
        }
        return std::unordered_map<K, V>::at(key);
    }
};

#endif