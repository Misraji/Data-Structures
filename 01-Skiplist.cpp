// URL: http://www.geeksforgeeks.org/skip-list-set-2-insertion/
// URL: http://www.geeksforgeeks.org/skip-list-set-3-searching-deletion/
#include <cstdlib>

#include <iostream>
using std::cout;
using std::endl;

#include <ostream>

#include <algorithm>
using std::max;

//=============================================================================

// Each node in the skiplist.
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
		// Note that the type of P MUST be a float. I accidentally set it to
		// int and then only had 1 level throughout the run.
		const float P;

		// current max-level of any node in this skiplist
		int level;
		Node *header;

		int random_level();
		Node* get_equal_or_next(int key, Node **update);

	public:
		Skiplist(int, float);
		~Skiplist();

		void insert(int key);
		bool search(int key);
		void remove(int key);

		// Simple friend function to print this skiplist to the standard-out.
		friend std::ostream& operator<<(std::ostream &stream, const Skiplist &list);
};

Skiplist::Skiplist(int MAX_LEVEL, float P) : MAX_LEVEL(MAX_LEVEL), P(P), level(0) {
	header = new Node(-1, MAX_LEVEL);
}

Skiplist::~Skiplist() {
	Node *current = header;	
	Node *next = NULL;

	// Delete all the nodes. Note how we traverse only the lowest level.
	while(current) {
		next = current->forward[0];
		delete current;
		current = next;
	}
}

// A method that generates the number of levels the new node should have.
int Skiplist::random_level() {
	float r = (float)rand() / RAND_MAX;
	int level = 0;

	while ((r < this->P) && (level < this->MAX_LEVEL)) {
		++level;
		r = (float)rand() / RAND_MAX;
	}
	return level;
}

// This function simply gets the node that is equal-to-or-greater than the
// given key. It can return NULL. It can NEVER return the this->header node.
Node* Skiplist::get_equal_or_next(int key, Node** update) {
	Node *current = this->header;

	// Note how we search from the top of the lists (from this->level)
	for(int i = level; i >=0; i--) {
		while ( current->forward[i] &&	
			  (	current->forward[i]->key < key)) {
				current = current->forward[i];
		}

		if(update) {
			update[i] = current;
		}
	}

	// Reached level 0. Forward the current to one position to the right.
	// That is the insert position.
	// Note that this automatically takes care of the situation that
	// current==this->header.
	return current->forward[0];
}

bool Skiplist::search(int key) {

	Node *node = get_equal_or_next(key, NULL);
	return ((node) && (node->key == key));
}

void Skiplist::insert(int key) {
	// Create update array and initialize it
	Node *update[this->MAX_LEVEL + 1];
	memset(update, 0, sizeof(Node*)*(this->MAX_LEVEL + 1));

	Node *current = get_equal_or_next(key, update);

	if ( (current == NULL) || (current->key != key)) {
		
		int new_level = random_level();
		Node *n = new Node(key, new_level);

		// Update max level of this skiplist. For the new levels created, set
		// the corresponding pointer in update array to NULL.
		for(int i = this->level + 1; i < (new_level+1); i++) {
			update[i] = this->header;
		}
		this->level = max(this->level, new_level);

		// Note how we update links from 0 to new_level. (** Hmm, the order
		// may not be important in this case.)
		for (int i = 0; i <= new_level; i++) {
			n->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = n;
		}
	}
}

void Skiplist::remove(int key) {
	// Create update array and initialize it
	Node *update[this->MAX_LEVEL + 1];
	memset(update, 0, sizeof(Node*)*(this->MAX_LEVEL + 1));

	Node *current = get_equal_or_next(key, update);

	if (current && (current->key == key)) {

		// Establish links between nodes previous to current at all levels.
		// Note how we start from 0 and break when prev-node (update[i]) does
		// not point to current anymore.
		for(int i = 0; i <= this->level; i++) {
			if (update[i]->forward[i] != current) {
				break;
			}
			update[i]->forward[i] = current->forward[i];
		}

		while ( (this->level > 0) && 
				(header->forward[this->level] == NULL)) {
			this->level--;
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

//=============================================================================

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

	// Search for element 19
	cout << "Found 19 = " << list.search(19) << endl;
	cout << "Found 100 = " << list.search(100) << endl << endl;

	// Delete 19
	cout << "Deleting 19..." << endl;
	list.remove(19);
	cout << "Found 19 = " << list.search(19) << endl;

	cout << list;
}
//=============================================================================
