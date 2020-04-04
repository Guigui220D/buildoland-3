#ifndef NETWORKREQUESTQUEUE_H
#define NETWORKREQUESTQUEUE_H
#include <type_traits>
#include <typeindex>
#include <cstddef>
#include <queue>
#include <cassert>
#include <new>
#include <experimental/optional>

#if defined __has_include
#  if __has_include (<optional>)
#    define HAS_MAINSTREAM_OPTIONAL
#  endif
#endif


#ifdef HAS_MAINSTREAM_OPTIONAL
#include <optional>
#else
#include <experimental/optional>
namespace std {
template <typename T>
using optional = experimental::optional<T>;
}
#endif

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>

namespace detail
{
template <size_t arg1, size_t... others>
struct static_max;

template <size_t arg>
struct static_max<arg>
{
    static const size_t value = arg;
};

template <size_t arg1, size_t arg2, size_t... others>
struct static_max<arg1, arg2, others...>
{
    static const size_t value = arg1 >= arg2 ? static_max<arg1, others...>::value : static_max<arg2, others...>::value;
};


template <typename...>
struct is_one_of {
    static constexpr bool value = false;
};

template <typename F, typename S, typename... T>
struct is_one_of<F, S, T...> {
    static constexpr bool value =
        std::is_same<F, S>::value || is_one_of<F, T...>::value;
};
}

template <typename... RequestTypes>
class NetworkRequestQueue
{
private:
    static constexpr std::size_t data_size = detail::static_max<sizeof(RequestTypes)...>::value;
    static constexpr std::size_t data_align = detail::static_max<alignof(RequestTypes)...>::value;

    using data_type = typename std::aligned_storage<data_size, data_align>::type;
    struct queue_entry
    {
        std::type_index type_id;
        data_type data;
    };

    std::queue<queue_entry> request_queue;
    mutable sf::Mutex mutex;

public:
    template <typename T>
    void pushRequest(T&& request)
    {
        sf::Lock lock(mutex);

        static_assert (detail::is_one_of<T, RequestTypes...>::value, "invalid request type");
        request_queue.emplace(queue_entry{typeid (T), data_type{}});
        new (&request_queue.back().data) T(std::move(request));
    }

    template <typename T>
    std::optional<T> tryPop()
    {
        sf::Lock lock(mutex);

        static_assert (detail::is_one_of<T, RequestTypes...>::value, "invalid request type");
        assert(!empty());

        queue_entry entry = std::move(request_queue.front());
        if (entry.type_id != typeid(T))
            return {};
        request_queue.pop();

        return std::move(*reinterpret_cast<T*>(&entry.data));
    }

    void skip()
    {
        sf::Lock lock(mutex);
        assert(!empty());

        request_queue.pop();
    }

    bool empty() const
    {
        sf::Lock lock(mutex);

        return request_queue.empty();
    }
};

#endif // NETWORKREQUESTQUEUE_H
