// URL: http://www.geeksforgeeks.org/b-tree-set-1-insert-2/
// URL: http://www.geeksforgeeks.org/b-tree-set-3delete/

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
	
		int find_key(int k);

		// remove() function and its associated helpers.
		void remove(int k);
		
		void remove_from_leaf(int k);

		void remove_from_non_leaf(int k);

		int get_pred(int index);

		int get_succ(int index);

		void fill(int index);

		void borrow_from_prev(int index);

		void borrow_from_next(int index);

		void merge(int idx);

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

int Node::find_key(int k) {
	int idx = 0;
	while ((idx < this->num_keys) && (this->keys[idx] < k)) {
		idx++;
	}
	return idx;
}

// A function to remove the key k from the sub-tree rooted with this node
void Node::remove(int k) {
	int idx = find_key(k);

	if ((idx < this->num_keys) && (keys[idx] == k)) {
		if (this->leaf) {
			remove_from_leaf(k);

		} else {
			remove_from_non_leaf(k);
		}
		return;
	}

	if (this->leaf) {
		cout << "The key " << k << " does not exist in the tree." << endl;
		return;
	}

	// Ok. So, at this point, either
	// - idx == this->num_keys OR
	// - ((idx < this->num_keys) AND (keys[idx] != k))
	
	// The key to be removed is present in the sub-tree rooted with this node.
	// The flag indicates whether the key is present in the sub-tree rooted
	// with the last child of this node.
	bool flag = (idx == this->num_keys);

	// If the child where the key is supposed to exist has less than t keys,
	// we will fill that child. This is done by borrowing keys from
	// neighboring nodes, if possible
	if (this->child[idx]->num_keys < this->T) {
		this->fill(idx);
	}

	// If the last child has been merged, it must have merged with the
	// previous child and so we recurse on the (idx-1)th child. Else, we
	// recurse on the idx-th child, which now has atleast T keys.
	if (flag && (idx > this->num_keys)) {
		this->child[idx-1]->remove(k);

	} else {
		// Invoked for the case
		// - idx < this-num_keys OR
		// - ((idx == this->num_keys) AND (idx <= this->num_keys))
		this->child[idx]->remove(k);
	}

	return;
}

void Node::remove_from_leaf(int idx) {
	
	// Move all the keys after the idx-th position, one place backward.
	for (int i = idx+1; (i < this->num_keys); i++) {
		this->keys[i-1] = this->keys[i];
	}

	// No child node pointers to manipulate since this is a leaf.
	
	this->num_keys--;
	return;
}

void Node::remove_from_non_leaf(int idx) {
	int k = this->keys[idx];

	if (this->child[idx] >= this->T) {
		// We shifted the responsibility to deleting pred from previous-child 
		// instead of k itself.
		int pred = get_pred(idx);
		keys[idx] = pred;
		this->child[idx]->remove(pred);

	} else if (this->child[idx + 1] >= this->T) {
		// We shifted the responsibility to delete the succ value from the
		// next child instead of k itself
		int succ = get_succ(idx);
		keys[idx] = succ;
		this->child[idx+1]->remove(succ);

	} else {
		// Both previous and next child have less than t keys. Merge them AND
		// the k value into the C[idx]. Now, C[idx] has 2t-1 keys. Free
		// C[idx+1], and delete k from C[idx]
		this->merge(idx);
		this->child[idx]->remove(k);
	}
}

// A function to get predecessor of the key at the specified idx:
// this->keys[idx]
int Node::get_pred(int idx) {
	// Keep moving towards right for child previous to keys[idx]
	shared_ptr<Node> curr = this->child[idx];
	while (curr->leaf == false) {
		curr = curr->child[curr->num_keys];
	}

	// Return the last key of the leaf
	return curr->keys[curr->num_keys-1];
}

// A function to get the success of key at specified idx: this->keys[idx]
int Node::get_succ(int idx) {
	// Keep moving towards right for child previous to keys[idx]
	shared_ptr<Node> curr = this->child[idx+1];
	while (curr->leaf == false) {
		curr = curr->child[0];
	}

	// Return the first key of the leaf
	return curr->keys[0];
}

// A function to fill child C[idx] which has less than t-1 keys. 
void Node::fill(int idx) {
	
	if ( (idx != 0) && (this->child[idx-1]->num_keys >= this->T)) {
		this->borrow_from_prev(idx);

	} else if ((idx != this->num_keys) && (this->child[idx+1]->num_keys >= T)) {
		this->borrow_from_next(idx);

	} else if (idx != this->num_keys) {
		// Merge child[idx] with keys[idx] && child[idx+1];
		merge(idx);

	} else {
		// Merge child[idx] with keys[idx-1] and child[idx-1]
		// Question: What about the 0 idex check here??
		merge(idx-1);
	}
}

