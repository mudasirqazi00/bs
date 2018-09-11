#include "../src/ndnSIM/LogManager.cpp"
#include "BTreeNode.hpp"
#include "BTree.hpp"
#include <iostream>
using namespace std;

namespace nfd {
	namespace cs {

		template <class T>
		void BTree<T>::traverse() { 
			if (root != NULL) 
				root->traverse(); 
		}

		template <class T>
		int BTree<T>::size() {
			if (root != NULL){
//				LogManager::AddLog("BTree.cpp->size.root != NULL");
				return root->size();
			}
			else{
//				LogManager::AddLog("BTree.cpp->size.root == NULL");
				return 0;
			}
		}

		template <class T>
		BTreeNode<T>* BTree<T>::search(T k) { 
			return root == NULL ? NULL : root->search(k);
		}

		template <class T>
		T* BTree<T>::searchKeyRef(T k) {
			if(root != NULL){
//				LogManager::AddLog("btree.cpp->searchKeyRef.if");
				return root->searchKeyRef(k);
			}
			else{
//				LogManager::AddLog("btree.cpp->searchKeyRef.else");
				return NULL;
			}
		}

		template <class T>
		void BTree<T>::insert(T k) {
//			LogManager::AddLog("btree.cpp->insert");

			if (root == NULL) { // If tree is empty
//				LogManager::AddLog("btree.cpp->insert.if");
//				LogManager::AddLog("btree.cpp->insert.t", t);
				// Allocate memory for root
				root = new BTreeNode<T>(t, true);
				root->keys[0] = k; // Insert key
				root->n = 1; // Update number of keys in root
			}
			else // If tree is not empty
			{
//				LogManager::AddLog("btree.cpp->insert.else");
//				LogManager::AddLog("btree.cpp->insert.2*t-1=", 2*t-1);
				// If root is full, then tree grows in height
				if (root->n == 2*t-1)
				{
					// Allocate memory for new root
					BTreeNode<T> *s = new BTreeNode<T>(t, false);
		
					// Make old root as child of new root
					s->C[0] = root;
		
					// Split the old root and move 1 key to the new root
					s->splitChild(0, root);
		
					// New root has two children now. Decide which of the
					// two children is going to have new key
					int i = 0;
					if (s->keys[0] < k)
						i++;
					s->C[i]->insertNonFull(k);
		
					// Change root
					root = s;
				}
				else // If root is not full, call insertNonFull for root
					root->insertNonFull(k);
			}

		}

		template <class T>
		bool BTree<T>::isEmpty() {
			return root == NULL;
		}

		template <class T>
		void BTree<T>::remove(T k) {
			if (!root) {
				std::cout << "The tree is empty\n";
				return;
			}
		
			// Call the remove function for root
			root->remove(k);
		
			// If the root node has 0 keys, make its first child as the new root
			// if it has a child, otherwise set root as NULL
			if (root->n ==0 ) {
				BTreeNode<T> *tmp = root;
				if (root->leaf)
					root = NULL;
				else
					root = root->C[0];
		
				// Free the old root
				delete tmp;
			}
			return;
		}

		template <class T>
		void BTree<T>::deleteAll() {
			for (int i = 0; i < root->n; i++) {
				root->remove(root->keys[i]);
			}
		}
	}
}
