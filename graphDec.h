
class IMyGraphNode;
class IMyGraph;

template <class myType>
class LLNode;
template <class myType>
class LList;



template <class myType>
class LLNode {

public:
	LLNode();
	LLNode(myType);

	friend class LList<myType>;
	friend class IMyGraph;

	LLNode<myType>* getNext() { return this->next; }
	myType getData() { return this->data; }

protected:
	LLNode<myType>* next;

	myType data;

};

template <class myType>
class LList {

public:

	LList();

	bool isEmpty() { return !(this->base); };

	void append(myType);
	void append(LLNode<myType>*);

	LLNode<myType>* getBase() { return this->base; }

	//DEBUG FUNCTIONS
	void displayList(std::ostream &os = std::cout);

protected:

	LLNode<myType>* base;
	LLNode<myType>* last;

};







class IMyGraphNode {
/*
TODO: Description
*/
public:
	IMyGraphNode(unsigned int index = 0);
	~IMyGraphNode();

	bool connect(IMyGraphNode* newDestination, bool twoWay = false);

	bool joinClans(IMyGraphNode* newMember, unsigned int &componentCount);

	friend class IMyGraph;
// protected:

	IMyGraphNode* findLeader();

	void displayClan(std::ostream &os = std::cout);

	void addDestination(IMyGraphNode* newDestination);
	void expandDestArr(unsigned int expansionFactor = 2);

	unsigned int index;

	unsigned int destinationsCount;
	unsigned int destinationsArrSize;
	IMyGraphNode** destinations;

	IMyGraphNode* superior;			// the node one step above this node (eventually, this chain leads to the leader)
	unsigned int clanSize;
	LList<IMyGraphNode*> clanMembers;

private:
};



class IMyGraph {
/*
TODO: Description
*/

public:

	IMyGraph();
	~IMyGraph();

	void readGraph(int &verticeCount, int &edgeCount, std::istream &is = std::cin);
	void displayComponentInfo(std::ostream &os = std::cout);

	//DEBUG FUNCTIONS
	void displayVertices(std::ostream &os = std::cout);

// protected:

	unsigned int componentCount;
	unsigned int verticeCount;
	unsigned int edgeCount;
	IMyGraphNode** vertices;


private:

	
};
