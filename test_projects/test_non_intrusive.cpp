#include <non_intrusive.hpp>
#include <gtest/gtest.h>

#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/version.hpp>

#include <test_utils.h>

TEST(Test_NonIntrusive_NoError, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        NonIntrusive_NoError obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        NonIntrusive_NoError obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1');
    }
}

TEST(Test_NonIntrusive_TaggedMemberNotSerialized, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        NonIntrusive_TaggedMemberNotSerialized obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        NonIntrusive_TaggedMemberNotSerialized obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, 'c'); // Loaded from the default constructor
    }
}

TEST(Test_NonIntrusive_UntaggedMemberSeralized, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        NonIntrusive_UntaggedMemberSeralized obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        NonIntrusive_UntaggedMemberSeralized obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1'); // Loaded from the default constructor
    }
}

TEST(Test_NonIntrusive_TwoErrorsAtOnce, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        NonIntrusive_TwoErrorsAtOnce obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        NonIntrusive_TwoErrorsAtOnce obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, 'c'); // Loaded from the default constructor
    }
}