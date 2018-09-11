#include <iostream>
#include "BTreeNode.hpp"
#ifndef BTree_def
#define	BTree_def
	
namespace AIFBS {
	template<class T>
	class BTree {
		BTreeNode<T> *root; // Pointer to root node
		int t; // Minimum degree
	
	public:
		
		BTree<T> (int _t)
		{ 
			root = NULL; 
			t = _t; 
		}
	
		void traverse()
		{ 
			if (root != NULL) 
				root->traverse(); 
		}
	
		BTreeNode<T>* search(T k)
		{ 
			return (root == NULL)? NULL : root->search(k); 
		}
	
		T* searchKeyRef(T k)
		{ 
			return (root == NULL)? NULL : root->searchKeyRef(k); 
		}
	
		void insert(T k);

		// The main function that removes a new key in thie B-Tree
		void remove(T k);

		bool isEmpty() {
			return root == NULL;
		}
		void deleteAll() {
			for (int i = 0; i < root->n; i++) {
				root->remove(root->keys[i]);
			}
		}
	};
}
#endif
