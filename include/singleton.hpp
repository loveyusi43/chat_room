#ifndef __SINGLETON_HPP
#define __SINGLETON_HPP

template <class T>
class Singleton {
public:
    static T& Instence(void) {
        static T instance;
        return instance;
    }

private:
    Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif  // __SINGLETON_HPP