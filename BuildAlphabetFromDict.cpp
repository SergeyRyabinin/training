#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <stack>
#include <tuple>
#include <queue>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>

// Input: a list of words previously sorted according to some alphabet
// Build an alphabet used to sort this list

// Inheritance is not a nice choice, just to make it faster
// In a real code, I would prefer composition
class graph_t : public std::unordered_map<char, std::unordered_set<char>>
{
public:
    void addEdge(const char src, const char dst)
    {
        if (src == dst)
            return;
        (*this)[src].insert(dst);
        (*this)[dst];
    }

    const auto& getEdges(const char src) const
    {
        return (*this).at(src);
    }
};

void cmpWords(const std::string& left, const std::string& right, graph_t& aGraph)
{
    for (size_t i = 0; i < std::min(left.size(), right.size()); ++i)
    {
        if (left[i] != right[i])
        {
            aGraph.addEdge(left[i], right[i]);
            break;
        }
    }
}

graph_t buildGraph(const std::vector<std::string>& vWords)
{
    graph_t aGraph;
    for (size_t i = 1; i < vWords.size(); ++i)
    {
        cmpWords(vWords[i - 1], vWords[i], aGraph);
    }

    return aGraph;
}

// Topological sorting
void topoGrSortUtil(const graph_t& aGraph, char node, std::unordered_set<char>& visited, std::stack<char>& alph)
{
    visited.insert(node);

    for (const auto& it : aGraph.getEdges(node))
    {
        if (visited.find(it) == visited.end())
        {
            topoGrSortUtil(aGraph, it, visited, alph);
        }
    }
    alph.push(node);
}

std::vector<char> topoGrSort(const graph_t& aGraph)
{
    std::unordered_set<char> visited;
    std::stack<char> alphabet;

    for (const auto& it : aGraph)
    {
        if (visited.find(it.first) == visited.end())
        {
            topoGrSortUtil(aGraph, it.first, visited, alphabet);
        }
    }

    std::vector<char> ret;
    ret.reserve(alphabet.size());
    while (!alphabet.empty())
    {
        ret.push_back(alphabet.top());
        alphabet.pop();
    }

    return ret;
}

std::vector<char> topoGrSortIterative(const graph_t& aGraph)
{
    std::unordered_set<char> visited;
    std::stack<char> alphabet;

    std::queue<char> tasks;
    for (const auto& it : aGraph)
    {
        if (visited.find(it.first) == visited.end())
        {
            tasks.push(it.first);

            while (!tasks.empty())
            {
                char node = tasks.front();
                tasks.pop();

                std::stack<char> stack;
                stack.push(node);

                while (!stack.empty())
                {
                    bool repeat_mode = false;
                    node = stack.top();

                    stack.pop();

                    if (visited.find(node) != visited.end())
                    {
                        continue;
                    }
                    visited.insert(node);

                    bool hasChild = false;
                    for (const auto& it : aGraph.getEdges(node))
                    {
                        if (visited.find(it) == visited.end())
                        {
                            stack.push(it);
                            hasChild = true;
                        }
                    }
                    if (!hasChild)
                    {
                        alphabet.push(node);
                    }
                    else
                    {
                        // For cases when top letter arrives too early
                        visited.erase(node);
                        tasks.push(node);
                    }
                }
            }
        }
    }

    std::vector<char> ret;
    ret.reserve(alphabet.size());
    while (!alphabet.empty())
    {
        ret.push_back(alphabet.top());
        alphabet.pop();
    }

    return ret;
}

std::string GetRandomAlphabet()
{
    std::string aEnglishAlph = "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(aEnglishAlph.begin(), aEnglishAlph.end(), g);

    return aEnglishAlph;
}

void sortByAlphabet(std::vector<std::string>& iWords, const std::string& iAlp)
{
    //std::unordered_map<char, size_t> Alph;
    std::vector<size_t> Alph;
    Alph.resize(iAlp.size());
    for (size_t i = 0; i < iAlp.size(); ++i)
    {
        Alph[iAlp[i]-'a'] = i;
    }

    std::sort(iWords.begin(), iWords.end(), [&Alph](const auto& left, const auto& right)
        {
            for (size_t i = 0; i < std::min(left.size(), right.size()); ++i)
            {
                if (Alph.at(left[i]-'a') < Alph.at(right[i] - 'a'))
                    return true;
                else if (Alph.at(left[i] - 'a') > Alph.at(right[i] - 'a'))
                    return false;
            }
            if (left.size() < right.size())
            {
                return true;
            }

            return false;
        }
    );
}

int main()
{
    //Reading dictionary
    std::ifstream infile("google-10000-englishA-Z.txt");

    std::vector<std::string> vWords;
    std::string line;
    while (std::getline(infile, line))
    {
        vWords.emplace_back(std::move(line));
    }
    infile.close();

    for (size_t i = 0; i < 100; ++i)
    {
        const auto& aAlphabet = GetRandomAlphabet();
        std::cout << "Sort using: " << aAlphabet;
        sortByAlphabet(vWords, aAlphabet);
        std::cout << "  sorting done!\n";

        graph_t aGraph = buildGraph(vWords);

        auto ret = topoGrSortIterative(aGraph);
        //auto ret = topoGrSort(aGraph);
        std::string result{ ret.begin(), ret.end() };
        std::cout << "Got result: " << result << "\n";
        if (result == aAlphabet)
            std::cout << "--- Ok! ---\n";
        else
            std::cout << "--- FAIL! ---\n";
    }

    return 0;
}