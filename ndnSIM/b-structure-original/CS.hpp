#include "BTreeNode.cpp"
#include "BTree.cpp"
#include "ChunkNodeKey.cpp"
#include "FileNodeKey.cpp"
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

	class CS
	{
		int m_fileT;
		int m_chunkT;

		BTree<FileNodeKey> *fileTree;
		SplittedNames split(std::string);
	public:
		CS() {
			m_fileT = 3;
			m_chunkT = 5;
			fileTree = new BTree<FileNodeKey>(m_fileT);
		}
		CS(int t_t) {
			m_fileT = t_t;
			m_chunkT = t_t;
			fileTree = new BTree<FileNodeKey>(m_fileT);
		}
		CS(int t_fileT, int t_chunkT) {
			m_fileT = t_fileT;
			m_chunkT = t_chunkT;
			fileTree = new BTree<FileNodeKey>(m_fileT);
		}
		~CS() {

		}

		void insert(std::string name, int payload);
		ChunkNodeKey* find(std::string t_name);

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
