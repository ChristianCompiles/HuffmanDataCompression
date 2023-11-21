#include "huffTree.h"
#include <iostream>
#include <algorithm>

void huffTree::setUpAlphabetArray()
{
	std::cout << "Alphabet we are working with: " << this->alphabet << std::endl;

	for (int i = 0; i < this->alphabet.size(); i++)
	{
		if (alphabet[i] == 92) // if backslash encountered
		{
			i++; // look to next character
			switch (alphabet[i])
			{
			case 'n': alphabetArray.push_back('\n');
				break;
			case 't': alphabetArray.push_back('\t');
				break;
			case '\\': alphabetArray.push_back('\\');
				break;
			}
		}
		else
		{
			alphabetArray.push_back(alphabet[i]);
		}
	}
}

huffNode* huffTree::findLeader(int count, huffNode* node) // find leader with given count
{
	if (node) // make sure it's not a nullptr
	{
		while (node->prev != nullptr && node->prev->count == count)
		{
			node = node->prev;
		}
		return node;
	}
	else
		return nullptr;
}
void huffTree::nodeSwap(huffNode* node1, huffNode* node2)
{
	// swap everything but children pointers
	std::swap(node1->ch, node2->ch);
	std::swap(node1->count, node2->count);
	std::swap(node1->leftChild, node2->leftChild);
	std::swap(node1->rightChild, node2->rightChild);

	if (node1->leftChild)
	{
		node1->leftChild->parent = node1;
	}
	if (node1->rightChild)
	{
		node1->rightChild->parent = node1;
	}
	if (node2->leftChild)
	{
		node2->leftChild->parent = node2;

	}
	if (node2->rightChild)
	{
		node2->rightChild->parent = node2;
	}

	// update where table finds values 11/15/23 8:28p
	if (node1->ch != '0')
	{
		this->hashTable[node1->ch] = node1;
	}
	if (node2->ch != '0')
	{
		this->hashTable[node2->ch] = node2;
	}
}
void huffTree::increment(huffNode* node) // pass in node to increment its count, parent count, perform sibling check and necessary remediation
{
	int leaderCount = node->count; // leader has previous count value of node
	node->count++; // increment count of node passed in

	if (node->parent) // if parent node exists
	{
		if (node->prev) // prev node exists
		{
			if (node->count > node->prev->count) // if sibling check fails
			{
				huffNode* leader = findLeader(leaderCount, node);

				if (leader->leftChild != node && leader->rightChild != node) // make sure leader isn't parent
				{
					nodeSwap(node, leader); // swap node and leader
					node = leader;
					increment(node->parent); // increment parent of node after swap
				}
				else // leader is parent
				{
					increment(node->parent);
				}
			}
			else // sibling check did not fail
			{
				increment(node->parent);
			}
		}
	}
}
void huffTree::calcPathToRootAndAppend(huffNode* node) // start at node and find path back to root
{
	std::string path;
	while (node->parent != nullptr)
	{
		if (node->parent->leftChild == node) // if child is left child
		{
			path.push_back('0');
		}
		else
		{
			path.push_back('1');

		}
		node = node->parent;
	}

	reverse(path.begin(), path.end());
	this->strToEncode += path; // append path to strToEncode
}
void huffTree::printCharAsBin(char tmpCh)
{
	for (int i = 0; i < 8; i++) // convert ascii decimal to string of zeros and ones (binary)
	{
		std::cout << !!((tmpCh << i) & 0x80);
	}
	std::cout << " ";
}
// visualize tree functions
void huffTree::printTree()
{
	std::cout << std::endl;
	//std::cout << std::endl;
	std::cout << "------------------------------------\n";
	printSubtree(root, 0);
	std::cout << "------------------------------------\n";
}
void huffTree::printSubtree(huffNode* node, int level)
{
	if (node == nullptr)
	{
		return;
	}

	printSubtree(node->rightChild, level + 1);

	for (int i = 0; i < level; i++)
	{
		std::cout << "    ";
	}
	switch (node->ch)
	{
	case '\n': std::cout << "\\n" << " " << node->count << std::endl;
		break;
	case '\t': std::cout << "\\t" << " " << node->count << std::endl;
		break;
	case '\\': std::cout << "\\" << " " << node->count << std::endl;
		break;
	default: std::cout << node->ch << " " << node->count << std::endl;
	}
	
	printSubtree(node->leftChild, level + 1);
}