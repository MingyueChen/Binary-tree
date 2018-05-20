#ifndef _AVL_H_
#define _AVL_H_

//#include<stdio.h>
#include<stdexcept>
#include<iostream>

namespace cop3530 {

	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	class AVL
	{
	private:
		//Create an AVLNode
		struct AVLNode
		{
			AVLNode(T Key, V Value, AVLNode*l, AVLNode*r)
				:key(Key), value(Value), lchild(l), rchild(r) {}

			T key;
			V value;
			int height = -1;
			AVLNode* lchild;
			AVLNode* rchild;
		};
		AVLNode* root;

	public:
		AVL();			//Constructor
		~AVL();			//Destructor

						//void preOrder();
		void InOrder();
		//void postOrder();

		//void print();
		void clear();

		void insert(const T& key, V value);
		void remove(const T& key);
		V& lookup(const T& key);
		bool contains(const T& key);
		//Copy constructor
		AVL(const AVL &obj);
		//Copy assignment operator
		AVL& operator= (const AVL &obj);
		//Move constructor
		AVL(AVL&&) noexcept;
		//Move assignment operator
		AVL& operator=(AVL&& obj) noexcept;
		
		//T minimum();
		//T maximum();

		int height();		//return the height of the tree


	private:
		AVLNode* copy(AVLNode* root);//Used for the copy constructor and copy assignment operator
		//void preOrder(AVLNode<T, V>* pnode) const;
		void inOrder(AVLNode* pnode) const;
		//void postOrder(AVLNode<T, V>* pnode) const;

		//void print(AVLNode<T, V>* pnode, T key, int direction) const;
		void clear(AVLNode *& pnode);
		bool checkKey(const T& key, V value);
		int height(AVLNode* pnode); //overloaded function
		int max(int a, int b);

		AVLNode* insert(AVLNode* &pnode, const T& key, V value);
		AVLNode* remove(AVLNode* & pnode, const T& key);

		AVLNode* minimum(AVLNode* pnode)const;
		AVLNode* maximum(AVLNode* pnode)const;

		AVLNode* RR_rotation(AVLNode* proot); //RR_rotation: rotate left (counterclockwise)
		AVLNode* LL_rotation(AVLNode* pnode); //LL_rotation: rotate right (clockwise)
		AVLNode* LR_rotation(AVLNode* pnode);  //LR_rotation
		AVLNode* RL_rotation(AVLNode* pnode);  //RL_rotation

	};//End of AVL class

	  //Define the constructor
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	AVL<T, V, comparison, equality>::AVL()
		:root(nullptr) {}

	//Define the destructor
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	AVL<T, V, comparison, equality>::~AVL()
	{
		//call clear function
		clear(root);
	}

	//Define clear (overloaded function)
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	void AVL<T, V, comparison, equality>::clear(typename AVL<T, V, comparison, equality>::AVLNode *& pnode)
	{
		if (pnode != nullptr)
		{
			clear(pnode->lchild);
			clear(pnode->rchild);
			delete pnode;
			pnode = nullptr;
		}
	}

	//Define clear
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	void AVL<T, V, comparison, equality>::clear()
	{
		clear(root);
	}

