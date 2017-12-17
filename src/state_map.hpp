#ifndef _STATE_MAP_HPP
#define _STATE_MAP_HPP

#include <vector>
#include <iostream>

template <typename TYPE> struct node;
template <typename TYPE> struct node_link;

template <typename TYPE> struct node
{
    int id;
    TYPE data;
    std::vector< node_link<TYPE>* > links; 
};

template <typename TYPE> struct node_link
{
    node<TYPE> *to;
    int (*func)(TYPE, TYPE);
    int weight;
};

//-------------------- MAIN CLASS --------------------//

template <class TYPE> class state_map
{
public:
    state_map(int id, TYPE data);
    ~state_map(void);

    void addNode(int parent, int id, TYPE data, int (*func)(TYPE, TYPE), int weight);
    node<TYPE>* exists(int id);
    void printNode(int id);

private:
    node<TYPE> *root;

    void _destroy_(node<TYPE> *link, std::vector<node<TYPE>*> &visited);    
    void _add_(node<TYPE> *parent, int id, TYPE data, int (*func)(TYPE, TYPE), int weight);
    node<TYPE>* _exists_(int id, node<TYPE>* link, std::vector<node<TYPE>*> visited);
    void _printNode_(node<TYPE> *link, int id);

};

#endif