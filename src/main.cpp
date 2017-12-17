#include "state_map.hpp"
#include "state_map.cpp"

// Example data structure
typedef struct data
{
    int a;
    int b;
    int c;
} data;

// Example processing codes for a node
int codefornode1(data a, data b)
{
    return (a.a % b.a) + (a.b % b.b) + (a.c % b.c);
}

int codefornode2(data a, data b)
{
    return (a.a * b.a) - (a.b % b.b) + (a.c * b.c);
}

int main()
{

    // Example data
    data b
    {
        5,
        6,
        7
    };

    state_map<data> map(0, b);

    // Can also be passed directly in
    map.addNode(0, 1, data {1, 2, 3}, &codefornode1, 1);
    map.addNode(0, 2, b, &codefornode2, 2);
    map.addNode(1, 2, b, &codefornode1, 3);

    node<data> *n = map.exists(0);

    for (auto &i : n->links)
    {
        std::cout << i->func(data {1, 2, 3}, b) << std::endl;
    }

    return 0;
}