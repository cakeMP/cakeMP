//
//  Octree.h
//
//  Created by Eduardo Poyart on 6/4/12.
//  Modifed by Melissa Cat on 26/2/17.
//

/*
Copyright (c) 2012, Eduardo Poyart.
Copyright (c) 2017, Melissa Cat.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <assert.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/vec3.hpp>

#define COMPUTE_SIDE(i, bit, p, mid, newMin, newMax) \
if (p >= mid) { \
	i |= bit; \
	newMin = mid; \
} else { \
	newMax = mid; \
}

template <class N>
class Octree
{
protected:
	struct OctreeNode
	{
		N _nodeData;
		OctreeNode* _children[8];
		OctreeNode()
		{
			for (int i = 0; i < 8; i++)
				_children[i] = nullptr;
		}
		virtual ~OctreeNode()
		{
			for (int i = 0; i < 8; i++)
			{
				if (_children[i])
					delete _children[i];
			}
		}
	};

	glm::vec3 _min;
	glm::vec3 _max;
	glm::vec3 _cellSize;
	OctreeNode* _root = nullptr;

public:
	Octree(const glm::vec3 &min, const glm::vec3 &max, const glm::vec3 &cellSize)
		: _min(min), _max(max), _cellSize(cellSize)
	{
	}

	virtual ~Octree()
	{
		if (_root)
			delete _root;
	}

	class Callback
	{
	public:
		// Return value: true = continue; false = abort.
		virtual bool operator()(const glm::vec3 &min, const glm::vec3 &max, N& nodeData) = 0;
	};

	N& getCell(const glm::vec3 &pos, Callback* callback = NULL)
	{
		assert(glm::all(glm::greaterThanEqual(pos, _min)) && glm::all(glm::lessThan(pos, _max)));
		glm::vec3 currMin = _min;
		glm::vec3 currMax = _max;
		glm::vec3 delta = _max - _min;
		if (!_root)
			_root = new OctreeNode();
		OctreeNode* currNode = _root;
		while (glm::all(glm::greaterThanEqual(delta, _cellSize)))
		{
			bool shouldContinue = true;
			if (callback)
				shouldContinue = callback->operator()(currMin, currMax, currNode->_nodeData);
			if (!shouldContinue)
				break;
			glm::vec3 mid = (delta * 0.5f) + currMin;
			glm::vec3 newMin(currMin);
			glm::vec3 newMax(currMax);
			int index = 0;
			COMPUTE_SIDE(index, 1, pos.x, mid.x, newMin.x, newMax.x)
			COMPUTE_SIDE(index, 2, pos.y, mid.y, newMin.y, newMax.y)
			COMPUTE_SIDE(index, 4, pos.z, mid.z, newMin.z, newMax.z)
			if (!(currNode->_children[index]))
				currNode->_children[index] = new OctreeNode();
			currNode = currNode->_children[index];
			currMin = newMin;
			currMax = newMax;
			delta = currMax - currMin;
		}
		return currNode->_nodeData;
	}

	void traverse(Callback &callback)
	{
		traverseRecursive(&callback, _min, _max, _root);
	}

	void clear()
	{
		delete _root;
		_root = NULL;
	}

	class Iterator
	{
	public:
		Iterator getChild(int i)
		{
			return Iterator(_currNode->_children[i]);
		}
		N* getData()
		{
			if (_currNode)
				return &_currNode->_nodeData;
			else return NULL;
		}
	protected:
		OctreeNode* _currNode;
		Iterator(OctreeNode* node): _currNode(node) {}
		friend class Octree;
	};

	Iterator getIterator()
	{
		return Iterator(_root);
	}

protected:
	void traverseRecursive(Callback* callback, const glm::vec3 &currMin, const glm::vec3 &currMax, OctreeNode* currNode)
	{
		if (!currNode)
			return;
		bool shouldContinue = callback->operator()(currMin, currMax, currNode->_nodeData);
		if (!shouldContinue)
			return;
		glm::vec3 delta = currMax - currMin;
		glm::vec3 mid = (delta * 0.5f) + currMin;
		traverseRecursive(callback, currMin, mid, currNode->_children[0]);
		traverseRecursive(callback, glm::vec3(mid.x, currMin.y, currMin.z),
																glm::vec3(currMax.x, mid.y, mid.z), currNode->_children[1]);
		traverseRecursive(callback, glm::vec3(currMin.x, mid.y, currMin.z),
																glm::vec3(mid.x, currMax.y, mid.z), currNode->_children[2]);
		traverseRecursive(callback, glm::vec3(mid.x, mid.y, currMin.z),
																glm::vec3(currMax.x, currMax.y, mid.z), currNode->_children[3]);
		traverseRecursive(callback, glm::vec3(currMin.x, currMin.y, mid.z),
																glm::vec3(mid.x, mid.y, currMax.z), currNode->_children[4]);
		traverseRecursive(callback, glm::vec3(mid.x, currMin.y, mid.z),
																glm::vec3(currMax.x, mid.y, currMax.z), currNode->_children[5]);
		traverseRecursive(callback, glm::vec3(currMin.x, mid.y, mid.z),
																glm::vec3(mid.x, currMax.y, currMax.z), currNode->_children[6]);
		traverseRecursive(callback, mid, currMax, currNode->_children[7]);
	}
};
