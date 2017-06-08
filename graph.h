#include "graphDec.h"

const unsigned int INITIAL_VERTICES_ARR_SIZE = 10;

/*

Linked List

*/


template <class myType>
LLNode<myType>::LLNode() {
/*
Initializes a new linked list node
*/	
	next = NULL;
}
template <class myType>
LLNode<myType>::LLNode(myType initData) {
/*
Initializes a new linked list node with a given data
*/	
	next = NULL;

	data = initData;
}





template <class myType>
LList<myType>::LList() {
/*
Initializes a new linked list
*/
	this->base = NULL;
	this->last = NULL;
}

template <class myType>
void LList<myType>::append(myType newVal) {
/*
Precondition - Parameters:	newVal - value to be appended to the list
Postcondition:				A new node containing a value of newVal is appended to the list
Returns:						void
*/
	// Create a new node
	LLNode<myType>* newNode = new LLNode<myType>(newVal);

	// Append new node to the list
	this->append(newNode);

}
template <class myType>
void LList<myType>::append(LLNode<myType>* newNode) {
/*
Precondition - Parameters:	newNode - node to be appended to the list
Postcondition:				A new node is appended to the list
Returns:						void
*/
	// If the list is currently empty, the new node is the base
	if(this->isEmpty()) {
		this->base = newNode;
		this->last = newNode;
		return;
	}

	// Set the next of last to be the new node
	this->last->next = newNode;

	// Set last to be the new node
	this->last = newNode;
	
}














IMyGraphNode::IMyGraphNode(unsigned int index) {
/*
Precondition - Parameters:	index - index that the node should be given
Postcondition:				An object of type IMyGraphNode is created and initialized
Returns:						void
*/
	this->index = index;

	this->destinationsCount = 0;

	this->destinations = new IMyGraphNode* [1];
	this->destinationsArrSize = 1;

	this->superior = this;
	this->clanSize = 1;
}
IMyGraphNode::~IMyGraphNode() {
/*
Precondition - Parameters:	none
Postcondition:				An object of type IMyGraphNode is destroyed and dynamically allocated memory is freed
Returns:						void
*/
	// TODO
	delete [] destinations;
}

bool IMyGraphNode::connect(IMyGraphNode* newDestination, bool twoWay) {
/*
Precondition - Parameters:	newDestination - pointer to the node this node needs to be connected to
							twoWay	-	if the connection should be made two way or directed only from the source to the destination (default = false)
Postcondition:				newDestination is added to the list of this node's destinations, establishing a connection
Returns:						bool - if the connection was successfully made
*/
	// If the destination is NULL, cannot make a connection
	if(!newDestination || !this) return false;

	// Add destination to the list of destinations
	this->addDestination(newDestination);

	// If two way, add this node to newDestination's list of destinations
	if(twoWay) newDestination->addDestination(this);

}
void IMyGraphNode::addDestination(IMyGraphNode* newDestination) {
/*
Precondition - Parameters:	newDestination - pointer to the node that needs to be added to the list of destinations
Postcondition:				newDestination is added to the list of this node's destinations, establishing a connection
Returns:						void
*/
	// If there is more space in the destinations[] array, simply add the new element
	if(
		this->destinationsCount
	 <
	  this->destinationsArrSize) 
	{
		this->destinations[this->destinationsCount] = newDestination;
	}

	// Otherwise, expand the array and then add the new destination
	else {
		this->expandDestArr();
		this->destinations[this->destinationsCount] = newDestination;
	}

	// Update the number of destinations
	this->destinationsCount ++;

}
void IMyGraphNode::expandDestArr(unsigned int expansionFactor) {
/*
Precondition - Parameters:	expansionFactor - the factor by which the array should be expanded (default = 2)
Postcondition:				expands the destinations*[] array to be able to hold more elements
Returns:						void
*/
	// Temporarily save the address of the old destinations array
	IMyGraphNode** oldDests = this->destinations;

	// Allocate a new chunch of memory with the new desired size
	this->destinations = new IMyGraphNode*[this->destinationsArrSize*expansionFactor];

	// Copy elements from the old array into the new
	for(unsigned int i = 0; i < this->destinationsArrSize; i++) {
		this->destinations[i] = oldDests[i];
	}

	// Update the array size variable
	this->destinationsArrSize *= expansionFactor;

	// Deallocate previously used memory
	delete [] oldDests;

}

