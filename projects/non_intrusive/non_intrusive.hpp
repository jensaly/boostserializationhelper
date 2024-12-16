#include <serializable.h>

struct SERIALIZABLE NonIntrusive_NoError {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    NonIntrusive_NoError(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    NonIntrusive_NoError() = default;
};

template<class Archive>
void serialize(Archive & ar, NonIntrusive_NoError& obj, const unsigned int version) {
    ar & obj.m_a;
    ar & obj.m_b;
    ar & obj.m_c;
}