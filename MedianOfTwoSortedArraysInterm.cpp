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
#include <queue>
#include <functional>

//find median of 2 sorted arrays

std::pair<int,int> merge_till_half(const std::vector<int>& left, const std::vector<int>& right)
{
    auto total = (left.size() + right.size() + 1) / 2;
    auto lIt = left.cbegin();
    auto rIt = right.cbegin();

    //std::vector<int> res;
    size_t res = 0;
    int prev = -1;
    int last = -1;

    if (*lIt < *rIt)
    {
        prev = *lIt;
        ++lIt;
    }
    else
    {
        prev = *rIt;
        ++rIt;
    }
    res++;

    if (*lIt < *rIt)
    {
        last = *lIt;
        ++lIt;
    }
    else
    {
        last = *rIt;
        ++rIt;
    }
    res++;

    while (lIt != left.cend() && rIt != right.cend() && res != total)
    {
        if (*lIt < *rIt)
        {
            prev = last;
            last = *lIt;
            ++res;
            ++lIt;
        }
        else if (*lIt > *rIt)
        {
            prev = last;
            last = *rIt;
            ++res;
            ++rIt;
        }
        else
        {
            prev = last;
            last = *lIt;
            ++res;
            ++lIt;
            if (res != total)
            {
                prev = last;
                last = *rIt;
                ++res;
                ++rIt;
            }
        }
    }

    while (lIt != left.cend() && res != total)
    {
        prev = last;
        last = *lIt;
        ++res;
        ++lIt;
    }
    while (rIt != right.cend() && res != total)
    {
        prev = last;
        last = *rIt;
        ++res;
        ++rIt;
    }

    return { prev, last };
}

int main()
{
    std::vector<int> vec1 = { 1,3,5,7,9,12,15,16 };
    std::vector<int> vec2 = { 2,2,6,11,13,17,20,21,22 };

    auto merged = merge_till_half(vec1, vec2);

    auto res = (merged.first + merged.second) / 2;

    return res;
    //auto& smaller = vec1.size() > vec2.size() ? vec1 : vec2;
    //auto& bigger  = vec1.size() > vec2.size() ? vec2 : vec1;
}