#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include <functional>
#include <vector>

template <typename T>
class Generator {
   public:
    class Iterator {
       public:
        Iterator(Generator& gen, bool is_end = false) : gen_(gen) {
            if (!is_end) {
                value_ = gen_();
            }
        }
        Iterator& operator++() {
            value_ = gen_();
            return *this;
        }
        bool operator!=(const Iterator& other) const {
            return !gen_.is_done_ || !other.gen_.is_done_;
        }
        const T& operator*() const { return value_; }

       private:
        Generator& gen_;
        T value_;
    };

    Generator(std::function<T()> fn) : fn_(fn), is_done_(false) {}
    Iterator begin() { return Iterator(*this); }
    Iterator end() { return Iterator(*this, true); }
    T operator()() {
        if (!is_done_) {
            T value = fn_();
            is_done_ = value == T{};
            return value;
        }
        return T{};
    }

   private:
    std::function<T()> fn_;
    bool is_done_;
};

#endif  //! GENERATOR_HPP_INCLUDED