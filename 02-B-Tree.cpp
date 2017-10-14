// URL: http://www.geeksforgeeks.org/b-tree-set-1-insert-2/
//

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <memory>
using std::shared_ptr;

//=============================================================================
// Single node in the BTree
class Node {
	private:
		vector<int> keys;						// Set of keys in the node.
		vector<shared_ptr<Node> > child;		// Pointer to children.
		const int T;							// Minimum number of children.
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

		void traverse();
		shared_ptr<Node> search(int k);

		friend class BTree;
};

Node::Node(int t, bool leaf): T(t), leaf(leaf) {}

bool Node::full() {
	return (keys.size() == (2*T - 1));
}

// TODO
void Node::traverse() {

}

shared_ptr<Node> Node::search(int k) {
	
	int i = 0;
	while ( (i < keys.size()) && (keys[i] < k)) {
		i++;
	}

	// If we contain the key, return current node.
	if (keys[i] == k) {
		return shared_ptr<Node>(this);
	}

	// If they are no more child nodes, return null.
	if (this->leaf) {
		return nullptr;
	}

	// continue search at i since key[i] >= k, and child[i] contains all
	// values between key[i-1] and key[i]
	return child[i]->search(k);
}

// Note that this function assumes that the node is NOT full.
void Node::insert(const int k) {

	if (this->leaf == true) {
		int i = (this->keys.size() - 1);
		
		// Insert tmp element at the back to make space.
		this->keys.push_back(0);

		// The following loop does two things
		// - Find the location where the key should be inserted.
		// - Move all the elements one place to the right, to make space.
		while ( (i >= 0) && (this->keys[i] > k)) {
			keys[i+1] = keys[i];
			i--;
		}

		keys[i+1] = k;
		return;
	}

	// This is not a leaf node.
	// Find which child should have the new key.
	int i = this->keys.size() - 1;
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
	const int new_key = old_node->keys[OLD_T-1];

	// Create a new node that which is going to store (t-1) keys
	// Its properties are the same as the old_node.
	shared_ptr<Node > new_node(new Node( OLD_T, old_node->leaf));

	// Copy last (t-1) keys from old_node to new_node
	for(int i = 0; i < (OLD_T-1); i++) {
		new_node->keys.push_back( old_node->keys[i + OLD_T] );
	}

	// If non-leaf node, copy last T children to new_node
	// ** Note how we copy (T-1) keys and T children!!
	if (old_node->leaf == false) {
		for (int i = 0; i < OLD_T; i++) {
			new_node->child.push_back( old_node->child[i + OLD_T]);
		}
	}

	// Reduce the number of keys and children in the old_node
	old_node->keys.resize(OLD_T-1);
	old_node->child.resize(OLD_T);

	// Make space in the current node for the new key and new child.
	this->keys.push_back(0);
	this->child.push_back(nullptr);

	// Move the child pointers by one to the right.
	for(int i = (this->child.size()-1); i >= (child_index+1); i--) {
		this->child[i+1] = this->child[i]; 
	}
	// Set the pointer to the new node.
	this->child[child_index+1] = new_node;

	// Move keys one space 
	for(int i = (this->keys.size()-1); i >= child_index; i--) {
		this->keys[i+1] = this->keys[i]; 
	}
	this->keys[child_index] = new_key;
}

//=============================================================================

class BTree {
	private:
		shared_ptr<Node> root;
		const int T;

	public:
		BTree(const int T): T(T), root(nullptr) {}

		// The traverse function simply hands it off to the root node.
		void traverse() {
			if (root) {
				root->traverse();
			}
		}

		// The search() function simply hands it off to the root node.
		shared_ptr<Node> search(int k) {
			return (root ? root->search(k) : nullptr);
		}

		// The main function that inserts elements into this btree.
		void insert(int k);
};

void BTree::insert(int k) {
	// Check if tree is empty
	if (root == nullptr) {
		root = shared_ptr<Node>(new Node(this->T, true));
		root->keys.push_back(k);
		return;
	}

	// 
	if (root->full() == false) {
		root->insert(k);
		return;
	}


}

//=============================================================================

int main(int argc, char **argv) {

}

//=============================================================================
