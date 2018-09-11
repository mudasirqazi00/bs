/**
*
*	@description ChunkTree Key having our entity as payload
**/
#ifndef CHUNKTREE_NODE_KEY_DEF
#define CHUNKTREE_NODE_KEY_DEF
#include <string>
#include "NodeKeyBase.cpp"
namespace AIFBS {
	class ChunkNodeKey
	{
		std::string m_key;
		// entity cd;
		
		//all chunck playload can come here
	public:
		ChunkNodeKey() {
			m_key = "default-key";
		};
		~ChunkNodeKey() {

		};
		

		//Must to overload
		bool operator< (const ChunkNodeKey& other) {
			return m_key < other.m_key;
		}
		
		bool operator> (const ChunkNodeKey& other) {
			return m_key > other.m_key;
		}
		
		bool operator== (const ChunkNodeKey& other) {
			return m_key == other.m_key;
		}
		
		void Print( std::ostream& out) {
			out<<m_key;
		}

		friend std::ostream& operator<<( std::ostream& out, ChunkNodeKey& b )
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