bool IMyGraphNode::joinClans(IMyGraphNode* newMember, unsigned int &componentCount) {
/*
Precondition - Parameters:	newMemebr - pointer to the new member (graph node) with which this node should join clans
Postcondition:				The clans of this node and another node from the graph are joined
Returns:						bool - if the clan merging was successful
*/
	// If the new member is NULL, it cannot join
	if(!newMember) return false;

	// Find this node's leader
	IMyGraphNode* thisLeader = this->findLeader();

	// Find the new member's leader
	IMyGraphNode* otherLeader = newMember->findLeader();

	// ==== Join ====

	// If both clan leaders are the same node, do not do anything
	if(thisLeader == otherLeader) return false;
	
	// If this node's clan is larger
	if(thisLeader->clanSize >= otherLeader->clanSize) {
	//	Make this node's leader the leader of the newMember's clan
		otherLeader->superior = thisLeader;
		// Add the other leader to this leader's clan members
		thisLeader->clanMembers.append(otherLeader);
		// Update clan size
		thisLeader->clanSize += otherLeader->clanSize;
	}

	// If the new memeber's clan is larger
	else {
	//	Make the new node's clan leader the leader of this node's clan
		thisLeader->superior = otherLeader;
		// Add the this leader to the other leader's clan members
		otherLeader->clanMembers.append(thisLeader);
		// Update clan size
		otherLeader->clanSize += thisLeader->clanSize;
	}

	// Update Component Count
	if(thisLeader != otherLeader) componentCount --;
}
IMyGraphNode* IMyGraphNode::findLeader() {
/*
Precondition - Parameters:	none
Postcondition:				The leader of this node is found
Returns:						IMyGraphNode* - pointer to the leader of this node
*/
	while(this->superior != this->superior->superior) {
		this->superior = this->superior->superior;
	}

	return this->superior;
}

void IMyGraphNode::displayClan(std::ostream &os) {
/*
Precondition - Parameters:	os - by reference. Where the write should be made to
Postcondition:				The members of this node's clan are written
Returns:						void
*/
	// If this element is not the first one, display spacing characters (for formatting purposes)
	if(this->superior != this) os << ',';

	// Display this element's index
	os << this->index + 1;	// +1 to compensate that they are stored from 0 instead of from 1

	// Recursively display every clan member and their clan members
	LLNode<IMyGraphNode*>* current = this->clanMembers.getBase();

	while (current != NULL) {
		current->getData()->displayClan(os);
		current = current->getNext();
	}

}








IMyGraph::IMyGraph() {
/*
Precondition - Parameters:	none
Postcondition:				An object of type IMyGraph is created and initialized
Returns:						void
*/
	componentCount = 0;
	verticeCount = 0;
	edgeCount = 0;

	vertices = NULL;

}
IMyGraph::~IMyGraph() {
/*
Precondition - Parameters:	none
Postcondition:				An object of type IMyGraph is destroyed and dynamically allocated memory is freed
Returns:						void
*/
	// TODO
	if(vertices != NULL) delete [] vertices;
}

void IMyGraph::readGraph(int &verticeCount, int &edgeCount, std::istream &is) {
/*
Precondition - Parameters:	is - by reference. Where the read should be made from
Postcondition:				The vertices and edges of a graph are read and stored in this object
Returns:						verticeCount (by reference) - number of vertices read
								edgeCount (by reference)	- number of edges read
*/
	// Read the number of vertices
	is >> this->verticeCount;

	// Initialize component count for counting
	this->componentCount = this->verticeCount;

	// Initialize all vertices
	this->vertices = new IMyGraphNode* [this->verticeCount];
	for(int i = 0; i < this->verticeCount; i++) {
		this->vertices[i] = new IMyGraphNode(i);
	}

	// Read the edges
	int source;
	int destination;
	while (is >> source) {
		// Read the destination of each source
		is >> destination;

		// Translating to indexes of the array
		source --;
		destination --;

		// Create the connection
		this->vertices[source]->connect(this->vertices[destination]);

		// Join the clans of the two vertices connected to preserve components information
		this->vertices[source]->joinClans(this->vertices[destination], this->componentCount);

		// Update the number of edges
		this->edgeCount ++;
	}


	// Update values to return
	verticeCount = this->verticeCount;
	edgeCount = this->edgeCount;
}

void IMyGraph::displayComponentInfo(std::ostream &os) {
/*
Precondition - Parameters:	os - by reference. Where the write should be made to
Postcondition:				Information about the graph's components is written
Returns:						void
*/

	if(this->componentCount == 1) os << "There is 1 component.\n";

	else os << "There are " << this->componentCount << " components.\n";

	for(unsigned int i = 0; i < this->verticeCount; i++) {
		// If this vertex's superior is itself, then it is the leader of its clan. Display it
		if( this->vertices[i]->superior == this->vertices[i] ) {
			this->vertices[i]->displayClan(os);
			os << '.' << '\n';
		}
	}
}








/*
=================================
DEBUG FUNCTIONS. PLEASE DISREGARD
=================================
*/
void IMyGraph::displayVertices(std::ostream &os) {
	os << '[';
	for (int i = 0; i < this->verticeCount; i++) {
		os << i << ':' << this->vertices[i]->superior->index << ' ';
	}
	os << ']';
}
template <class myType>
void LList<myType>::displayList(std::ostream &os) {
	os << '[';
	LLNode<myType>* current = this->base;
	while(current) {
		os << current->data << ' ';
		current = current->next;
	}
	os << ']';
}