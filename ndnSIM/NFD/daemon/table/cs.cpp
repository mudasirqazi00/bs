/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2018,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cs.hpp"
#include "core/algorithm.hpp"
#include "core/asserts.hpp"
#include "core/logger.hpp"
#include <ndn-cxx/lp/tags.hpp>

namespace nfd {
namespace cs {

NFD_ASSERT_FORWARD_ITERATOR(Cs::const_iterator);

NFD_LOG_INIT("ContentStore");

unique_ptr<Policy> makeDefaultPolicy() {
	const std::string DEFAULT_POLICY = "priority_fifo";
	return Policy::create(DEFAULT_POLICY);
}

Cs::Cs(size_t nMaxPackets) :
		m_shouldAdmit(true), m_shouldServe(true) {
	this->setPolicyImpl(makeDefaultPolicy());
	m_policy->setLimit(nMaxPackets);

	//initializing b-structure members
	m_fileT = m_treeOrder;
	m_chunkT = m_treeOrder;
	m_csTree = new BTree<FileNodeKey>(m_treeOrder);
}

void Cs::insert(const Data& data, bool isUnsolicited) {

	LogManager::AddLog("cs.cpp->insert.start");
	LogManager::AddLog("cs.cpp->insert.name", data.getName().toUri());

	if (!m_shouldAdmit || m_policy->getLimit() == 0) {
		return;
	}

	NFD_LOG_DEBUG("insert " << data.getName());

	// recognize CachePolicy
	shared_ptr<lp::CachePolicyTag> tag = data.getTag<lp::CachePolicyTag>();
	if (tag != nullptr) {
		lp::CachePolicyType policy = tag->get().getPolicy();
		if (policy == lp::CachePolicyType::NO_CACHE) {
//			LogManager::AddLog("cs.cpp->insert.return.CachePolicy", "NO_CACHE");
			return;
		}
	}

	iterator it;
	bool isNewEntry = false;
	std::tie(it, isNewEntry) = m_table.emplace(data.shared_from_this(),
			isUnsolicited);
	EntryImpl& entry = const_cast<EntryImpl&>(*it);

	entry.updateStaleTime();

	if (!isNewEntry) { // existing entry
		// XXX This doesn't forbid unsolicited Data from refreshing a solicited entry.
		if (entry.isUnsolicited() && !isUnsolicited) {
			entry.unsetUnsolicited();
		}

		m_policy->afterRefresh(it);
	} else {
		m_policy->afterInsert(it);
	}

	std::string t_name = data.getName().toUri();
	bool alreadyExists = false;
	ChunkNodeKey* requiredChunk = findInCsTree(t_name);
	if (requiredChunk == NULL) {
		alreadyExists = true;
	}
//	LogManager::AddLog("cs.cpp->insert.findInCsTree.alreadyExists", alreadyExists);

	bool isReplaced = false;
	isReplaced = insertInCsTree(t_name, &entry);

//	LogManager::AddLog("cs.cpp->insert.isReplaced", isReplaced);
	LogManager::AddLog("cs.cpp->insert.completed");

	//Working code (before b-structure)
	/*
	 if (!m_shouldAdmit || m_policy->getLimit() == 0) {
	 return;
	 }
	 NFD_LOG_DEBUG("insert " << data.getName());

	 // recognize CachePolicy
	 shared_ptr<lp::CachePolicyTag> tag = data.getTag<lp::CachePolicyTag>();
	 if (tag != nullptr) {
	 lp::CachePolicyType policy = tag->get().getPolicy();
	 if (policy == lp::CachePolicyType::NO_CACHE) {
	 return;
	 }
	 }

	 iterator it;
	 bool isNewEntry = false;
	 std::tie(it, isNewEntry) = m_table.emplace(data.shared_from_this(), isUnsolicited);
	 EntryImpl& entry = const_cast<EntryImpl&>(*it);

	 entry.updateStaleTime();

	 if (!isNewEntry) { // existing entry
	 // XXX This doesn't forbid unsolicited Data from refreshing a solicited entry.
	 if (entry.isUnsolicited() && !isUnsolicited) {
	 entry.unsetUnsolicited();
	 }

	 m_policy->afterRefresh(it);
	 }
	 else {
	 m_policy->afterInsert(it);
	 }
	 */
}

void Cs::find(const Interest& interest, const HitCallback& hitCallback,
		const MissCallback& missCallback) const {
	BOOST_ASSERT(static_cast<bool>(hitCallback));
	BOOST_ASSERT(static_cast<bool>(missCallback));

	LogManager::AddLog("cs.cpp->find.start");
	LogManager::AddLog("cs.cpp->find.name", interest.getName().toUri());

	if (!m_shouldServe || m_policy->getLimit() == 0) {
		missCallback(interest);
//		LogManager::AddLog("cs.cpp->find.return.missCallback.1");
		return;
	}

	std::string t_name = interest.getName().toUri();
	ChunkNodeKey* requiredChunk = findInCsTree(t_name);
	if (requiredChunk == NULL) {
//		LogManager::AddLog("cs.cpp->find.return.missCallback");
		missCallback(interest);
		return;
	} else {
//		LogManager::AddLog("cs.cpp->find.else.1");
		const Name& prefix = interest.getName();
		bool isRightmost = interest.getChildSelector() == 1;
		NFD_LOG_DEBUG("find " << prefix << (isRightmost ? " R" : " L"));

		iterator first = m_table.lower_bound(prefix);
		iterator last = m_table.end();
		if (prefix.size() > 0) {
			last = m_table.lower_bound(prefix.getSuccessor());
		}

		iterator match = last;
		if (isRightmost) {
			match = this->findRightmost(interest, first, last);
		} else {
			match = this->findLeftmost(interest, first, last);
		}
//		LogManager::AddLog("cs.cpp->find.else.2");

//		LogManager::AddLog("cs.cpp->find.hitCallback");
		m_policy->beforeUse(match);
		EntryImpl* entry = requiredChunk->getPayload();
		const Data& data = entry->getData();
		hitCallback(interest, data);
	}

	//Working code (before b-structure)
	/*
	 BOOST_ASSERT(static_cast<bool>(hitCallback));
	 BOOST_ASSERT(static_cast<bool>(missCallback));

	 LogManager::AddLog("cs.cpp->find.start");
	 LogManager::AddLog("cs.cpp->find.name", interest.getName().toUri());

	 if (!m_shouldServe || m_policy->getLimit() == 0) {
	 missCallback(interest);
	 LogManager::AddLog("cs.cpp->find.return.missCallback.1");
	 return;
	 }

	 const Name& prefix = interest.getName();
	 bool isRightmost = interest.getChildSelector() == 1;
	 NFD_LOG_DEBUG("find " << prefix << (isRightmost ? " R" : " L"));

	 iterator first = m_table.lower_bound(prefix);
	 iterator last = m_table.end();
	 if (prefix.size() > 0) {
	 last = m_table.lower_bound(prefix.getSuccessor());
	 }

	 iterator match = last;
	 if (isRightmost) {
	 match = this->findRightmost(interest, first, last);
	 }
	 else {
	 match = this->findLeftmost(interest, first, last);
	 }

	 if (match == last) {
	 NFD_LOG_DEBUG("  no-match");
	 missCallback(interest);
	 LogManager::AddLog("cs.cpp->find.return.missCallback.2");
	 return;
	 }
	 NFD_LOG_DEBUG("  matching " << match->getName());
	 m_policy->beforeUse(match);
	 LogManager::AddLog("cs.cpp->find.hitCallback");
	 hitCallback(interest, match->getData());
	 LogManager::AddLog("cs.cpp->find.completed");
	 */
}

iterator Cs::findLeftmost(const Interest& interest, iterator first,
		iterator last) const {
	return std::find_if(first, last,
			bind(&cs::EntryImpl::canSatisfy, _1, interest));
}

iterator Cs::findRightmost(const Interest& interest, iterator first,
		iterator last) const {
	// Each loop visits a sub-namespace under a prefix one component longer than Interest Name.
	// If there is a match in that sub-namespace, the leftmost match is returned;
	// otherwise, loop continues.

	size_t interestNameLength = interest.getName().size();
	for (iterator right = last; right != first;) {
		iterator prev = std::prev(right);

		// special case: [first,prev] have exact Names
		if (prev->getName().size() == interestNameLength) {
			NFD_LOG_TRACE("  find-among-exact " << prev->getName());
			iterator matchExact = this->findRightmostAmongExact(interest, first,
					right);
			return matchExact == right ? last : matchExact;
		}

		Name prefix = prev->getName().getPrefix(interestNameLength + 1);
		iterator left = m_table.lower_bound(prefix);

		// normal case: [left,right) are under one-component-longer prefix
		NFD_LOG_TRACE("  find-under-prefix " << prefix);
		iterator match = this->findLeftmost(interest, left, right);
		if (match != right) {
			return match;
		}
		right = left;
	}
	return last;
}

iterator Cs::findRightmostAmongExact(const Interest& interest, iterator first,
		iterator last) const {
	return find_last_if(first, last, bind(&EntryImpl::canSatisfy, _1, interest));
}

void Cs::dump() {
	NFD_LOG_DEBUG("dump table");
	for (const EntryImpl& entry : m_table) {
		NFD_LOG_TRACE(entry.getFullName());
	}
}

void Cs::setPolicy(unique_ptr<Policy> policy) {
	BOOST_ASSERT(policy != nullptr);
	BOOST_ASSERT(m_policy != nullptr);
	size_t limit = m_policy->getLimit();
	this->setPolicyImpl(std::move(policy));
	m_policy->setLimit(limit);
}

void Cs::setPolicyImpl(unique_ptr<Policy> policy) {
	NFD_LOG_DEBUG("set-policy " << policy->getName());
	m_policy = std::move(policy);
	m_beforeEvictConnection = m_policy->beforeEvict.connect(
			[this] (iterator it) {
				m_table.erase(it);
			});

	m_policy->setCs(this);
	BOOST_ASSERT(m_policy->getCs() == this);
}

void Cs::enableAdmit(bool shouldAdmit) {
	if (m_shouldAdmit == shouldAdmit) {
		return;
	}
	m_shouldAdmit = shouldAdmit;
	NFD_LOG_INFO(
			(shouldAdmit ? "Enabling" : "Disabling") << " Data admittance");
}

void Cs::enableServe(bool shouldServe) {
	if (m_shouldServe == shouldServe) {
		return;
	}
	m_shouldServe = shouldServe;
	NFD_LOG_INFO((shouldServe ? "Enabling" : "Disabling") << " Data serving");
}

/*********************************************************************/
/************************* b-structure code **************************/
/*********************************************************************/
SplittedNames Cs::split(const std::string name) const {
	int index;
	for (unsigned int i = (unsigned int)(name.size() - 1); i >= 0; i--) {
		if (name.at(i) == '/') {
			index = i;
			break;
		}
	}

	SplittedNames sn;
	sn.m_fileName = name.substr(0, index);
	sn.m_chunkName = name.substr(index + 1, name.size() - 1);
	return sn;
}

SplittedNames Cs::splitit(const std::string name) const {
	int index;
	for (unsigned int i = 0; i <= (unsigned int)name.size() - 1; i++) {
		if (name.at(i) == '/' && i > 0) {
			index = i;
			break;
		}
	}

	SplittedNames sn;
	sn.m_fileName = name.substr(0, index);
	sn.m_chunkName = name.substr(index + 1, name.size() - 1);
	return sn;
}

int Cs::treeSize() {
	if (m_csTree == NULL) {
		return 0;
	} else {
		return m_csTree->size();
	}
}

ChunkNodeKey* Cs::findInCsTree(const std::string t_name) const {
	LogManager::AddLog("cs.cpp->findInCsTree.start");
	LogManager::AddLog("cs.cpp->findInCsTree.name", t_name);
	if (m_csTree == NULL) {
		return NULL;
	}
	SplittedNames sn = splitit(t_name);
	FileNodeKey tempFileKey;
	tempFileKey.setKey(sn.m_fileName);
	FileNodeKey *fileNodeKey = m_csTree->searchKeyRef(tempFileKey);
	if (fileNodeKey == NULL) {
		LogManager::AddLog("cs.cpp->findInCsTree.return");
		return NULL;
	} else {
		ChunkNodeKey tempChunkKey;
		tempChunkKey.setKey(sn.m_chunkName);
		ChunkNodeKey* requiredChunk = fileNodeKey->m_chunkTree->searchKeyRef(
				tempChunkKey);
		if (requiredChunk == NULL) {
			LogManager::AddLog("cs.cpp->findInCsTree.return");
			return NULL;
		} else {
			LogManager::AddLog("cs.cpp->findInCsTree.return.found", t_name);
			return requiredChunk;
		}
	}
}

bool Cs::insertInCsTree(std::string t_name, EntryImpl* entryImpl) {
	LogManager::AddLog("cs.cpp->insertInCsTree.start");
	LogManager::AddLog("cs.cpp->insertInCsTree.name", t_name);
	if (entryImpl == NULL) {
		LogManager::AddLog("cs.cpp->insertInCsTree.return");
		return false;
	}

	SplittedNames sn = splitit(t_name);
	FileNodeKey tempNode;
	tempNode.setKey(sn.m_fileName);
	FileNodeKey *fileNodeKey = m_csTree->searchKeyRef(tempNode);
	if (fileNodeKey == NULL) {
		BTree<ChunkNodeKey> *chunkTree = new BTree<ChunkNodeKey>(m_chunkT);
		FileNodeKey newFileNodeKey(sn.m_fileName, chunkTree);
		m_csTree->insert(newFileNodeKey);
		fileNodeKey = &newFileNodeKey;
		LogManager::AddLog("cs.cpp->insertInCsTree.return.true");
		return true;
	} else {
		ChunkNodeKey *chunkNodeKey = new ChunkNodeKey();
		chunkNodeKey->setKey(sn.m_chunkName);
		chunkNodeKey->setPayload(entryImpl);
		fileNodeKey->m_chunkTree->insert(*chunkNodeKey);
		LogManager::AddLog("cs.cpp->insertInCsTree.return.true");
		return true;
	}
}

} // namespace cs
} // namespace nfd
