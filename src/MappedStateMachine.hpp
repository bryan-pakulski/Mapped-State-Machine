#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

template <typename TYPE> struct node;
template <typename TYPE> struct nodeLink;

template <typename TYPE> struct node
{
	int id;
	TYPE data;
	std::vector< nodeLink<TYPE>* > links; 

	void sort() {
		std::sort(links.begin(), links.end(), [](nodeLink<TYPE>* lhs, nodeLink<TYPE>* rhs) {
			return lhs->weight > rhs->weight;
		});
	}
};

template <typename TYPE> struct nodeLink {
	node<TYPE> *to;
	int (*func)(TYPE, TYPE);
	int weight;
};

/**
* Class Definition
*/
template <class TYPE> class MappedStateMachine {
private:
	std::map<int, node<TYPE>*> nodeMap;

public:
	MappedStateMachine();
	~MappedStateMachine();

	int addNodeToParent(int parentNodeId, int nodeId, TYPE data, int nodeWeight, int (*func)(TYPE, TYPE));
	int addNode(int nodeId, TYPE data);
	int addLink(int childNodeId, int parentNodeId, int (*func)(TYPE, TYPE), int linkWeight);
	node<TYPE>* getNode(int id);
};

/**
* Constructor
*
* @param data
*/
template <class TYPE>
MappedStateMachine<TYPE>::MappedStateMachine() {}

/**
* Deconstructor - Deletes all nodelinks -> holding node iteratively from hashmap reference
*/
template <class TYPE>
MappedStateMachine<TYPE>::~MappedStateMachine() {
	for (std::pair<const int, node<TYPE>*> nodeIndex : nodeMap) {
		for (nodeLink<TYPE>* link : nodeIndex.second->links) {
			delete link;
		}
		delete nodeIndex.second;
	}
	nodeMap.clear();
}

/**
* Inserts a new node into the current map, if a node can't be inserted -1 is returned, otherwise 0
* 
* @param parentNodeId
* @param nodeId
* @param data
* @param nodeWeight
* @param NodePointerFunc
*
* @return int (-1 failure state, 0 success state)	
*/
template <class TYPE> 
int MappedStateMachine<TYPE>::addNodeToParent(int parentNodeId, int nodeId, TYPE data, int nodeWeight, int (*func)(TYPE, TYPE)) {
	
	auto it = nodeMap.find(parentNodeId);
	auto cIt = nodeMap.find(nodeId);

	// Check if parent node exists / child node doesn't already exist
	if (it == nodeMap.end()) {
		std::cerr << "Unable to insert node, valid parent not found" << std::endl;
		return -1;
	} else if (cIt != nodeMap.end()) {
		std::cerr << "Unabled to insert node, node with id already exists" << std::endl;
		return -1;
	} else {
		// Create new node and link
		node<TYPE>* np = new node<TYPE> {
			nodeId,
			data
		};

		nodeLink<TYPE>* nl = new nodeLink<TYPE> {
			np,
			func,
			nodeWeight
		};

		// Push back node & link
		nodeMap.insert({nodeId, np});
		it->second->links.push_back(nl);
		it->second->sort();
	}

	return 0;
}

/**
* Creates a node without any links, if a node can't be creatied -1 is returned, otherwise 0
* 
* @param nodeId
* @param data
* @param nodeWeight
* @param func
*
* @return int (-1 failure state, 0 success state)	
*/
template <class TYPE> 
int MappedStateMachine<TYPE>::addNode(int nodeId, TYPE data) {
	auto it = nodeMap.find(nodeId);	

	// Check if parent & child exist
	 if (it != nodeMap.end()) {
		std::cerr << "Unabled to insert node, node with id already exists" << std::endl;
		return -1;
	} else {

		node<TYPE>* np = new node<TYPE> {
			nodeId,
			data
		};

		nodeMap.insert({nodeId, np});
	}
	
	return 0;
}

/**
* Inserts a new link into a node, if a link can't be inserted -1 is returned, otherwise 0
* 
* @param childNodeId
* @param parentNodeId
* @param func
* @param linkWeight
*
* @return int (-1 failure state, 0 success state)	
*/
template <class TYPE> 
int MappedStateMachine<TYPE>::addLink(int sourceNodeId, int destNodeId, int (*func)(TYPE, TYPE), int linkWeight) {
	auto sIt = nodeMap.find(sourceNodeId);
	auto dIt = nodeMap.find(destNodeId);

	// Check if parent & child exist
	if (sIt == nodeMap.end()) {
		std::cerr << "Unable to insert node, valid parent not found" << std::endl;
		return -1;
	} else if (dIt == nodeMap.end()) {
		std::cerr << "Unabled to insert node, node with id already exists" << std::endl;
		return -1;
	} else {

		nodeLink<TYPE>* nl = new nodeLink<TYPE> {
			(node<TYPE>*) &dIt->second,
			func,
			linkWeight
		};

		sIt->second->links.push_back(nl);
		sIt->second->sort();
	}
	
	return 0;
}