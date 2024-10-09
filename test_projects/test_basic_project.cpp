#include <basic_class.hpp>
#include <gtest/gtest.h>

#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/version.hpp>

#include <test_utils.h>

TEST(Test_Basic_AllMembersSerialized, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Basic_AllMembersSerialized obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Basic_AllMembersSerialized obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1');
    }
}

TEST(Test_Basic_OneMemberNotSerialized, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Basic_OneMemberNotSerialized obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Basic_OneMemberNotSerialized obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, 'c');
    }
}

TEST(Test_Basic_TaggedMemberNotSerialized, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Basic_TaggedMemberNotSerialized obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Basic_TaggedMemberNotSerialized obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 1.);
        EXPECT_EQ(obj.m_c, 'c');
    }
}

TEST(Test_Basic_UntaggedMemberSeralized, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Basic_UntaggedMemberSeralized obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Basic_UntaggedMemberSeralized obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1'); // If the tags were corrct, this should be 'c
    }
}