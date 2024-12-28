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

struct SERIALIZABLE NonIntrusive_TaggedMemberNotSerialized {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    NonIntrusive_TaggedMemberNotSerialized(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    NonIntrusive_TaggedMemberNotSerialized() = default;
};

template<class Archive>
void serialize(Archive & ar, NonIntrusive_TaggedMemberNotSerialized& obj, const unsigned int version) {
    ar & obj.m_a;
    ar & obj.m_b;
}

struct SERIALIZABLE NonIntrusive_UntaggedMemberSeralized {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    char m_c = 'c';

    NonIntrusive_UntaggedMemberSeralized(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    NonIntrusive_UntaggedMemberSeralized() = default;
};

template<class Archive>
void serialize(Archive & ar, NonIntrusive_UntaggedMemberSeralized& obj, const unsigned int version) {
    ar & obj.m_a;
    ar & obj.m_b;
    ar & obj.m_c;
}

struct SERIALIZABLE NonIntrusive_SerializableWithoutFunction {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    NonIntrusive_SerializableWithoutFunction(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    NonIntrusive_SerializableWithoutFunction() = default;
};

struct SERIALIZABLE NonIntrusive_TwoErrorsAtOnce {
    SERIALIZABLE int m_a = 1;
    float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    NonIntrusive_TwoErrorsAtOnce(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    NonIntrusive_TwoErrorsAtOnce() = default;
};

template<class Archive>
void serialize(Archive & ar, NonIntrusive_TwoErrorsAtOnce& obj, const unsigned int version) {
    ar & obj.m_a;
    ar & obj.m_b;
}