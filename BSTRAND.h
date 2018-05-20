#ifndef _BSTRAND_H_
#define _BSTRAND_H_
#include <stdexcept>
#include <iostream>
#include<stack>//used for in order traversal and the tree size
#include<cstdlib> //for rand()
#include<time.h>//for time
namespace cop3530 {
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	class BSTRAND {
	private:
		//Create a Node class
		struct Node {
			K key;
			V value;
			Node* left;
			Node* right;
			Node(K newkey, V newvalue) {
				key = newkey;
				value = newvalue;
				left = right = nullptr;
			}
			Node() {

			}
		};//end of node class
		Node * root;
	public:
		//constructors & destructors
		BSTRAND();
		~BSTRAND();
		//Copy constructor
		BSTRAND(const BSTRAND &obj);
		//Copy assignment operator
		BSTRAND& operator= (const BSTRAND &obj);
		//Move constructor
		BSTRAND(BSTRAND&&) noexcept;
		//Move assignment operator
		BSTRAND& operator=(BSTRAND&& obj) noexcept;
		//functions
		void remove(const K& key);
		void insert(const K& key, V value);
		V& lookup(const K& key);
		void clear();
		size_t size();
	private:
		void clear_tree(typename BSTRAND<K, V, comparison, equality>::Node *& node);
		void insert_at_root(typename BSTRAND<K, V, comparison, equality>::Node *& node, const K& key, V value);
		void rotate_right(Node *& tree_root);
		void rotate_left(Node *& tree_root);
		bool checkKey(const K& key, V value);
		void insert_at_leaf(const K& key, V value);
		Node* copy(Node* root);//Used for the copy constructor and copy assignment operator
	};//End of BSTRAND class
	
	  //Define constructor and destructor
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	BSTRAND< K, V, comparison, equality>::BSTRAND() {
		root = nullptr;
		srand((size_t)time(NULL));
	}
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	BSTRAND< K, V, comparison, equality>::~BSTRAND() {
		clear_tree(root);
	}

	//Create a copy function for copy constructor and copy assignemnt operator
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	typename BSTRAND<K, V, comparison, equality>::Node* BSTRAND<K, V, comparison, equality>::copy(Node* root) {
		Node* newRoot = nullptr; //new root
		Node* newLeft = nullptr; //new left child
		Node* newRight = nullptr; //new right child 
								  //return nullptr if the root passed in is null
		if (!root)
		{
			return nullptr;
		}
		else
		{
			newLeft = copy(root->left); //copy the left subtree
			newRight = copy(root->right); //copy the right subtree
			newRoot = new Node(root->key, root->value); //Build a new node to connect the left child and the right child to the root
			newRoot->left = newLeft;
			newRoot->right = newRight;
		}

		return newRoot;
	}


	//four member functions (copy constructor, copy assignment, move constructor, move assignment)
	//Copy constructor
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	BSTRAND <K, V, comparison, equality>::BSTRAND(const BSTRAND &obj) {
		//Start with blank
		root = nullptr;
		//Copy the tree
		root = copy(obj.root);

	}
	//Copy assignment operator
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	BSTRAND<K, V, comparison, equality>& BSTRAND <K, V, comparison, equality>::operator= (const BSTRAND &obj) {
		//Check if two trees have the same address
		if (this != &obj) {
			//delete the original tree
			clear();
			//Copy the tree
			root = copy(obj.root);
		}
		return *this;
	}
	//Move constructor
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	BSTRAND<K, V, comparison, equality>::BSTRAND(BSTRAND &&obj) noexcept:
	root(nullptr)
	{
		//Copy the values from the source obejct
		root = obj.root;
		//leave obj in a destructible state
		obj.root = nullptr;
	}
	//Move assignment operator
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	BSTRAND<K, V, comparison, equality>& BSTRAND<K, V, comparison, equality>::operator=(BSTRAND &&obj) noexcept
	{
		//Check if there are two same trees
		if (this != &obj) {
			//Free the existing tree
			clear();
			//Copy the values from the source obejct
			root = obj.root;
			//leave obj in a destructible state
			obj.root = nullptr;
		}
		return *this;
	}
	//Define clear
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	void BSTRAND<K, V, comparison, equality>::clear() {
		clear_tree(root);
	}
	//Define clear_tree
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	void BSTRAND<K, V, comparison, equality>::clear_tree(typename BSTRAND<K, V, comparison, equality>::Node *& node) {
		if (node == nullptr)
			return;
		clear_tree(node->left);
		clear_tree(node->right);
		delete node;
		node = nullptr;
	}