// A function to borrow a key from this->child[idx-1] and insert into
// this->child[idx]
void Node::borrow_from_prev(int idx) {
	
	shared_ptr<Node> child = this->child[idx];
	shared_ptr<Node> sibling = this->child[idx-1];

	// The last key from C[idx-1] goes up to the parent and key[idx-1]
	// from parent is inserted as the first key in C[idx]. Thus, the  loses
	// sibling one key and child gains one key
 
	// Moving all key in C[idx] one step ahead	
	for (int i = (child->num_keys-1); i >=0; --i) {
		child->keys[i+1] = child-keys[i];
	}

	if (child->leaf == false) {
		for(int i = child->num_keys; i >= 0; i--) {
			child->child[i+1] = child->child[i];
		}
	}	

	// Setting child's first key equal to keys[idx-1] from the current node
	child->keys[0] = this->keys[idx-1];

	// Moving sibling's last child as C[idx]'s first child
	if (this->leaf == false) {
		child->child[0] = sibling->child[sibling->num_keys];
	}

	// Moving the key from the sibling to the parent
	keys[idx-1] = sibling->keys[sibling->num_keys -1];

	child->num_keys++;
	sibling->num_keys--;

}

// A function to borrow a key from the C[idx+1] and place it in child[idx+1]
void Node::borrow_from_next(int idx) {
	
	shared_ptr<Node> child = this->child[idx];
	shared_ptr<Node> sibling = this->child[idx+1];

	// Current node's keys[idx] is inserted as the last key in C[idx]
	child->keys[child->num_keys] = this->keys[idx];

	// Sibling's first child is inserted as the last child into C[idx]
	if (child->leaf == false) {
		child->child[child->num_keys + 1] = sibling->child[0];
	}

	//The first key from sibling is inserted into keys[idx]
	this->keys[idx] = sibling->keys[0];

	// Moving all keys in sibling one step behind
	for (int i = 1; i < sibling->num_keys; i++) {
		sibling->keys[i-1] = sibling->keys[i];
	}

	if (sibling->leaf == false) {
		for(int i = 1; i <= sibling->num_keys; i++) {
			sibling->child[i-1] = sibling->child[i];
		}
	}

	// Increasing and decreasing the key count of C[idx] and C[idx+1]
	// respectively.
	child->num_keys++;
	sibling->num_keys--;

}

// A function to merge this->child[idx] with this->child[(idx+1)]
// this->child[idx+1] is freed after the merging.
void Node::merge(int idx) {
	
	shared_ptr<Node> child = this->child[idx];
	shared_ptr<Node> sibling = this->child[idx+1];

	// Pull the key from the current node and insert it into the (t-1)th
	// position. 
	child->keys[this->T-1] = this->keys[idx];
	
	// In the current node, moving all keys after idx, one step before.
	for (int i = idx+1; i < this->num_keys; i++) {
		this->keys[i-1] = this->keys[i];
	}

	// Copying the keys from C[idx+1] to C[idx] at the end
	for(int i = 0; i < sibling->num_keys; i++) {
		child->keys[i+ this->T] = sibling->keys[i];
	}

	// Copying the child pointers from C[idx+1] to C[idx]
	if (child->leaf == false) {
		for (int i = 0; i < sibling->num_keys; i++) {
			child->child[i + T] = sibling->child[i];
		}
	}
	
	// In the current node, moving all the child-pointers after (idx+1) one
	// step before (simply because idx[i+1] == sibling-node is to be deleted.
	for (int i = idx+2; i <= this->num_keys; i++) {
		this->child[i-1] = this->child[i];
	}

	// Update key counts for current node and child node.
	this->num_keys--;
	this->child[idx]->num_keys += (sibling->num_keys + 1);

	// Deleting sibling by letting sibling go out of scope.
	return;
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

		void remove(int k);

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

void BTree::remove(int k) {
	if (root == nullptr) {
		return;
	}

	//	Call the remove function on the 
	root->remove(k);

	// If root node now has 0 keys, make its first child as the root (if
	// child exists). Else set it to null.
	if(root->num_keys == 0) {
		shared_ptr<Node> tmp = root;
		if (root->leaf) {
			root = nullptr;
		} else {
			root = root->child[0];
		}
	}
}

//=============================================================================

int main_insert(int argc, char **argv) {
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

int main_delete(int argc, char **argv) {
    BTree t(3); // A B-Tree with minium degree 3
 
    t.insert(1);
    t.insert(3);
    t.insert(7);
    t.insert(10);
    t.insert(11);
    t.insert(13);
    t.insert(14);
    t.insert(15);
    t.insert(18);
    t.insert(16);
    t.insert(19);
    t.insert(24);
    t.insert(25);
    t.insert(26);
    t.insert(21);
    t.insert(4);
    t.insert(5);
    t.insert(20);
    t.insert(22);
    t.insert(2);
    t.insert(17);
    t.insert(12);
    t.insert(6);
 
    cout << "Traversal of tree constructed is\n";
    cout << t << endl;
 
    t.remove(6);
    cout << "Traversal of tree after removing 6\n";
    cout << t << endl;
 
    t.remove(13);
    cout << "Traversal of tree after removing 13\n";
    cout << t << endl;
 
    t.remove(7);
    cout << "Traversal of tree after removing 7\n";
    cout << t << endl;
 
    t.remove(4);
    cout << "Traversal of tree after removing 4\n";
    cout << t << endl;
 
    t.remove(2);
    cout << "Traversal of tree after removing 2\n";
    cout << t << endl;
 
    t.remove(16);
    cout << "Traversal of tree after removing 16\n";
    cout << t << endl;
 
    return 0;
}

int main(int argc, char **argv) {
	// return main_insert(argc, argv);
	return main_delete(argc, argv);
}
//=============================================================================
