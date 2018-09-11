#ifndef BTreeNode_def
#define	BTreeNode_def
	
namespace AIFBS {
	template <class T>
	class BTreeNode {
		T *keys;
		int t;	 // Minimum degree (defines the range for number of keys)
		BTreeNode **C; // An array of child pointers
		bool leaf; 
		int n;	 // Current number of keys

	public:
		BTreeNode(int _t, bool _leaf);
	
		void insertNonFull(T k);
	
		// A utility function to split the child y of this node. i is index of y in
		// child array C[]. The Child y must be full when this function is called
		void splitChild(int i, BTreeNode *y);
	
		void traverse();
	
		// A function to search a key in subtree rooted with this node.
		BTreeNode<T> *search(T k); // returns NULL if k is not present.
		
		T *searchKeyRef(T k);
		
		//BTree Deletion
		
	    // A function that returns the index of the first key that is greater
	    // or equal to k
	    int findKey(T k);
	    
		// A wrapper function to remove the key k in subtree rooted with
		// this node.
		void remove(T k);
	
		// A function to remove the key present in idx-th position in
		// this node which is a leaf
		void removeFromLeaf(int idx);
	
		// A function to remove the key present in idx-th position in
		// this node which is a non-leaf node
		void removeFromNonLeaf(int idx);
	
		// A function to get the predecessor of the key- where the key
		// is present in the idx-th position in the node
		T getPred(int idx);
	
		// A function to get the successor of the key- where the key
		// is present in the idx-th position in the node
		T getSucc(int idx);
	
		// A function to fill up the child node present in the idx-th
		// position in the C[] array if that child has less than t-1 keys
		void fill(int idx);
	
		// A function to borrow a key from the C[idx-1]-th node and place
		// it in C[idx]th node
		void borrowFromPrev(int idx);
	
		// A function to borrow a key from the C[idx+1]-th node and place it
		// in C[idx]th node
		void borrowFromNext(int idx);
	
		// A function to merge idx-th child of the node with (idx+1)th child of
		// the node
		void merge(int idx);
		
	
	template <class U>
	friend class BTree;
	friend class AIFBS_FileTree;
	friend class AIFBS_ChunkTree;
	};
}
#endif