	//rotate_right
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	void BSTRAND<K, V, comparison, equality>::rotate_right(typename BSTRAND<K, V, comparison, equality>::Node *& tree_root) {
		Node* tmp = tree_root;
		tree_root = tree_root->left;
		tmp->left = tree_root->right;
		tree_root->right = tmp;
	}

	//rotate_left
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	void BSTRAND<K, V, comparison, equality>::rotate_left(typename BSTRAND<K, V, comparison, equality>::Node *& tree_root) {
		Node* tmp = tree_root;
		tree_root = tree_root->right;
		tmp->right = tree_root->left;
		tree_root->left = tmp;
	}

	//Define insert_at_root
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	void BSTRAND<K, V, comparison, equality>::insert_at_root(typename BSTRAND<K, V, comparison, equality>::Node *& node, const K& key, V value) {

		//When the tree is empty
		if (node == nullptr) {
			node = new Node(key, value);
			return;
		}

		//When the current key is greater than the user's key, insert it in the left subtree
		if (!comparison(node->key, key) && !(equality(node->key, key))) {
			insert_at_root(node->left, key, value);
			rotate_right(node);
		}
		//current key is less than the uer's key, insert it in the right subtree
		else if (comparison(node->key, key)) {
			insert_at_root(node->right, key, value);
			//rotate left
			rotate_left(node);
		}
	}

	//Define insert_at_leaf
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	void BSTRAND< K, V, comparison, equality>::insert_at_leaf(const K& key, V value) {
		//When the tree is empty
		if (root == nullptr) {
			root = new Node(key, value);
			return;
		}
		//Make curr root
		Node* curr = root;
		while (true) {
			//When the current key is less than the user's key, Insert it in the right subtree
			if (comparison(curr->key, key)) {
				//Insert value when the right child is null
				if (curr->right == nullptr) {
					curr->right = new Node(key, value);
					break;
				}
				//If the right child is not null
				else {
					curr = curr->right;
				}
			}
			//When the current key is greater then the user's key, Insert it in the left subtree
			else if (!(comparison(curr->key, key)) && !(equality(curr->key, key))) {
				//Insert value when the left child is null
				if (curr->left == nullptr) {
					curr->left = new Node(key, value);
					break;
				}
				//If the left child is not null
				else {
					curr = curr->left;
				}
			}
			//When two keys are equal
			else {
				curr->value = value;
				break;
			}
		}
	}

	//Define size
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	size_t BSTRAND< K, V, comparison, equality>::size() {
		//Empty tree  
		//std::count << "~function is called" << std::endl;
		size_t SizeOfTree = 0;  //use a counter here
		if (root == nullptr)
			return SizeOfTree;
		//non-empty tree  
		Node* curr = root;
		std::stack<Node*> s;
		while (!s.empty() || curr)
		{
			//Traverse to the left-most subtree and save all the nodes when we traverse
			while (curr)
			{
				s.push(curr);
				//std::count << curr->left << std::endl;
				curr = curr->left;
			}
			//when curr is null, we have gotten to the left-most node. We need to pop nodes.
			if (!s.empty())
			{
				curr = s.top();
				s.pop();
				SizeOfTree++;
				//::cout << curr->key << " " <<curr->value << std::endl;
				//Go to the right subtree. Traverse the left part.
				curr = curr->right;
			}
		}
		return SizeOfTree;
	}

