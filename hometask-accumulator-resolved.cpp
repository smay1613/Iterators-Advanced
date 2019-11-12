#pragma once
#include <vector>
#include <string>
#include <iostream>

template <class Container>
struct accumulator :
       public std::iterator<std::output_iterator_tag, void, void, void, void>
{
    using value_type = Container;

    accumulator(Container& container)
        : m_container {container}
    {
    }
    accumulator& operator=(const accumulator& rhs)
    {
        m_container = rhs.m_container;
        return *this;
    }
    accumulator& operator=(const Container& rhs)
    {
        m_container.insert(m_container.end(), rhs.begin(), rhs.end());
        return *this;
    }
    accumulator& operator=(Container&& rhs)
    {
        m_container.insert(m_container.end(),
                           std::make_move_iterator(rhs.begin()),
                           std::make_move_iterator(rhs.end()));
        return *this;
    }
    accumulator& operator*()
    {
        return *this;
    }
    accumulator& operator++()
    {
        return *this;
    }

private:
    Container& m_container;
};

void accumulate()
{
    std::vector<std::string> data {"1", "2", "3"};
    std::string res;

    std::copy(data.begin(),
              data.end(),
              accumulator<std::string>(res));

    std::cout << res << std::endl;
}
