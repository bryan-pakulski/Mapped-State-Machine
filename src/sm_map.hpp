#ifndef _SM_MAP_HPP
#define _SM_MAP_HPP_

// Created by Bryan Pakulski

#include <cstddef>
#include <stdio.h>
#include <vector>
#include <algorithm>

// Data structures
template <typename TYPE>
struct node;

// Linking structure
template <typename TYPE>
struct node_link;


// Node data structure
template <typename TYPE>
struct node
{
	// Data node storage
	TYPE data;

	// Children pointers
	std::vector<node_link<TYPE>*> links;
};

// Linking structure
template <typename TYPE>
struct node_link
{
	// Nodes shared, to = child, from = parent i.e. the active node
	node<TYPE> *to;
	// Control function for that node, pass values of to and from parents
	int (*func)(TYPE, TYPE);
	// Weight of the path
	int weight;
};


// Our main sm_map class
template <class TYPE>
class sm_map
{

private:

	// sm_map root
	node<TYPE> *root;

	// Recursive functions go here
	void _destroy_(node<TYPE> *link, std::vector<node<TYPE>*> *visited);
	void _printNode_(node<TYPE> *link, TYPE key);
	node<TYPE>* _exists_(TYPE key, node<TYPE> *link, std::vector<node<TYPE>*> visited);


	bool comparitor(const node_link<TYPE> &lhs, const node_link<TYPE> &rhs);

public:

	sm_map(TYPE init);
	~sm_map(void);

	node<TYPE>* exists(TYPE key);
	void addNode(TYPE key, TYPE parent, int (*func)(TYPE, TYPE), int weight);
	void printNode(TYPE key);

};

// Order lists by function weight
template <class TYPE> bool
sm_map<TYPE> :: comparitor(const node_link<TYPE> &lhs, const node_link<TYPE> &rhs)
{
	return lhs.weight < rhs.weight;
}

// Create new root node
template <class TYPE>
sm_map<TYPE> :: sm_map(TYPE init)
{
	node<TYPE> *temp = new node<TYPE>;
	temp->data = init;

	root = temp;
}


// Deconstructor
template <class TYPE>
sm_map<TYPE> :: ~sm_map(void)
{	

	// This vector pointer holds all nodes in our sm_map
	std::vector<node<TYPE>*>* visited = new std::vector<node<TYPE>*>();

	if (root)
	{
		_destroy_(root, visited);
	}

	// Iterate over nodes, remove links and then delete nodes themselves
	for (int i = 0; i < (int) visited->size(); i++)
	{

		// Clean out links
		for (int j = 0; j < (int) (*visited)[i]->links.size(); j++)
		{
			delete (*visited)[i]->links[j];
		}

		// Clean up nodes
		delete (*visited)[i];
	}

	// Cleanup vector 
	delete visited;

}

// Destroy function
template <class TYPE> void
sm_map<TYPE> :: _destroy_(node<TYPE> *link, std::vector<node<TYPE>*> *visited)
{
	bool vis = false;

	// Check if the current node has been visited
	for (int i = 0; i < (int) visited->size(); i++)
	{
		if (link->data == (*visited)[i]->data)
		{
			vis = true;
		}
	}

	// If this node is new add it to our vector and iterate down to each of its children
	if (!vis)
	{
		visited->push_back(link);

		for (int i = 0; i < (int) link->links.size(); i++)
		{
			_destroy_(link->links[i]->to, visited);
		}	
	}

	return;
}

// Adds a node to the sm_mapwork
template <class TYPE> void
sm_map<TYPE> :: addNode(TYPE key, TYPE parent, int (*func)(TYPE, TYPE), int weight)
{
	// Check that nodes aren't the same value
	if (key == parent)
	{
		perror("Key and Parent can't be the same value!");
	}

	// Check if root exists
	if (!root)
	{
		// Error message
		perror("Graph is empty!");
	}
	else
	{
		// Check if parent and child exists
		node<TYPE> *pcheck = exists(parent);
		node<TYPE> *ccheck = exists(key);

		// Parent exists
		if (pcheck != NULL)
		{
			// Child exists
			if (ccheck != NULL)
			{
				bool exists = false; 

				// Check if link exists already
				for (int i = 0; i < (int) pcheck->links.size(); i++)
				{
					if (pcheck->links[i]->to->data == ccheck->data)
					{
						exists = true;
					}
				}

				if (exists)
				{
					printf("Nodes already linked!\n");
					return;
				}
				else
				{	
					// Create link between nodes
					node_link<TYPE> *templink = new node_link<TYPE>;

					templink->to = ccheck;
					templink->func = func;
					templink->weight = weight;

					pcheck->links.push_back(templink);

					// Sort by weight
					//std::sort(pcheck->links.begin(), pcheck->links.end(), comparitor);
				}
			}

			// Create child if it doesn't exist
			if (ccheck == NULL)
			{
				// Create new pointers
				node<TYPE> *tempnode = new node<TYPE>;
				node_link<TYPE> *templink = new node_link<TYPE>;

				tempnode->data = key;

				// Create link between nodes
				templink->to = tempnode;
				templink->func = func;
				templink->weight = weight;

				pcheck->links.push_back(templink);
				
				// Sort by weight
				//std::sort(pcheck->links.begin(), pcheck->links.end(), comparitor);
			}
		}
		else
		{
			printf("Parent doesn't exist!\n");
		}
	}
}


// Print functions //

template <class TYPE> void
sm_map<TYPE> :: printNode(TYPE key)
{
	if (!exists(key))
	{
		printf("Key doesn't exist!\n");
	}
	else
	{
		_printNode_(root, key);
	}
}

template <class TYPE> void
sm_map<TYPE> :: _printNode_(node<TYPE> *link, TYPE key)
{
	// Found the node in question
	if (link->data == key)
	{
		printf("==Node==\n\t%d\n", key);
		printf("==Links==\n");

		for (int i = 0; i < (int) link->links.size(); i++)
		{
			printf("\t%d -> weight (%d)\n", link->links[i]->to->data, link->links[i]->weight);
		}

		// Return to stop searching
		return;
	}
	else
	{
		for (int i = 0; i < (int) link->links.size(); i++)
		{
			_printNode_(link->links[i]->to, key);
		}	
	}
	
}

// Exists functions //

// Check if a node exists in the sm_map
template <class TYPE> node<TYPE>*
sm_map<TYPE> :: exists(TYPE key)
{
	std::vector<node<TYPE>*> visited;

	// Either null or pointer to node
	return _exists_(key, root, visited);
}

template <class TYPE> node<TYPE>*
sm_map<TYPE> :: _exists_(TYPE key, node<TYPE> *link, std::vector<node<TYPE>*> visited)
{	
	// Found the node
	if (link->data == key)
	{
		return link;
	}
	else
	{
		// Continue search
		for (int i = 0; i < (int) link->links.size(); i++)
		{
			bool vis = false;

			// Check that node has not yet been visited
			for (int j = 0; j < (int) visited.size(); j++)
			{	
				if (link->links[i]->to->data == visited[j]->data)
				{
					vis = true;
				}
			}
			
			// Node is new
			if (!vis)
			{
				// Navigate down, add to visited
				visited.push_back(link->links[i]->to);
				return _exists_(key, link->links[i]->to, visited); 
			}
		}
	}

	return NULL;
}

#endif