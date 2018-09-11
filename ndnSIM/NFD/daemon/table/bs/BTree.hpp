
#ifndef bstructure_btree_hpp
#define	bstructure_btree_hpp

#include "BTreeNode.hpp"
#include "BTreeNode.cpp"
#include <iostream>
using namespace std;

namespace nfd {
	namespace cs {
		template<class T>
		class BTree {
		private:
			BTreeNode<T> *root;
			int t; //degree
		
		public:
			BTree<T> (int _t){
				//LogManager::AddLog("btree.hpp->btree.t", _t);
				t = _t;
				root = new BTreeNode<T>(t, true);
			}
			void traverse();
			BTreeNode<T>* search(T k);
			T* searchKeyRef(T k);
			void insert(T k);
			void remove(T k);
			bool isEmpty();
			void deleteAll();
			int size();
		};
	}
}
#endif
