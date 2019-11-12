#include <iostream>
#include <vector>
#include <iterator>
#include <map>

template <class Iterator, bool IsFirst>
struct pair_iterator : public Iterator
{
};

template<class Iterator>
struct pair_iterator<Iterator, true> : public Iterator
{
    using base_type = typename std::iterator_traits<Iterator>::value_type;
    using reference = typename pair_iterator::base_type::first_type&;
    pair_iterator(Iterator init)
        : Iterator {init}
    {
    }
    auto& operator*()
    {
        return Iterator::operator*().first;
    }
    auto& operator*() const
    {
        return Iterator::operator*().first;
    }
};

template<class Iterator>
struct pair_iterator<Iterator, false> : public Iterator
{
    using base_type = typename std::iterator_traits<Iterator>::value_type;
    using reference = typename base_type::second_type&;

    pair_iterator(Iterator init)
        : Iterator {init}
    {
    }
    auto& operator*()
    {
        return Iterator::operator*().second;
    }
    auto& operator*() const
    {
        return Iterator::operator*().second;
    }
};

template<class Iterator>
using key_iterator = pair_iterator<Iterator, true>;

template<class Iterator>
using value_iterator = pair_iterator<Iterator, false>;

template <class MapIterator>
value_iterator<MapIterator> make_value_iterator (MapIterator iterator)
{
    return value_iterator<MapIterator> {iterator};
};

template <class MapIterator>
key_iterator<MapIterator> make_key_iterator (MapIterator iterator)
{
    return key_iterator<MapIterator> {iterator};
};

template<class Map>
struct map_keys_adaptor
{
    using iterator = key_iterator<typename Map::iterator>;

    map_keys_adaptor(Map& map) : m_map {map} {}

    auto begin()
    {
        return make_key_iterator(m_map.begin());
    }
    auto end()
    {
        return make_key_iterator(m_map.end());
    }
    auto begin() const
    {
        return make_key_iterator(m_map.cbegin());
    }
    auto end() const
    {
        return make_key_iterator(m_map.cend());
    }
private:
    Map& m_map;
};

template<class Map>
struct map_values_adaptor
{
    using iterator = value_iterator<typename Map::iterator>;

    map_values_adaptor(Map& map) : m_map {map} {}

    auto begin()
    {
        return make_value_iterator(m_map.begin());
    }
    auto end()
    {
        return make_value_iterator(m_map.end());
    }
    auto begin() const
    {
        return make_value_iterator(m_map.cbegin());
    }
    auto end() const
    {
        return make_value_iterator(m_map.cend());
    }
private:
    Map& m_map;
};

template <class Map>
map_values_adaptor<Map> make_values_adaptor(Map& map)
{
    return map_values_adaptor<Map> {map};
}

template <class Map>
map_keys_adaptor<Map> make_keys_adaptor(Map& map)
{
    return map_keys_adaptor<Map> {map};
}

int main()
{
    std::map<std::string, std::string> data {
        {"1", "2"},
        {"3", "4"},
        {"5", "6"}
    };

    for (auto& key : make_values_adaptor(data))
    {
        std::cout << key << std::endl;
    }

    std::vector <std::string> values {std::make_move_iterator(make_value_iterator(data.begin())),
                                      std::make_move_iterator(make_value_iterator(data.end()))};

    std::copy(make_key_iterator(data.begin()),
              make_key_iterator(data.end()),
              std::back_inserter(values));

    std::copy(values.begin(),
              values.end(),
              std::ostream_iterator<std::string>(std::cout, " "));

    return 0;
}
