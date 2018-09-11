#ifndef CS_DEF
#define CS_DEF

#include "CS.hpp"
namespace AIFBS
{
	void CS::insert(std::string t_name, int payload) {
		SplittedNames sn = split(t_name);
		FileNodeKey tempNode;
		tempNode.setKey(sn.m_fileName);

		FileNodeKey *fileNodeKey = fileTree->searchKeyRef(tempNode);

		if(fileNodeKey == NULL) {
			BTree<ChunkNodeKey> *chunkTree = new BTree<ChunkNodeKey>(m_chunkT);
			FileNodeKey newFileNodeKey(sn.m_fileName, chunkTree);
			fileTree->insert(newFileNodeKey);
			fileNodeKey = &newFileNodeKey;
		}

		ChunkNodeKey *chunkNodeKey = new ChunkNodeKey();
		chunkNodeKey->setKey(sn.m_chunkName);
		fileNodeKey->m_chunkTree->insert(*chunkNodeKey);
	}
	
	ChunkNodeKey* CS::find(std::string t_name){
		SplittedNames sn = split(t_name);
		FileNodeKey tempFileKey;
		tempFileKey.setKey(sn.m_fileName);
		FileNodeKey *fileNodeKey = fileTree->searchKeyRef(tempFileKey);
		if(fileNodeKey == NULL) {
			//required file does not exists and therefore chunks does not exists
			return NULL;
		}
		else {
			ChunkNodeKey tempChunkKey;
			tempChunkKey.setKey(sn.m_chunkName);
			ChunkNodeKey* requiredChunk = fileNodeKey->m_chunkTree->searchKeyRef(tempChunkKey);
			if(requiredChunk == NULL){
				//required chunk not found
				return NULL;
			}
			else{
				return requiredChunk;
			}
		}
	}

	SplittedNames CS::split(std::string name) {
		int index;
		for(int i = (name.size()-1); i>=0;i--) {
			if(name.at(i) == '/') {
				index = i;
				break;
			}
		}
		
		SplittedNames SN;
		SN.m_fileName = name.substr(0, index);
		SN.m_chunkName = name.substr(index+1, name.size()-1);

		return SN;
	}

	bool CS::remove(std::string chunkDetails) {
		SplittedNames sn = this->split(chunkDetails);

		FileNodeKey tempFileKey;
		tempFileKey.setKey(sn.m_fileName);
		FileNodeKey *fileNodeKey = fileTree->searchKeyRef(tempFileKey);

		if (fileNodeKey == NULL) {
			return false;
		}
		else {
			ChunkNodeKey tempChunkKey;
			tempChunkKey.setKey(sn.m_chunkName);
			ChunkNodeKey* requiredChunk = fileNodeKey->m_chunkTree->searchKeyRef(tempChunkKey);

			if (requiredChunk == NULL) {
				return false;
			}
			else {
				fileNodeKey->m_chunkTree->remove(*requiredChunk);
				if (fileNodeKey->m_chunkTree->isEmpty()) {
					fileTree->remove(*fileNodeKey);
				}
			}
		}
	}

	bool CS::removeFile(std::string fileName) {
		FileNodeKey tempFileKey;
		tempFileKey.setKey(fileName);
		FileNodeKey *fileNodeKey = fileTree->searchKeyRef(tempFileKey);

		if (fileNodeKey == NULL) {
			return false;
		}
		else {
			fileNodeKey->m_chunkTree->deleteAll();
		}
		fileTree->remove(*fileNodeKey);
	}
}
#endif
