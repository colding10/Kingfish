#ifndef GENERATOR_h_INCLUDED
#define GENERATOR_h_INCLUDED

#include <coroutine>

template <class T>
struct Generator {
    struct promise_type;
    using coro_handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        T    current_value;
        auto get_return_object() {
            return Generator{coro_handle::from_promise(*this)};
        }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() { std::terminate(); }
        auto yield_value(T value) {
            current_value = value;
            return std::suspend_always{};
        }
    };

    bool next() { return coro ? (coro.resume(), !coro.done()) : false; }
    T    value() { return coro.promise().current_value; }

    Generator(Generator const &rhs) = delete;
    Generator(Generator &&rhs)
        : coro(rhs.coro) {
        rhs.coro = nullptr;
    }
    ~Generator() {
        if (coro)
            coro.destroy();
    }

  private:
    Generator(coro_handle h)
        : coro(h) {}
    coro_handle coro;
};

#endif
