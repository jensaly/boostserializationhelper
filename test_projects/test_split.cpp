#include <split_class.hpp>
#include <gtest/gtest.h>

#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/version.hpp>

#include <test_utils.h>

TEST(Test_Split_NoError, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_AllMembersSerialized obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_AllMembersSerialized obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1');
    }
}

TEST(Test_Split_OneMemberNotSaved, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_OneMemberNotSaved obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_OneMemberNotSaved obj;

        EXPECT_THROW(ar >> obj, boost::archive::archive_exception); // Will read past EOF.
    }
}


TEST(Test_Split_OneMemberNotLoaded, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_OneMemberNotLoaded obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_OneMemberNotLoaded obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, 'c');
    }
}

TEST(Test_Split_UnmarkedMemberSaved, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_UnmarkedMemberSaved obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_UnmarkedMemberSaved obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1'); // m_c is not SERIALIZABLE, therefore it should actually be 'c'.
    }
}

TEST(Test_Split_MarkedMemberNotSaved, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_MarkedMemberNotSavedOrLoaded obj(14, 14, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_MarkedMemberNotSavedOrLoaded obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 14);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, 'c'); // m_c is SERIALIZABLE, therefore it should actually be '1'. But it's actually 'c'.
    }
}

TEST(Test_Split_WrongOrder_TypeOk, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_WrongOrder_TypeOk obj(14, 17, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_WrongOrder_TypeOk obj;

        EXPECT_NO_THROW(ar >> obj);

        EXPECT_EQ(obj.m_a, 17);
        EXPECT_EQ(obj.m_b, 14.);
        EXPECT_EQ(obj.m_c, '1');
    }
}

TEST(Test_Split_WrongOrder_TypeWrong, ValueTest) {
    std::string filename;
    {
        filename = generateUniqueFilename();

        std::ofstream ifs(filename);

        boost::archive::text_oarchive ar(ifs);

        Split_WrongOrder_TypeWrong obj(14, 17, '1');

        EXPECT_NO_THROW(ar << obj);
    }
    {

        std::ifstream fs(filename);

        boost::archive::text_iarchive ar(fs);

        Split_WrongOrder_TypeWrong obj;

        EXPECT_THROW(ar >> obj, boost::archive::archive_exception); // Will throw due to type mismatch
    }
}