	//Define checkKey
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	bool BSTRAND<K, V, comparison, equality>::checkKey(const K& key, V value) {
		//When the tree is empty, no key will be found
		if (root == nullptr) {
			return false;
		}
		//Make curr root
		Node* curr = root;
		while (true) {
			//When the current key is less than the user's key, go to the right subtree
			if ((comparison(curr->key, key))) {
				//return false when the right child is null
				if (curr->right == nullptr) {
					return false;
				}
				//If the right child is not null, make curr equal to its right child
				else {
					curr = curr->right;
				}
			}
			//When the current key is greater than the user's key, go to the left subtree
			else if (!(comparison(curr->key, key)) && !(equality(curr->key, key))) {
				//Print out an error message when the left child is null
				if (curr->left == nullptr) {
					return false;
				}
				//If the left child is not null, set curr to its left child
				else {
					curr = curr->left;
				}
			}
			//When two keys are equal, we exit the loop. We found the key. We also need to update the value.
			else {
				curr->value = value;
				break;
			}
		}
		return true;
	}//End of checkKey

	//Define remove
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	void BSTRAND< K, V, comparison, equality>::remove(const K& Key) {
		//When the tree is empty
		if (root == nullptr) {
			throw std::runtime_error("The tree is empty.\n");
		}

		Node *curr_pre = nullptr;//Set curr_pre null pointer
		Node *curr = nullptr;
		Node *min_pre = nullptr;
		Node *min = nullptr;
		curr = root;//Set the curr equal to root

					//Find the node 
		while (curr)
		{
			//Two keys are the same
			if (equality(curr->key, Key))
			{
				break;
			}
			//Key is greater than curr->key
			else if (!comparison(Key, curr->key) && !equality(Key, curr->key))
			{
				//Go to the right subtree
				curr_pre = curr;
				curr = curr->right;
			}
			//Key is less than curr->key
			else
			{
				//Go to the left child
				curr_pre = curr;
				curr = curr->left;
			}
		}

		//When such a node does not exist
		if (curr == nullptr)
		{
			throw std::runtime_error("The node does not exist.\n");
		}

		//When there is such a node
		/*
		1. If the node is a leaf node, you just delete it
		2. If the node only has left or right child, you delete the node and link its parent to the deleted node's only child
		3. If the node has two children, you need to find the node (min) with smallest value of the deleted node right sub_child_tree
		replace the deleted node curr with min, and do some justification to min's children
		*/
		//Case 1
		//If the root to be deleted is not the root node
		if (curr && curr_pre)
		{
			bool hasTwo = true;    //If the node has two children 
			min_pre = min = curr->right; //min_pre and min point to the right child of curr
										 //Case 1a
			if (curr_pre->left == curr)    //If the node to be deleted is the left child of its parent  
			{
				if (!curr->left && !curr->right) //if the node is a leaf node
				{
					//Delete the leaf node
					curr_pre->left = nullptr;
					delete curr;
					hasTwo = false;
				}
				else if (!curr->right)      //if the node only has left child
				{
					curr_pre->left = curr->left; //Make the left child of curr become the left child of curr_pre
					delete curr;
					hasTwo = false;
				}
				else if (!curr->left)//if the node only has right child
				{

					curr_pre->left = curr->right;//Make the right child of curr become the left child if curr_pre
					delete curr;
					hasTwo = false;
				}
			}
			//Case 1b
			else//If the node is the right child of its parent  
			{
				if (!curr->left && !curr->right)//if the node is a leaf node 
				{
					//Delete curr
					curr_pre->right = nullptr;
					delete curr;
					hasTwo = false;
				}
				else if (!curr->right)//if the node only has left child
				{
					curr_pre->right = curr->left;//Make the left child of curr become the right child of curr_pre
					delete curr;
					hasTwo = false;
				}
				else if (!curr->left)//if the node only has right child
				{
					curr_pre->right = curr->right;//Make the right child of curr become the right child of curr_pre
					delete curr;
					hasTwo = false;
				}
			}

			//if the node has two children
			if (hasTwo)
			{
				min_pre = curr;
				min = curr->right;
				//find the smallest value in curr's right children
				while (min->left)
				{
					min_pre = min;
					min = min->left;
				}

				curr->key = min->key;  //replace the key of the deleted node with min->key
				curr->value = min->value; //replace the value of the deleted node with min->value
										  //delete the min node
										  //There are only two possibilities here:
										  //1. min is the right child of curr node
										  //2. min is not the right child of curr, but min only has a right child or nullptr

										  //min does not have left child (curr->right does not have a left child)
				if (min_pre == curr)
				{
					curr->right = min->right;
				}
				else
				{
					min_pre->left = min->right; //Make the right child of min become the left child of min_pre
				}
				//Delete min
				delete min;
			}
			//std::cout << "The node is removed" << std::endl;

		}
		//The node to be deleted is the root
		if (curr && !curr_pre)
		{
			//If the tree only has a root
			if (!curr->left && !curr->right)
			{
				delete curr;
				root = nullptr;
			}
			//The root has a right child only
			else if (!curr->left && curr->right)
			{
				//Make the right child of the root become the root
				root = curr->right;
				delete curr;
			}
			//The root has a left child only
			else if (curr->left && !curr->right)
			{
				//Make the left child of the root become the root
				root = curr->left;
				delete curr;
			}
			//The root has two children
			else if (curr->left && curr->right)
			{
				root = curr->left;
				Node* right = curr->right;
				delete curr;
				curr = root;
				//Find the largest key in the right subtree of left child of the original root
				//Make the left subtree the root and insert the right subtree to it
				while (true)
				{
					//If curr still has a right child, keep going, make curr equal to curr->right
					if ((!comparison(right->key, curr->key)) && !(equality(right->key, curr->key)) && curr->right)
					{
						curr = curr->right;
						continue;
					}
					if (comparison(right->key, curr->key) && curr->left)
					{
						curr = curr->left;
						continue;
					}
					//If curr does not have a right child, exit the loop
					//Make the left subtree the root and insert the right subtree to it
					if ((!comparison(right->key, curr->key)) && !(equality(right->key, curr->key)) && !curr->right)
					{
						curr->right = right;
						break;
					}
					else if (comparison(right->key, curr->key) && !curr->left)
					{
						curr->left = right;
						break;
					}
				}

			}//end of the if statement for the root node with two children
		}//end of the if statement for the node to be deleted is the root

	}//end of remove function

