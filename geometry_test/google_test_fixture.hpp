//  Copyright 2016 Brandon Kohn. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef TEST_google_test_fixture_HPP
#define TEST_google_test_fixture_HPP
#pragma once

#if !defined(BOOST_TEST_DYN_LINK)
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif
#include <boost/algorithm/string.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>

template <typename String>
inline void output_debug_string(const String& s)
{
#ifdef WINDOWS
    OutputDebugString(s);
#else 
    std::cerr << s << std::endl;
#endif
}

class google_test_fixture
{
    class boost_test_google_test_adapter : public testing::EmptyTestEventListener 
    {
    public:
        boost_test_google_test_adapter()
            : is_google_test(false)
        {}

    private:
        //! This state is used for google tests.
        bool is_google_test;
        std::string GoogleTestCase;
        std::string GoogleTestName;

        virtual void OnTestStart(const testing::TestInfo& testInfo) 
        {
            is_google_test = true;
            GoogleTestCase = testInfo.test_case_name();
            GoogleTestName = testInfo.name();

            std::stringstream s;
            std::string name = testInfo.name() ? testInfo.name() : "<unknown>";
            s << "***Starting test [" << GoogleTestCase << ":" << name << "]" << std::endl;
            BOOST_TEST_CHECKPOINT(s.str());
#if defined(UNICODE)
            std::string sstr = s.str();
            std::wstring ws(sstr.size(), L' ');
            ws.resize(mbstowcs(&ws[0], sstr.c_str(), sstr.size()));
            output_debug_string(ws.c_str());
#else
            output_debug_string(s.str().c_str());
#endif
        }

        virtual void OnTestPartResult(const testing::TestPartResult& testPartResult)
        {
            std::stringstream s;
            std::string filename = testPartResult.file_name() ? testPartResult.file_name() : "<unknown file>";
            int line_number = testPartResult.line_number();
            std::string summary = testPartResult.summary() ? testPartResult.summary() : "<no description of error>";
            s << "\tTest ["
                << (is_google_test ? (GoogleTestCase + "::") + GoogleTestName : std::string())
                << "] " << (testPartResult.failed() ? "FAILED" : "PASSED") << ":\n\t"
                << filename
                << "("
                << line_number
                << ")\n\t"
                << boost::replace_all_copy(summary, "\n", "\n\t")
                << std::endl;

            BOOST_CHECK_MESSAGE(false, s.str());
#if defined(UNICODE)
            std::string sstr = s.str();
            std::wstring ws(sstr.size(), L' ');
            ws.resize(mbstowcs(&ws[0], sstr.c_str(), sstr.size()));
            output_debug_string(ws.c_str());
#else
            output_debug_string(s.str().c_str());
#endif
        }

        virtual void OnTestEnd(const ::testing::TestInfo& testInfo)
        {
            std::stringstream s;
            std::string name = testInfo.name() ? testInfo.name() : "<unknown>";
            s << "***Ending test [" << GoogleTestCase << ":" << GoogleTestName << "]" << std::endl;
            BOOST_TEST_CHECKPOINT(s.str());
#if defined(UNICODE)
            std::string sstr = s.str();
            std::wstring ws(sstr.size(), L' ');
            ws.resize(mbstowcs(&ws[0], sstr.c_str(), sstr.size()));
            output_debug_string(ws.c_str());
#else
            output_debug_string(s.str().c_str());
#endif
            is_google_test = false;
            GoogleTestCase = "";
            GoogleTestName = "";
        }
    };
public:

    google_test_fixture() 
    {
        testing::InitGoogleMock(&boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);
        testing::TestEventListeners &listeners = testing::UnitTest::GetInstance()->listeners();
        // this removes the default error printer
        delete listeners.Release(listeners.default_result_printer());
        listeners.Append(new boost_test_google_test_adapter);
    }

    ~google_test_fixture() 
    {}

};

BOOST_GLOBAL_FIXTURE(google_test_fixture);

//! Setup a means to run google tests 
namespace google_testing_adaptor
{
    inline int& get_google_test_results() { static int instance = 0; return instance; }
}
BOOST_AUTO_TEST_CASE(GEOMETRIX_GOOGLE_TESTS)
{
    //! Run any GOOGLE Tests.
    google_testing_adaptor::get_google_test_results() = RUN_ALL_TESTS();
}

#endif // TEST_google_test_fixture_HPP
