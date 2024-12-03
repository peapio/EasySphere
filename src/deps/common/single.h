#pragma once

namespace common {

template <typename T>
class Singleton {
public:
    Singleton() = default;
    virtual ~Singleton() = default;
    static T& getInstance()
    {
        static T instance;
        return instance;
    }

};

}