	//Define insert
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	void BSTRAND< K, V, comparison, equality>::insert(const K& key, V value) {
		//Get the treeSize
		size_t treeSize = size();
		//Generate a random integer from 0 to tree size  (inclusive), that is, [0,treeSize]
		size_t n;
		n = rand() % (treeSize+1);
		//std::cout << "tree size: " <<treeSize<< "  random num: " << n;
		//Determine the way to insert
		//Insert_at_root when the random number is 0
		if (n == 0) {
		//Call the function checkKey to check if the user's key is already in the tree
			if (!checkKey(key, value)) {
				//If the user's key is not in the tree, then call insert_at_root
				insert_at_root(root, key, value);
			}
		        
		}
		//Otherwise, call insert_at_leaf
		else {
		insert_at_leaf(key, value);
		}
		
	}

	 //Define lookup
	template <typename K, typename V, bool(*comparison)(const K&, const K&), bool(*equality)(const K&, const K&)>
	V& BSTRAND< K, V, comparison, equality>::lookup(const K& key) {
		//When the tree is empty
		if (root == nullptr) {
			throw std::runtime_error("The tree is empty.\n");
		}
		//Make curr root
		Node* curr = root;
		while (true) {
			//When the current key is less than the user's key, go to the right subtree
			if ((comparison(curr->key, key))) {
				//Throw an error message when the right child is null
				if (curr->right == nullptr) {
					throw std::runtime_error("No value has been found.\n");
				}
				//If the right child is not null, make curr equal to its right child
				else {
					curr = curr->right;
				}
			}
			//When the current key is greater than the user's key, go to the left subtree
			else if (!(comparison(curr->key, key)) && !(equality(curr->key, key))) {
				//Print out an error message when the left child is null
				if (curr->left == nullptr) {
					throw std::runtime_error("No value has been found.\n");
				}
				//If the left child is not null, set curr to its left child
				else {
					curr = curr->left;
				}
			}
			//When two keys are equal, we exit the loop. We found the key
			else {
				break;
			}
		}
		return curr->value;
	}//End of look up

}//End of namespace cop3530

#endif