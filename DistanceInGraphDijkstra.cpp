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

#if !defined(INT_MAX)
#define INT_MAX (0xFFFFFFFF - 1)/2
#endif

// Just a Dijkstra algorithm implementation

auto compare = [](const std::pair<unsigned int, int>& left, const std::pair<unsigned int, int>& right)
{
    return left.second > right.second;
};

class graph_t : public std::unordered_map<
    unsigned int,
    std::vector<std::pair<unsigned int, int>
    >
>
{
public:
    void addEdge(unsigned int src, unsigned int dst, int cost)
    {
        (*this)[src].push_back({ dst,cost });
        //std::push_heap((*this)[src].begin(), (*this)[src].end(), compare);
        (*this)[dst].push_back({ src,cost });
        //std::push_heap((*this)[dst].begin(), (*this)[dst].end(), compare);
    }

    void make_heap()
    {
        //O(V * 3E)
        for (auto& node : (*this))
        {
            std::make_heap(node.second.begin(), node.second.end(), compare);
        }
    }
};

int main()
{
    graph_t graph;

    graph.addEdge(1, 2, 7);
    graph.addEdge(1, 3, 9);
    graph.addEdge(1, 6, 14);

    graph.addEdge(2, 3, 10);
    graph.addEdge(2, 4, 15);

    graph.addEdge(3, 6, 2);
    graph.addEdge(3, 4, 11);

    graph.addEdge(4, 5, 6);

    graph.addEdge(5, 6, 9);

    graph.make_heap();

    //distance from queried vertex to all others
    std::vector<unsigned int> dist(graph.size(), INT_MAX);
    std::vector<int> path(graph.size());
    std::unordered_set<unsigned int> visited;

    unsigned int node = 1;
    dist[node - 1] = 0;
    while (visited.size() != graph.size())
    {
        for(size_t i = 0; i < graph[node].size(); ++i)
        {
            std::pop_heap(graph[node].begin(), graph[node].end() - i, compare);
            const auto& edge = *(graph[node].end() - i - 1);

            if (visited.find(edge.first) != visited.end())
                continue;

            if (dist[edge.first - 1] > dist[node - 1] + edge.second)
            {
                dist[edge.first - 1] = dist[node - 1] + edge.second;
                path[edge.first - 1] = node;
            }
        }

        visited.insert(node);

        int min_price = INT_MAX;
        int min_idx = 0;
        for (size_t i = 1; i < dist.size() + 1; ++i)
        {
            if (visited.find(i) == visited.end() &&
                dist[i - 1] < std::min(INT_MAX, min_price))
            {
                min_price = dist[i - 1];
                min_idx = i;
            }
        }
        if (visited.size() != graph.size())
        {
            assert(min_idx != 0);
        }
        node = min_idx;
    }

    return 0;

}