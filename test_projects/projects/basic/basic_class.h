#include <boost_serialization_helper.h>

struct Basic_AllMembersSerialized {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Basic_AllMembersSerialized(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Basic_AllMembersSerialized() = default;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }
};

struct Basic_OneMemberNotSerialized {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    char m_c = 'c';

    Basic_OneMemberNotSerialized(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Basic_OneMemberNotSerialized() = default;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & m_a;
        ar & m_b;
        // ar & m_c;
    }
};

struct Basic_TaggedMemberNotSerialized {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    char m_c = 'c';

    Basic_TaggedMemberNotSerialized(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Basic_TaggedMemberNotSerialized() = default;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & m_a;
    }
};

struct Basic_UntaggedMemberSeralized {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    char m_c = 'c';

    Basic_UntaggedMemberSeralized(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Basic_UntaggedMemberSeralized() = default;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }
};