	//four member functions (copy constructor, copy assignment, move constructor, move assignment)
	//Copy constructor
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	AVL <T, V, comparison, equality>::AVL(const AVL &obj) {
		//Start with blank
		root = nullptr;
		//Copy the tree
		root = copy(obj.root);

	}
	//Copy assignment operator
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	AVL<T, V, comparison, equality>& AVL <T, V, comparison, equality>::operator= (const AVL &obj) {
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
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	AVL<T, V, comparison, equality>::AVL(AVL &&obj) noexcept:
	root(nullptr)
	{
		//Copy the values from the source obejct
		root = obj.root;
		//leave obj in a destructible state
		obj.root = nullptr;
	}
	//Move assignment operator
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	AVL<T, V, comparison, equality>& AVL<T, V, comparison, equality>::operator=(AVL &&obj) noexcept
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

	//Create a copy function for copy constructor and copy assignemnt operator
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	typename AVL<T, V, comparison, equality>::AVLNode* AVL<T, V, comparison, equality>::copy(AVLNode* root) {
		AVLNode* newRoot = nullptr; //new root
		AVLNode* newLeft = nullptr; //new left child
		AVLNode* newRight = nullptr; //new right child 
								  //return nullptr if the root passed in is null
		if (!root)
		{
			return nullptr;
		}
		else
		{
			newLeft = copy(root->lchild); //copy the left subtree
			newRight = copy(root->rchild); //copy the right subtree
			newRoot = new AVLNode(root->key, root->value); //Build a new node to connect the left child and the right child to the root
			newRoot->lchild = newLeft;
			newRoot->rchild = newRight;
		}

		return newRoot;
	}

	//Define inOrder
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	void AVL<T, V, comparison, equality>::inOrder(typename AVL<T, V, comparison, equality>::AVLNode * pnode) const
	{
		if (pnode != nullptr)
		{
			inOrder(pnode->lchild);
			std::cout << pnode->key << std::endl;
			inOrder(pnode->rchild);
		}
	}

	//Call inOrder
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	void AVL<T, V, comparison, equality>::InOrder()
	{
		inOrder(root);
	}
	//return the larger one
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	int AVL<T, V, comparison, equality>::max(int a, int b)
	{
		return a > b ? a : b;
	}

	//Define RR_rotation (rotate left)
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	typename AVL<T, V, comparison, equality>::AVLNode*  AVL<T, V, comparison, equality>::RR_rotation(typename AVL<T, V, comparison, equality>::AVLNode * proot)
	{
		//Remember the original root's right child
		AVLNode* prchild = proot->rchild;
		//Make the original root adopt the original root's right child's left child as root's right child
		proot->rchild = prchild->lchild;
		//Make the original right child adopt the original root as its left child
		prchild->lchild = proot;

		//Update the height
		proot->height = max(height(proot->lchild), height(proot->rchild)) + 1;
		prchild->height = max(height(prchild->lchild), height(prchild->rchild)) + 1;

		//Return prchild to remember the original right child as the new root
		return prchild;
	}

	//Define LL_rotation (rotate left)
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	typename AVL<T, V, comparison, equality>::AVLNode* AVL<T, V, comparison, equality>::LL_rotation(typename AVL<T, V, comparison, equality>::AVLNode * proot)
	{
		//Remember the original root's left child
		AVLNode* plchild = proot->lchild;
		//Make the original root adopt the original root's left child's right child as root's new left child
		proot->lchild = plchild->rchild;
		//Make the original right child adopt the original root as its right child
		plchild->rchild = proot;

		//Update the height
		proot->height = max(height(proot->lchild), height(proot->rchild)) + 1;
		plchild->height = max(height(plchild->lchild), height(plchild->rchild)) + 1;

		return plchild;
	}

	//Define LR_rotation
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	typename AVL<T, V, comparison, equality>::AVLNode* AVL<T, V, comparison, equality>::LR_rotation(typename AVL<T, V, comparison, equality>::AVLNode * proot)
	{
		proot->lchild = RR_rotation(proot->lchild);
		return LL_rotation(proot);
	}

	//Define RL_rotation
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	typename AVL<T, V, comparison, equality>::AVLNode* AVL<T, V, comparison, equality>::RL_rotation(typename AVL<T, V, comparison, equality>::AVLNode * proot)
	{
		proot->rchild = LL_rotation(proot->rchild);
		return RR_rotation(proot);
	}

	//Define height
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	int AVL<T, V, comparison, equality>::height()
	{
		return height(root);
	}

	//Define height (overloaded function)
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	int AVL<T, V, comparison, equality>::height(typename AVL<T, V, comparison, equality>::AVLNode * pnode)
	{
		if (pnode != nullptr)
		{
			return pnode->height;
		}
		return -1;
	}

	//Define contains
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	bool AVL<T, V, comparison, equality>::contains(const T& key) {
		//When the tree is empty, no key will be found
		if (root == nullptr) {
			return false;
		}
		//Make curr root
		AVLNode* curr = root;
		while (true) {
			//When the current key is less than the user's key, go to the right subtree
			if ((comparison(curr->key, key))) {
				//return false when the right child is null
				if (curr->rchild == nullptr) {
					return false;
				}
				//If the right child is not null, make curr equal to its right child
				else {
					curr = curr->rchild;
				}
			}
			//When the current key is greater than the user's key, go to the left subtree
			else if (!(comparison(curr->key, key)) && !(equality(curr->key, key))) {
				//Print out an error message when the left child is null
				if (curr->lchild == nullptr) {
					return false;
				}
				//If the left child is not null, set curr to its left child
				else {
					curr = curr->lchild;
				}
			}
			//When two keys are equal, we exit the loop. We found the key.
			else {
				break;
			}
		}
		return true;
	}//End of contains

	 //Define checkKey
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	bool AVL<T, V, comparison, equality>::checkKey(const T& key, V value) {
		//When the tree is empty, no key will be found
		if (root == nullptr) {
			return false;
		}
		//Make curr root
		AVLNode* curr = root;
		while (true) {
			//When the current key is less than the user's key, go to the right subtree
			if ((comparison(curr->key, key))) {
				//return false when the right child is null
				if (curr->rchild == nullptr) {
					return false;
				}
				//If the right child is not null, make curr equal to its right child
				else {
					curr = curr->rchild;
				}
			}
			//When the current key is greater than the user's key, go to the left subtree
			else if (!(comparison(curr->key, key)) && !(equality(curr->key, key))) {
				//Print out an error message when the left child is null
				if (curr->lchild == nullptr) {
					return false;
				}
				//If the left child is not null, set curr to its left child
				else {
					curr = curr->lchild;
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

	 //Define insert (overloaded function)
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	typename AVL<T, V, comparison, equality>::AVLNode* AVL<T, V, comparison, equality>::insert(typename AVL<T, V, comparison, equality>::AVLNode * &pnode, const T& key, V value)
	{
		if (pnode == nullptr)	//Find the location to insert
		{
			pnode = new AVLNode(key, value, nullptr, nullptr);
		}
		//User's key is greater than the key of current node
		else if (comparison(pnode->key, key))
		{
			//Go to the right child of pnode (call recursively)
			pnode->rchild = insert(pnode->rchild, key, value);
			//Check the balance factor of the tree
			if (height(pnode->rchild) - height(pnode->lchild) == 2)
			{
				//Case 1: RR Rotation
				if (comparison(pnode->rchild->key, key))
					pnode = RR_rotation(pnode);
				//Case 3: RL Rotation
				else if (comparison(key, pnode->rchild->key))
					pnode = RL_rotation(pnode);
			}
		}
		//User's key is less than the key of current node
		else if (comparison(key, pnode->key))
		{
			//Go to the left child of pnode (call recursively)
			pnode->lchild = insert(pnode->lchild, key, value);
			//Check the balance factor of the tree
			if (height(pnode->lchild) - height(pnode->rchild) == 2)
			{
				//Case2: LL Rotation
				if (comparison(key, pnode->lchild->key))
					pnode = LL_rotation(pnode);
				else if (comparison(pnode->lchild->key, key))
					//Case 4: LR Rotation
					pnode = LR_rotation(pnode);
			}
		}
		//User's ley is equal to the key of current node. Update the value
		else if (equality(key, pnode->key)) {
			pnode->value = value;
		}
		//Update the height of pnode
		pnode->height = max(height(pnode->lchild), height(pnode->rchild)) + 1;
		return pnode;
	}

	//Define insert
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	void AVL<T, V, comparison, equality>::insert(const T& key, V value)
	{
		//Check if the key is already in the tree. If not, call insert. Otherwise, update the value in the checkKey function.
		if (!checkKey(key, value)) {
			insert(root, key, value);
		}
	}

	//Define lookup
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	V& AVL<T, V, comparison, equality>::lookup(const T& key) {
		//When the tree is empty
		if (root == nullptr) {
			throw std::runtime_error("The tree is empty.\n");
		}
		//Make curr root
		AVLNode* curr = root;
		while (true) {
			//When the current key is less than the user's key, go to the right subtree
			if ((comparison(curr->key, key))) {
				//Throw an error message when the right child is null
				if (curr->rchild == nullptr) {
					throw std::runtime_error("No value has been found.\n");
				}
				//If the right child is not null, make curr equal to its right child
				else {
					curr = curr->rchild;
				}
			}
			//When the current key is greater than the user's key, go to the left subtree
			else if (!(comparison(curr->key, key)) && !(equality(curr->key, key))) {
				//Print out an error message when the left child is null
				if (curr->lchild == nullptr) {
					throw std::runtime_error("No value has been found.\n");
				}
				//If the left child is not null, set curr to its left child
				else {
					curr = curr->lchild;
				}
			}
			//When two keys are equal, we exit the loop. We found the key
			else {
				break;
			}
		}
		return curr->value;
	}

	//Define remove
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	void AVL<T, V, comparison, equality>::remove(const T& key) {
		//Check if the key is in the tree
		if (contains(key))
			root = remove(root, key);
		else
			throw std::runtime_error("The node does not exist.\n");;
	}//end of remove function

	 //Define remove(overloaded function)
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	typename AVL<T, V, comparison, equality>::AVLNode* AVL<T, V, comparison, equality>::remove(typename AVL<T, V, comparison, equality>::AVLNode * &pnode, const T& key)
	{
		if (pnode != nullptr)
		{
			//When you find the node you are going to delete
			if (equality(key, pnode->key))
			{
				//If the node has two children
				if (pnode->lchild != nullptr&&pnode->rchild != nullptr)
				{
					// If left subtree is higher than right subtree
					if (height(pnode->lchild) > height(pnode->rchild))
					{
						//find the largest key in left subtree
						AVLNode* ppre = maximum(pnode->lchild);
						//replace the key of deleted node with the largest key in its left subtree
						pnode->key = ppre->key;
						//Replace value
						pnode->value = ppre->value;
						pnode->lchild = remove(pnode->lchild, ppre->key);  //resursively delete the the node with the largest key in left subtree
						pnode->height = max(height(pnode->lchild), height(pnode->rchild)) + 1;  //update the height, when resursive function returns
					}
					//If right subtree is higher than the left subtree
					else
					{

						AVLNode* psuc = minimum(pnode->rchild);    //Find the node with the minimum key in its right subtree
						pnode->key = psuc->key;
						pnode->value = psuc->value;
						pnode->rchild = remove(pnode->rchild, psuc->key);
						pnode->height = max(height(pnode->lchild), height(pnode->rchild)) + 1;  //update the height, when resursive function returns
					}

				}//End of the case that the node has two children
				else   //when the node you are going to delete only has one child or does not have any child
				{
					AVLNode* ptemp = pnode;
					if (pnode->lchild != nullptr)
					{
						pnode = pnode->lchild;
						delete ptemp;
						return pnode;
					}
					else if (pnode->rchild != nullptr)
					{
						pnode = pnode->rchild;
						delete ptemp;
						return pnode;
					}
					else
						return nullptr;
				}

			}//End of key is the same as pnode->key
			else if (comparison(pnode->key, key)) //key comes after pnode->key
			{
				pnode->rchild = remove(pnode->rchild, key); //call remove recursively
				pnode->height = max(height(pnode->lchild), height(pnode->rchild)) + 1; //update the height, when resursive function returns
				if (height(pnode->lchild) - height(pnode->rchild) == 2)  //Here we may delete a node of ponde's right subtree, which can only cause this unbalanced condition
				{
					//Do rotations
					if (height(pnode->lchild->rchild)>height(pnode->lchild->lchild))
						pnode = LR_rotation(pnode);
					else
						pnode = LL_rotation(pnode);
				}
			}
			//key comes before pnode->key
			else if (comparison(key, pnode->key))
			{
				pnode->lchild = remove(pnode->lchild, key);
				pnode->height = max(height(pnode->lchild), height(pnode->rchild)) + 1;  //update the height, when resursive function returns
				if (height(pnode->rchild) - height(pnode->lchild) == 2)             //Here we may delete a node of ponde left subtree, which can only cause this unbalanced condition
				{
					//Do roataions
					if (height(pnode->rchild->lchild)>height(pnode->rchild->rchild))
						pnode = RL_rotation(pnode);
					else
						pnode = RR_rotation(pnode);
				}
			}
			return pnode;
		}//End of removing the node
		else
		{
			throw std::runtime_error("The tree is empty.\n");
			return nullptr;
		}

	}//end of remove function (overloaded)

	 //Define maximum
	 //return the node with largest key
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	typename AVL<T, V, comparison, equality>::AVLNode* AVL<T, V, comparison, equality>::maximum(typename AVL<T, V, comparison, equality>::AVLNode* pnode)const
	{
		if (pnode != nullptr)
		{
			while (pnode->rchild != nullptr)
				pnode = pnode->rchild;
			return pnode;
		}
		return nullptr;
	}//End of maximum

	 //Define minimum
	 //return the node with the smallest key
	template <typename T, typename V, bool(*comparison)(const T&, const T&), bool(*equality)(const T&, const T&)>
	typename AVL<T, V, comparison, equality>::AVLNode* AVL<T, V, comparison, equality>::minimum(typename AVL<T, V, comparison, equality>::AVLNode * pnode)const
	{
		if (pnode != nullptr)
		{
			while (pnode->lchild != nullptr)
				pnode = pnode->lchild;
			return pnode;
		}
		return nullptr;
	}
}//End of namespace cop3530
#endif
