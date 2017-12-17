#include "state_map.hpp"

template <class TYPE> state_map<TYPE> :: state_map(int id, TYPE data)
{
    root = new node<TYPE>
    {
        id,
        data
    };
}

// Make sure memory is cleaned
template <class TYPE> state_map<TYPE> :: ~state_map(void)
{
    std::vector< node<TYPE>* > visited;

    if (root)
        _destroy_(root, visited);

    for (auto &item : visited)
    {
        for (auto &link : item->links)
        {
            delete link;
        }
        delete item;
    }
}


// Checks if current node is visited, if not iterates down its children
template <class TYPE> void state_map<TYPE> :: _destroy_(node<TYPE> *link, std::vector<node<TYPE>*> &visited)
{
    for (auto &item : visited)
    {
        if (link->id == item->id)
            return;
    }

    visited.push_back(link);

    for (auto &item : link->links)
        _destroy_(item->to, visited);

    return;
}

// Check if node exists
template <class TYPE> node<TYPE>* state_map<TYPE> :: exists(int id)
{
    std::vector<node<TYPE>*> visited;

    return _exists_(id, root, visited);
}

template <class TYPE> node<TYPE>* state_map<TYPE> :: _exists_(int id, node<TYPE>* link, std::vector<node<TYPE>*> visited)
{
    if (link->id == id)
        return link;
    
    for (auto &item_check : link->links)
    {
        bool found = false;

        for (auto &vis : visited)
        {
            if (item_check->to->id == vis->id)
                found = true;
        }

        if (!found)
        {
            visited.push_back(item_check->to);
            return _exists_(id, item_check->to, visited);
        }
    }

    return NULL;
}

// Add node to parent
template <class TYPE> void state_map<TYPE> :: _add_(node<TYPE> *parent, int id, TYPE data, int (*func)(TYPE, TYPE), int weight)
{
    node<TYPE> *tempnode = new node<TYPE>
    {
        id, 
        data
    };

    node_link<TYPE> *templink = new node_link<TYPE>
    {
        tempnode,
        func,
        weight
    };

    parent->links.push_back(templink);
}

// Adds a node to a parent based on it's respective ID, also checks if nodes already exists and aren't yet linked
template <class TYPE> void state_map<TYPE> :: addNode(int parent, int id, TYPE data, int (*func)(TYPE, TYPE), int weight)
{
    if (id == parent || !root)
        return;
    else
    {
        node<TYPE>* p_exists = exists(parent);
        node<TYPE>* c_exists = exists(id);

        if (p_exists)
        {
            if (c_exists)
            {
                for (auto &item : p_exists->links)
                {
                    if (item->to->id == c_exists->id)
                        return;      
                }
                node_link<TYPE> *templink = new node_link<TYPE>
                {
                    c_exists,
                    func,
                    weight
                };

                p_exists->links.push_back(templink);
            }
            else
            {
                _add_(p_exists, id, data, func, weight);
            }
        }
    }
}

// Print node and its children
template <class TYPE> void state_map<TYPE> :: printNode(int id)
{
    _printNode_(exists(id), id);
}

template <class TYPE> void state_map<TYPE> :: _printNode_(node<TYPE> *link, int id)
{
    if (!link)
        return;

    std::cerr << "==Node==\n\t" << id << std::endl;
    std::cerr << "==Links==" << std::endl;

    for (auto &item : link->links)
    {
        std::cerr << "\t" << item->to->id << " -> with weight " << item->weight << std::endl;
    }
}