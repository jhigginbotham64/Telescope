//
// Copyright 2022, Joshua Higginbotham
//

#pragma once

#include <exception>
#include <mutex>
#include <iostream>
#include <map>
#include <functional>
#include <sstream>
#include <vector>


/// \brief test interface, intended to be used with CMakes CTest. c.f. `test/template_test` for usage example
/// \author Clemapfel
namespace Test
{
    // exception thrown when a Test::test call fails
    struct AssertionException : public std::exception
    {
        AssertionException(const std::string& s)
                : _message(s)
        {}

        virtual const char* what() const noexcept override final
        {
            return _message.c_str();
        }

        std::string _message;
    };

    // internal state, do not interact
    namespace detail
    {
        static std::map<std::string, bool> _results;  // true = failed, false = passed
        static std::map<std::string, std::vector<std::string>> _assertion_messages;
        std::mutex _result_lock;
    }

    /// \brief initialize the test environment
    void initialize()
    {
        detail::_result_lock.lock();
        detail::_results.clear();
        detail::_result_lock.unlock();
    }

    /// \brief conclude the test, use `return = Test::conclude()` at the end of each test main
    /// \returns 0 if all tests successful, 1 otherwise
    int conclude()
    {
        size_t n = 0;
        size_t n_passed = 0;
        for (auto& pair : detail::_results)
        {
            std::cout << pair.first << ": " << (pair.second ? "FAILED" : "PASSED") << std::endl;
            for (auto& message : detail::_assertion_messages.at(pair.first))
            {
                std::cout << " _________________________________\n";
                std::cout << "| \n";
                std::cout << "| " << message << "\n";
                std::cout << "|_________________________________\n\n" << std::endl;
            }

            n++;
            if (not pair.second)
                n_passed++;
        }

        std::cout << n_passed << " out of " << n << " tests passed." << std::endl;


        if (n_passed != n)
            return 1;
        else
            return 0;
    }

    /// \brief starts a test set, a collection of individual Test::test calls
    /// \tparam Lambda_t  function type, arbitrary signature
    /// \param name: name of testset
    /// \param lambda: testset body
    template<typename Lambda_t>
    void testset(const std::string& name, Lambda_t&& lambda)
    {
        bool failed = false;

        std::cout.setstate(std::ios_base::failbit);
        std::cerr.setstate(std::ios_base::failbit);

        std::string assertion_name = "";
        std::vector<std::string> assertion_messages;
        try
        {
            lambda();
        }
        catch (const AssertionException& e)
        {
            failed = true;
            assertion_messages.emplace_back(e.what());
        }
        catch (const std::exception& e)
        {
            failed = true;
            assertion_messages.emplace_back(e.what());
        }

        std::cout.clear();
        std::cerr.clear();

        detail::_result_lock.lock();

        detail::_results.emplace(name, failed);
        detail::_assertion_messages.emplace(name, assertion_messages);

        detail::_result_lock.unlock();
    }

    /// \brief test if condition evaluates to true
    /// \param condition
    /// \param name (optional)
    void test(bool condition, const std::string name = "")
    {
        if (not condition)
            throw AssertionException("Assertion \"" + name + "\" failed");
    }

    /// \brief test if function throws an exception of any kind
    /// \tparam Lambda_t, arbitrary signature
    /// \param function
    /// \param name (option)
    template<typename Lambda_t>
    void test_throws(Lambda_t&& lambda, const std::string name = "")
    {
        try
        {
            lambda();
        }
        catch (...)
        {
            return;
        }

        throw AssertionException("Assertion \"" + name + "\" failed");
    }
}
