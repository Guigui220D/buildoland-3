#include <assert.h>

template <typename T>
class Arr2D
{
public:
    template <typename... Args>
    Arr2D(uint16_t size_x, uint16_t size_y, Args&&... args);
    ~Arr2D();

    inline uint16_t getSizeX() const { return msizex; };
    inline uint16_t getSizeY() const { return msizey; };

    inline T* getData() const { return values; };
    inline uint32_t getElementCount() const { return msizex * msizey; }
    inline size_t getDataSize() const { return getElementCount() * sizeof(T); }

    void set(uint16_t x, uint16_t y, T value);
    T get(uint16_t x, uint16_t y) const;
    T& at(uint16_t x, uint16_t y);
    const T& constAt(uint16_t x, uint16_t y) const;

private:
    const uint16_t msizex, msizey;
    T* values;
};

template <typename T>
template <typename... Args>
Arr2D<T>::Arr2D(uint16_t size_x, uint16_t size_y, Args&&... args) :
    msizex(size_x),
    msizey(size_y)
{
    values = static_cast<T*>(operator new(getDataSize()));
    for (uint32_t i = 0; i < getElementCount(); i++)
        new (values + i) T(args...);
}

template <typename T>
Arr2D<T>::~Arr2D()
{
    for (uint32_t i = 0; i < getElementCount(); i++)
        (values + i)->~T();
    operator delete(values, getDataSize());
}

template <typename T>
void Arr2D<T>::set(uint16_t x, uint16_t y, T value)
{
    assert(x < msizex);
    assert(y < msizey);
    values[x + y * msizex] = value;
}

template <typename T>
T Arr2D<T>::get(uint16_t x, uint16_t y) const
{
    assert(x < msizex);
    assert(y < msizey);
    return values[x + y * msizex];
}

template <typename T>
T& Arr2D<T>::at(uint16_t x, uint16_t y)
{
    assert(x < msizex);
    assert(y < msizey);
    return values[x + y * msizex];
}

template <typename T>
const T& Arr2D<T>::constAt(uint16_t x, uint16_t y) const
{
    assert(x < msizex);
    assert(y < msizey);
    return values[x + y * msizex];
}