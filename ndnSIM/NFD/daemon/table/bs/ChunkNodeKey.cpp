
#ifndef CHUNKTREE_NODE_KEY_DEF
#define CHUNKTREE_NODE_KEY_DEF

#include <string>
#include "../cs-internal.hpp"
#include "NodeKeyBase.cpp"

namespace nfd {
	namespace cs {
		class ChunkNodeKey
		{
		private:
			std::string m_key;
			//const Name& name = Name("default-payload");
			//EntryImpl& temp;
			EntryImpl* m_payload;// = temp;
		public:
			ChunkNodeKey() {
				m_key = "default-key";
				// const Name& name = Name("default-payload");
				m_payload = NULL; //EntryImpl(name);
			};
			~ChunkNodeKey() { };
			
			//Overloading
			bool operator< (const ChunkNodeKey& other) {
				return m_key < other.m_key;
			}
			
			bool operator> (const ChunkNodeKey& other) {
				return m_key > other.m_key;
			}
			
			bool operator== (const ChunkNodeKey& other) {
				return m_key == other.m_key;
			}
			
			void print(std::ostream& out) {
				out<<m_key;
			}

			friend std::ostream& operator<<( std::ostream& out, ChunkNodeKey& b )
			{
				b.print( out );
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
			EntryImpl* getPayload(){
				return m_payload;
			}
			void setPayload(EntryImpl* entryImpl){
				m_payload = entryImpl;
			}
			//Getter & Setters Completed
		};
	}
}
#endif
