#include <serializable.h>

class Complex {
    int a;
    SERIALIZABLE float b;

    template<class Archive>
    void serialize(Archive & ar) {
        ar & a;
    }
};