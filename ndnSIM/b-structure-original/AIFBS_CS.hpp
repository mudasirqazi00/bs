#include "AIFBS_BTreeNode.cpp"
#include "AIFBS_BTree.cpp"
#include "AIFBS_ChunkNodeKey.cpp"
#include "AIFBS_FileNodeKey.cpp"
#include <iostream>
#include <string>

namespace AIFBS
{

	struct SplittedNames
	{
		public:
			std::string m_fileName;
			std::string m_chunkName;
	};

	class AIFBS_CS
	{
		int m_fileT;
		int m_chunkT;

		AIFBS_BTree<AIFBS_FileNodeKey> *fileTree;
		SplittedNames split(std::string);
	public:
		AIFBS_CS() {
			m_fileT = 3;
			m_chunkT = 5;
			fileTree = new AIFBS_BTree<AIFBS_FileNodeKey>(m_fileT);
		}
		AIFBS_CS(int t_t) {
			m_fileT = t_t;
			m_chunkT = t_t;
			fileTree = new AIFBS_BTree<AIFBS_FileNodeKey>(m_fileT);
		}
		AIFBS_CS(int t_fileT, int t_chunkT) {
			m_fileT = t_fileT;
			m_chunkT = t_chunkT;
			fileTree = new AIFBS_BTree<AIFBS_FileNodeKey>(m_fileT);
		}
		~AIFBS_CS() {

		}

		void insert(std::string name, int payload);
		AIFBS_ChunkNodeKey* find(std::string t_name);

		void traverse() {
			if(fileTree == NULL)
				std::cout<<"Empty FileTree";
			else
				fileTree->traverse();
		}
		
		bool removeFile() {
			
		}
		
		bool remove(std::string chunkDetails);

		bool removeFile(std::string fileName);
	};
}
