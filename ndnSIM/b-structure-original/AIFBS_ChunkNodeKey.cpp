/**
*
*	@description ChunkTree Key having our entity as payload
**/
#ifndef CHUNKTREE_NODE_KEY_DEF
#define CHUNKTREE_NODE_KEY_DEF
#include <string>
#include "AIFBS_NodeKeyBase.cpp"
namespace AIFBS {
	class AIFBS_ChunkNodeKey
	{
		std::string m_key;
		// entity cd;
		
		//all chunck playload can come here
	public:
		AIFBS_ChunkNodeKey() {
			m_key = "default-key";
		};
		~AIFBS_ChunkNodeKey() {

		};
		

		//Must to overload
		bool operator< (const AIFBS_ChunkNodeKey& other) {
			return m_key < other.m_key;
		}
		
		bool operator> (const AIFBS_ChunkNodeKey& other) {
			return m_key > other.m_key;
		}
		
		bool operator== (const AIFBS_ChunkNodeKey& other) {
			return m_key == other.m_key;
		}
		
		void Print( std::ostream& out) {
			out<<m_key;
		}

		friend std::ostream& operator<<( std::ostream& out, AIFBS_ChunkNodeKey& b )
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
#endif
