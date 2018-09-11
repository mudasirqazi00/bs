/**
*
*	@description ChunkTree Key having our entity as payload
**/
#include <string>
#include "AIFBS_ChunkNodeKey.cpp"
#include "AIFBS_BTree.hpp"
namespace AIFBS {
	class AIFBS_FileNodeKey
	{
		std::string m_key;
		// entity cd;
		//all chunck playload can come here
	public:
		AIFBS_BTree<AIFBS_ChunkNodeKey> *m_chunkTree;
		AIFBS_FileNodeKey() {
			m_key = "default-fault";
		};
		AIFBS_FileNodeKey(std::string t_key, AIFBS_BTree<AIFBS_ChunkNodeKey> *t_chunkTree) {
			m_key = t_key;
			m_chunkTree = t_chunkTree;
		};
		~AIFBS_FileNodeKey() {
			//delete(m_chunkTree);
		};		

		//Must to overload
		bool operator< (const AIFBS_FileNodeKey& other) {
			return m_key < other.m_key;
		}
		
		bool operator> (const AIFBS_FileNodeKey& other) {
			return m_key > other.m_key;
		}
		
		bool operator== (const AIFBS_FileNodeKey& other) {
			return m_key == other.m_key;
		}

		void Print( std::ostream& out) {
			out<<m_key<<": ";
			if(m_chunkTree == NULL) {
				out<<"ChunkTree is NULL";
			} else {
				m_chunkTree->traverse();
			}
		}

		friend std::ostream& operator<<( std::ostream& out, AIFBS_FileNodeKey& b )
	    {
	        b.Print( out );
	        return out;
	    }	
	    //Overloading Completed


	    //Getter & Setters
	    std::string getKey() {
	    	return m_key;
	    }
	    void setKey(std::string t_key) {
	    	m_key = t_key;
	    }

	};


}