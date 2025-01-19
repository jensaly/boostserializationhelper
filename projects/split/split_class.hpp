#include <serializable.h>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

// Split intrusive serialization, must abide by the following:
// If an unmarked member is in EITHER of the serialize-functions, it shall flag as Error_UnmarkedFieldSerialized
// If a marked member is missing in EITHER of the serialize-functions, it shall flag as Error_MarkedFieldNotSerialized
// If neither of the above are true, it shall then verify that the order of the members in each serialize-function is
// the same. It shall not if any of the above are true, as it would potentially clutter the output.
// NOTE: Error_SerializeMethodNotFound does not apply because BOOST_SERIALIZATION_SPLIT_MEMBER will catch any missing
// save or load methods during compile time.

// Save and load correctly implemented intrusively
struct SERIALIZABLE Split_AllMembersSerialized {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_AllMembersSerialized(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_AllMembersSerialized() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};


struct SERIALIZABLE Split_OneMemberNotSaved {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_OneMemberNotSaved(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_OneMemberNotSaved() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & m_a;
        ar & m_b;
        // ar & m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

struct SERIALIZABLE Split_OneMemberNotLoaded {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_OneMemberNotLoaded(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_OneMemberNotLoaded() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & m_a;
        ar & m_b;
        // ar & m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

// Order of elements in save/load are correct, but an unmarked member is serialized
// Regression test
struct SERIALIZABLE Split_UnmarkedMemberSaved {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    char m_c = 'c';

    Split_UnmarkedMemberSaved(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_UnmarkedMemberSaved() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

// Order of elements in save/load are correct, but a marked member is not serialized
// Regression test
struct SERIALIZABLE Split_MarkedMemberNotSavedOrLoaded {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_MarkedMemberNotSavedOrLoaded(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_MarkedMemberNotSavedOrLoaded() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & m_a;
        ar & m_b;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & m_a;
        ar & m_b;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

struct SERIALIZABLE Split_WrongOrder_TypeOk {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE int m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_WrongOrder_TypeOk(int a, int b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_WrongOrder_TypeOk() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & m_b;
        ar & m_a;
        ar & m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

struct SERIALIZABLE Split_WrongOrder_TypeWrong {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_WrongOrder_TypeWrong(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_WrongOrder_TypeWrong() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & m_a;
        ar & m_b;
        ar & m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & m_b;
        ar & m_a;
        ar & m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};