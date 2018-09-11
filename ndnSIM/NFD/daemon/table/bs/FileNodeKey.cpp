
#include <string>
#include "ChunkNodeKey.cpp"
#include "BTree.hpp"

namespace nfd {
	namespace cs {
		class FileNodeKey
		{
			std::string m_key;
			// entity cd;
			//all chunck playload can come here
		public:
			BTree<ChunkNodeKey> *m_chunkTree;
			FileNodeKey() {
				m_key = "default-fault";
				m_chunkTree = NULL;
			};
			FileNodeKey(std::string t_key, BTree<ChunkNodeKey> *t_chunkTree) {
				m_key = t_key;
				m_chunkTree = t_chunkTree;
			};
			~FileNodeKey() {
				//delete(m_chunkTree);
			};		

			//Must to overload
			bool operator< (const FileNodeKey& other) {
				return m_key < other.m_key;
			}
			
			bool operator> (const FileNodeKey& other) {
				return m_key > other.m_key;
			}
			
			bool operator== (const FileNodeKey& other) {
				return m_key == other.m_key;
			}

			void print( std::ostream& out) {
				out<<m_key<<": ";
				if(m_chunkTree == NULL) {
					out<<"ChunkTree is NULL";
				} else {
					m_chunkTree->traverse();
				}
			}

			friend std::ostream& operator<<( std::ostream& out, FileNodeKey& b )
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
		};
	}
}
