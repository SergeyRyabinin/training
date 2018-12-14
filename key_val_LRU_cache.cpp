#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <functional>
#include <fstream>
#include <assert.h>
#include <stack>

// Implement last recently used cache
// Implemented just a cache as a bonus
// Implemented as a template as a bonus
// Implemented key-value cache instead of just simple set as a bonus

template <typename Key, typename Value, bool isLRU = true>
class myCache
{
public:
    myCache(const size_t size)
        :_size(size)
    {
    }

    // O(1), worst O(n)
    void push(const Key& key, const Value& val)
    {
        // O(1), worst O(n)
        if (_storage.find(key) != _storage.end())
        {
            *_storage[key] = { key,val };
        }
        else
        {
            if (_list.size() >= _size)
            {
                // O(1), worst O(n)
                _storage.erase(_list.back().first);
                _list.pop_back();
            }

            // O(1)
            _list.push_front({ key,val });
            // O(1), worst O(n)
            _storage[key] = _list.begin();
        }
    }

    const Value* get(const Key& key)
    {
        if (_storage.find(key) != _storage.end())
        {
            auto it = _storage.at(key);
            const Value* ret = &it->second;
            if (isLRU)
            {
                // What makes it LRU cache instead of just cache:
                // Move it to the front
                _list.splice(_list.begin(), _list, it);
                _storage[key] = _list.begin();
            }

            return ret;
        }

        return nullptr;
    }

protected:
    const size_t _size;

    std::unordered_map<Key, 
        typename std::list<
            std::pair<Key, Value>
        >::iterator
    > _storage;
    std::list<std::pair<Key, Value> > _list;
};

//LRU
int main()
{
    std::cout << "Hello world!\n";

    myCache<int, std::string, true> cache(5);

    cache.push(0, "Zero");
    cache.push(1, "First");
    cache.push(2, "Second");
    cache.push(3, "Third");
    cache.push(4, "Forth");
    const std::string* str = 0;
    for (size_t i = 0; i < 2; ++i)
    {
        if (str = cache.get(i))
        {
            std::cout << "From cache: " << *str << "\n";
        }
    }
    cache.push(5, "Fiveth");
    cache.push(6, "Sixth");
    cache.push(7, "Seventh");


    std::cout << "\n\n";
    for (size_t i = 0; i < 8; ++i)
    {
        if (str = cache.get(i))
        {
            std::cout << "From cache: " << *str << "\n";
        }
    }

    return 0;

}