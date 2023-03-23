#include <unordered_map>

template <typename K, typename V>
class DefaultDict {
   public:
    // Constructor takes a default value for the map
    defaultdict(const V& default_value) : default_value_(default_value) {}

    // Access operator returns a reference to the value associated with key
    // If key is not present in the map, a new element is inserted with value default_value_
    V& operator[](const K& key) {
        return data_[key];
    }

    // Const access operator returns a const reference to the value associated with key
    // If key is not present in the map, a new element is inserted with value default_value_
    inline const V& operator[](const K& key) const {
        auto it = data_.find(key);
        if (it == data_.end()) {
            return default_value_;
        } else {
            return it->second;
        }
    }

   private:
    std::unordered_map<K, V> data_;
    V default_value_;
};