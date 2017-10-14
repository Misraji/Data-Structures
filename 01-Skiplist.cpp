#include <cstdlib>

#include <iostream>
using std::cout;
using std::endl;

#include <ostream>

#include <algorithm>
using std::max;

//=============================================================================

class Node {
	public:
		int key;
		Node **forward;
		Node(int, int);
		~Node();
};

Node::Node(int key, int level) {
	this->key = key;
	this->forward = new Node*[level+1];
	memset(forward, 0, sizeof(Node *)*(level+1));
}

Node::~Node() {
	delete []this->forward;
}

//=============================================================================

class Skiplist {
	private:
		const int MAX_LEVEL;
		const float P;

		// current max-level of any node in this skiplist
		int level;
		Node *header;

		int random_level();

	public:
		Skiplist(int, float);
		~Skiplist();

		void insert(int key);
		friend std::ostream& operator<<(std::ostream &stream, const Skiplist &list);
};

Skiplist::Skiplist(int MAX_LEVEL, float P) : MAX_LEVEL(MAX_LEVEL), P(P), level(0) {
	header = new Node(-1, MAX_LEVEL);
}

Skiplist::~Skiplist() {
	Node *current = header;	
	Node *next = NULL;

	while(current) {
		next = current->forward[0];
		delete current;
		current = next;
	}
}

int Skiplist::random_level() {
	float r = (float)rand() / RAND_MAX;
	int level = 0;
	while ((r < this->P) && (level < this->MAX_LEVEL)) {
		++level;
		r = (float)rand() / RAND_MAX;
	}
	return level;
}

void Skiplist::insert(int key) {
	Node *current = this->header;

	// Create update array and initialize it
	Node *update[this->MAX_LEVEL + 1];
	memset(update, 0, sizeof(Node*)*(this->MAX_LEVEL + 1));

	for(int i = level; i >= 0; i--) {
		while ( (current->forward[i] != NULL) &&
				(current->forward[i]->key < key) ) {
			current = current->forward[i];
		}
		update[i] = current;
	}

	// Reached level 0. Forward the current to one position to the right.
	// That is the insert position.
	current = current->forward[0];

	if ( (current == NULL) || (current->key != key)) {
		
		int new_level = random_level();

		for(int i = this->level + 1; i < (new_level+1); i++) {
			update[i] = this->header;
		}
		this->level = max(this->level, new_level);

		Node *n = new Node(key, new_level);
		for (int i = 0; i <= new_level; i++) {
			n->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = n;
		}
	}
}

std::ostream& operator<<(std::ostream &stream, const Skiplist &list) { 
	stream << endl;
	for (int i = 0; i <= list.level; i++) {
		stream << "Level " << i << ": ";

		Node *node = list.header->forward[i];
		while (node != NULL) {
			stream << node->key << " ";
			node = node->forward[i];
		}
		stream << endl;
	}
	stream << endl;
	return stream;
}

int main() {
	srand( (unsigned)time(0) );

	// Create Skiplist object with MAX_LEVEL and P
	Skiplist list(3, 0.5);

	list.insert(3);
	list.insert(6);
	list.insert(7);
	list.insert(9);
	list.insert(12);
	list.insert(19);
	list.insert(17);
	list.insert(26);
	list.insert(21);
	list.insert(25);

	cout << list;
}
