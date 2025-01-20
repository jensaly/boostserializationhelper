#include <split_ampersand.hpp>
#include <gtest/gtest.h>

#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/version.hpp>

#include <test_utils.h>

TEST(Test_Split_NoError_Ampersand, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_AllMembersSerialized_Ampersand obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_AllMembersSerialized_Ampersand obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1');
    }
}

TEST(Test_Split_OneMemberNotSaved_Ampersand, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_OneMemberNotSaved_Ampersand obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_OneMemberNotSaved_Ampersand obj;

        EXPECT_THROW(ar >> obj, boost::archive::archive_exception); // Will read past EOF.
    }
}


TEST(Test_Split_OneMemberNotLoaded_Ampersand, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_OneMemberNotLoaded_Ampersand obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_OneMemberNotLoaded_Ampersand obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, 'c');
    }
}

TEST(Test_Split_UnmarkedMemberSaved_Ampersand, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_UnmarkedMemberSaved_Ampersand obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_UnmarkedMemberSaved_Ampersand obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1'); // m_c is not SERIALIZABLE, therefore it should actually be 'c'.
    }
}

TEST(Test_Split_MarkedMemberNotSaved_Ampersand, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_MarkedMemberNotSavedOrLoaded_Ampersand obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_MarkedMemberNotSavedOrLoaded_Ampersand obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, 'c'); // m_c is SERIALIZABLE, therefore it should actually be '1'. But it's actually 'c'.
    }
}

TEST(Test_Split_WrongOrder_TypeOk_Ampersand, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_WrongOrder_TypeOk_Ampersand obj(14, 17, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_WrongOrder_TypeOk_Ampersand obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 17);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1');
    }
}

TEST(Test_Split_WrongOrder_TypeWrong_Ampersand, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_WrongOrder_TypeWrong_Ampersand obj(14, 17, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_WrongOrder_TypeWrong_Ampersand obj;

        EXPECT_THROW(ar >> obj, boost::archive::archive_exception); // Will throw due to type mismatch
    }
}
