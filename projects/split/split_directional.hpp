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

// This file contains the same class definitions as split_Directional, however in this case it uses the directional serial-
// ization operators, << and >>, instead of &.
// It additionally tests mixed-operations.

// Save and load correctly implemented intrusively
struct SERIALIZABLE Split_AllMembersSerialized_Directional {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_AllMembersSerialized_Directional(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_AllMembersSerialized_Directional() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << m_a;
        ar << m_b;
        ar << m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> m_a;
        ar >> m_b;
        ar >> m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER() // This comment tests vulnerability
};

// Serialization is ok, class does not use macro for discovery
// Single regression test for the case without a macro
struct SERIALIZABLE Split_AllMembersSerialized_Directional_NoMacro {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_AllMembersSerialized_Directional_NoMacro(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_AllMembersSerialized_Directional_NoMacro() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << m_a;
        ar << m_b;
        ar << m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> m_a;
        ar >> m_b;
        ar >> m_c;
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int file_version ){
        boost::serialization::split_member(ar, *this, file_version);
    }
};

// Order of elements in save/load are correct, but a marked member is not serialized
// Single regression test for the case without a macro
struct SERIALIZABLE Split_OneMemberNotSaved_Directional_NoMacro {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_OneMemberNotSaved_Directional_NoMacro(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_OneMemberNotSaved_Directional_NoMacro() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << m_a;
        ar << m_b;
        // ar << m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> m_a;
        ar >> m_b;
        ar >> m_c;
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int file_version ){
        boost::serialization::split_member(ar, *this, file_version);
    }
};

struct SERIALIZABLE Split_OneMemberNotSaved_Directional {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_OneMemberNotSaved_Directional(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_OneMemberNotSaved_Directional() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << m_a;
        ar << m_b;
        // ar & m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> m_a;
        ar >> m_b;
        ar >> m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER() // This comment tests vulnerability
};

struct SERIALIZABLE Split_OneMemberNotLoaded_Directional {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_OneMemberNotLoaded_Directional(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_OneMemberNotLoaded_Directional() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << m_a;
        ar << m_b;
        ar << m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> m_a;
        ar >> m_b;
        // ar & m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

// Order of elements in save/load are correct, but an unmarked member is serialized
// Regression test
struct SERIALIZABLE Split_UnmarkedMemberSavedAndLoaded_Directional {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    char m_c = 'c';

    Split_UnmarkedMemberSavedAndLoaded_Directional(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_UnmarkedMemberSavedAndLoaded_Directional() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << m_a;
        ar << m_b;
        ar << m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> m_a;
        ar >> m_b;
        ar >> m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

// Order of elements in save/load are correct, but a marked member is not serialized
// Regression test
struct SERIALIZABLE Split_MarkedMemberNotSavedOrLoaded_Directional {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_MarkedMemberNotSavedOrLoaded_Directional(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_MarkedMemberNotSavedOrLoaded_Directional() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << m_a;
        ar << m_b;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> m_a;
        ar >> m_b;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

struct SERIALIZABLE Split_WrongOrder_TypeOk_Directional {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE int m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_WrongOrder_TypeOk_Directional(int a, int b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_WrongOrder_TypeOk_Directional() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << m_a;
        ar << m_b;
        ar << m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> m_b;
        ar >> m_a;
        ar >> m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

struct SERIALIZABLE Split_WrongOrder_TypeWrong_Directional {
    SERIALIZABLE int m_a = 1;
    SERIALIZABLE float m_b = 1.;
    SERIALIZABLE char m_c = 'c';

    Split_WrongOrder_TypeWrong_Directional(int a, float b, char c) : m_a{a}, m_b{b}, m_c{c} {}

    Split_WrongOrder_TypeWrong_Directional() = default;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << m_a;
        ar << m_b;
        ar << m_c;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> m_b;
        ar >> m_a;
        ar >> m_c;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};