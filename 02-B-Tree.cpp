// URL: http://www.geeksforgeeks.org/b-tree-set-1-insert-2/
//

#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

#include <vector>
using std::vector;

#include <memory>
using std::shared_ptr;

//=============================================================================
// Single node in the BTree
class Node {
	private:
		// Note that these two vectors have pre-allocated size.
		vector<int> keys;						// Set of keys in the node.
		vector<shared_ptr<Node> > child;		// Pointer to children.
		const int T;							// Minimum number of children.
		int num_keys;							// Number of keys currently.
		bool leaf;								// Is this node a leaf-node?

	public:
		Node(int T, bool leaf);

		// Simple function to check if this tree is full
		bool full();

		// A utility function to insert a new key in the subtree. This
		// function assumes that the node is NOT full.
		void insert(const int k);

		// A utility function to split the child at index i. This function
		// assumes that the child is full.
		void split_child(int i);

		bool search(int k);

		friend class BTree;
		friend ostream& operator<<(ostream&, const Node &);
};

Node::Node(const int T, bool leaf): keys((2*T - 1)), child(2*T), 
	T(T), num_keys(0), leaf(leaf) {}

bool Node::full() {
	return (this->num_keys == (2*T - 1));
}

ostream& operator<<(ostream& out, const Node &node) {

	int i = 0;
	for( i = 0; i < node.num_keys; i++) {
		if (node.leaf == false) {
			out << *(node.child[i]);
		}
		out << "," << node.keys[i];
	}
	if (node.leaf == false) {
		out << *(node.child[node.num_keys]);
	}

	return out;
}

bool Node::search(int k) {
	
	int i = 0;
	while ( (i < this->num_keys) && (this->keys[i] < k)) {
		i++;
	}

	// If we contain the key, return current node.
	if (this->keys[i] == k) {
		return true;
	}

	// If they are no more child nodes, return null.
	if (this->leaf) {
		return false;
	}

	// continue search at i since key[i] >= k, and child[i] contains all
	// values between key[i-1] and key[i]
	return this->child[i]->search(k);
}

// Note that this function assumes that the node is NOT full.
void Node::insert(const int k) {

	if (this->leaf == true) {
		int i = this->num_keys - 1;
		
		// The following loop does two things
		// - Find the location where the key should be inserted.
		// - Move all the elements one place to the right, to make space.
		while ( (i >= 0) && (this->keys[i] > k)) {
			keys[i+1] = keys[i];
			i--;
		}

		keys[i+1] = k;
		this->num_keys++;
		return;
	}

	// This is not a leaf node.
	// Find which child should have the new key.
	int i = this->num_keys - 1;
	while ( (i>=0) && (this->keys[i] > k)) {
		i--;
	}

	// The child we are talking about, is full. Split it.
	if (child[i+1]->full()) {
		// split this child.
		split_child(i+1);

		// After the split, the middle key of C[i+1] comes up into the current
		// node, and C[i+1] is split into 2. Hence we have to figure out which
		// of the new child is to have the new key.
		if (keys[i+1] < k) {
			i++;
		}
	}

	this->child[i+1]->insert(k);
}

void Node::split_child(int child_index) {

	// The node that we will split.
	shared_ptr<Node> old_node = this->child[child_index];
	const int OLD_T = old_node->T;

	// Create a new node that which is going to store (t-1) keys
	// Its properties are the same as the old_node.
	shared_ptr<Node> new_node(new Node( OLD_T, old_node->leaf));
	new_node->num_keys = (OLD_T-1);

	// Copy last (t-1) keys from old_node to new_node
	for(int i = 0; i < (OLD_T-1); i++) {
		new_node->keys[i] = old_node->keys[ (OLD_T + i) ];
	}

	// If non-leaf node, copy last T children to new_node
	// ** Note how we copy (T-1) keys and T children!!
	if (old_node->leaf == false) {
		for (int i = 0; i < OLD_T; i++) {
			new_node->child[i] = old_node->child[ (OLD_T + i) ];
		}
	}

	// Reduce number of keys in the old node
	old_node->num_keys = (OLD_T - 1);

	// Move the child pointers by one to the right.
	for(int i = (this->num_keys); i >= (child_index+1); i--) {
		this->child[i+1] = this->child[i]; 
	}
	// Set the pointer to the new node.
	this->child[child_index+1] = new_node;

	// Move keys one space 
	for(int i = (this->num_keys -1); i >= child_index; i--) {
		this->keys[i+1] = this->keys[i]; 
	}

	// Copy middle key of old node to this node.
	this->keys[child_index] = old_node->keys[OLD_T-1];
	// Increment number of keys of the current node.
	this->num_keys++;
}

//=============================================================================

class BTree {
	private:
		shared_ptr<Node> root;
		const int T;

	public:
		BTree(const int T): T(T), root(nullptr) {}

		// The search() function simply hands it off to the root node.
		bool search(int k) {
			return (root ? root->search(k) : false);
		}

		// The main function that inserts elements into this btree.
		void insert(int k);

		friend ostream& operator<<(ostream&, const BTree &);
};

ostream& operator<<(ostream& out, const BTree &tree) {
	if (tree.root) {
		out << *tree.root;
	}
	return out;
}

void BTree::insert(int k) {
	// Check if tree is empty
	if (root == nullptr) {
		root = shared_ptr<Node>(new Node(this->T, true));
		root->keys[0] = k;
		root->num_keys++;
		return;
	}

	// If root is not full, call insert in the root.
	// This node will automagically push the value further down.
	if (root->full() == false) {
		root->insert(k);
		return;
	}

	// At this point, the root node is full!! Now the tree will grow in
	// height.
	shared_ptr<Node> new_root(new Node(this->T, false));
	
	// Make old_root as child of the new root.
	new_root->child[0] = root;

	// Split the old root.
	new_root->split_child(0);

	// New Root is going to have two children now. Decide which of the two
	// children is going to have the key.
	int i = 0;
	if (new_root->keys[0] < k) {
		i++;
	}
	new_root->child[i]->insert(k);
	this->root = new_root;
}

//=============================================================================

int main(int argc, char **argv) {
    BTree t(3); // A B-Tree with minium degree 3
    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);
 
    cout << "Traversal of the constucted tree is ";
    cout << t << endl;
 
	cout << "Searching for 6: ";
    t.search(6)? cout << "Present" : cout << "Not Present";
	cout << endl;
 
	cout << "Searching for 15: ";
    t.search(15)? cout << "Present" : cout << "Not Present";
	cout << endl;

    return 0;
}

//=============================================================================
