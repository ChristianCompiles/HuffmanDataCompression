#include "huffNode.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#pragma once

class huffTree
{
private:

	huffNode* root = nullptr; // root of the tree
	std::string alphabet = ""; // alphabet used in message
	std::string readFileName = ""; // file to read from
	huffNode* zeroNode = nullptr; // store unused characters
	std::vector<int> vect;
	huffNode* hashTable[128] = { nullptr }; // has potential to hold all 128 ascii char

public:
	huffTree()
	{
		this->zeroNode = new huffNode;
		this->zeroNode->count = 0;
	}
	huffTree(std::string msgAlphabet, std::string sourcefile)
	{
		this->alphabet = msgAlphabet;
		this->readFileName = sourcefile;
		this->zeroNode = new huffNode;
		this->zeroNode->count = 0;
	}

	void encode()
	{/*this->readFileName = "input123";
		std::fstream source(readFileName, std::fstream::in);
		if (!source)
		{
			std::cout << "Error opening input file: " << readFileName;
		}*/

		// aabccdaef
		std::string bunchLetters = "aabccdaef"; // messes up at 'd' as of 11/09/23
		int i = 0;
		char tmpCh;

		while (i < bunchLetters.size())
		{
			tmpCh = bunchLetters[i];
	
			ultimatePrint();
			
			if (!root) // if tree empty
			{
				huffNode* parent = new huffNode;
				huffNode* charNode = new huffNode(tmpCh);

				parent->leftChild = this->zeroNode;
				parent->rightChild = charNode;

				this->zeroNode->parent = parent;
				this->zeroNode->prev = charNode;
				this->zeroNode->next = nullptr;

				this->root = parent;

				charNode->parent = this->root;
				charNode->next = zeroNode;
				charNode->prev = parent;

				this->hashTable[tmpCh] = charNode; // place address of char in tree into hashtable
				
				printCharAsBin(tmpCh);
				writeChar(tmpCh);
				increment(charNode);
				i++;
				continue;
			}

			// logic for non-empty tree
			huffNode* foundNode = this->hashTable[tmpCh]; // get address of node containing that character

			if (foundNode) // node w/ char has address, so its been found in tree
			{
				pathToRoot(foundNode); // find path to root from char
				writePath();
				printPathAndClear(); // print path rather than write to file
				increment(foundNode); // performs parent increment and necessary remeditations
			}
			else // char not in tree
			{
				// print path to zeroNode and ascii of char
				pathToRoot(this->zeroNode);
				writePath();
				printPathAndClear();
				writeChar(tmpCh);
				printCharAsBin(tmpCh);
				
				huffNode* newParent = new huffNode;
				huffNode* charNode = new huffNode(tmpCh);
				huffNode* zeroNodePrev = this->zeroNode->prev;
				huffNode* zeroNodeParent = this->zeroNode->parent;

				// newParent pointer adjustment
				newParent->leftChild = this->zeroNode;
				newParent->rightChild = charNode;
				newParent->prev = this->zeroNode->prev;
				newParent->next = charNode;
				newParent->parent = this->zeroNode->parent;

				zeroNodeParent->leftChild = newParent;

				zeroNodePrev->next = newParent;

				this->zeroNode->parent = newParent;
				this->zeroNode->prev = charNode;
				this->zeroNode->next = nullptr;

				charNode->parent = newParent;
				charNode->next = this->zeroNode;
				charNode->prev = newParent;

				this->hashTable[tmpCh] = charNode;

				increment(charNode);
			}
			
			i++;
			ultimatePrint();
		}
	}

	void increment(huffNode* node) // pass in node to increment its count, parent count, perform sibling check and necessary remediation
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
	huffNode* findLeader(int count, huffNode* node) // find leader with given count
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
	void nodeSwap(huffNode* node1, huffNode* node2)
	{
		// swap everything but children pointers
		std::swap(node1->ch, node2->ch);
		std::swap(node1->count, node2->count);
		//std::swap(node1->prev, node2->prev);
		//std::swap(node1->next, node2->next);
		//std::swap(node1->parent, node2->parent);
		std::swap(node1->leftChild, node2->leftChild);
		std::swap(node1->rightChild, node2->rightChild);
		if (node1->leftChild)
		{
			node1->leftChild->parent = node1;
		}
		if(node1->rightChild)
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
	void printCharAsBin(char tmpCh)
	{
		for (int i = 0; i < 8; i++) // convert ascii decimal to string of zeros and ones (binary)
		{
			std::cout << !!((tmpCh << i) & 0x80);
		}
		std::cout << " ";
	}
	void writeChar(char tmpCh)
	{
		std::string outputfilename = this->readFileName + ".encoded";
		std::ofstream outputfile;

		outputfile.open(outputfilename, std::ios::app | std::ios::binary | std::ios::out);

		if (!outputfile)
		{
			std::cout << "Error opening output file: " << outputfilename << std::endl;
			return;
		}

		std::string binaryStr = std::bitset<8>(tmpCh).to_string();
		outputfile.seekp(0, std::ios::end);
		outputfile.write(reinterpret_cast<char*>(&binaryStr), sizeof(binaryStr));
		outputfile.close();
		//for (int i = 0; i < 8; i++) // convert ascii decimal to string of zeros and ones (binary)
		//{
		//	outputfile << !!((tmpCh << i) & 0x80);
		//}
	}
	void writePath()
	{
		std::string outputfilename = this->readFileName + ".encoded";
		std::ofstream outputfile;

		outputfile.open(outputfilename, std::ios::binary | std::ios::app | std::ios::out);

		if (!outputfile)
		{
			std::cout << "Error opening output file: " << outputfilename << std::endl;
			return;
		}

		for (std::vector<int>::iterator it = vect.begin(); it != vect.end(); ++it) // print path to root
		{
			//outputfile.seekp(0, std::ios::end);
			outputfile.write(reinterpret_cast<char*>(&(*it)), sizeof(it));
			//outputfile << *it; // output path to file
		}
		if (!outputfile.good())
		{
			std::cout << "Error during writing\n";
		}
		outputfile.close();
	}
	void pathToRoot(huffNode* node) // start at node and find path back to root
	{
		vect.clear(); // in case not cleared before creating path
		while (node->parent != nullptr)
		{
			if (node->parent->leftChild == node) // if child is left child
			{
				this->vect.push_back(0);
			}
			else
			{
				this->vect.push_back(1);
			}
			node = node->parent;
		}

		std::reverse(this->vect.begin(), this->vect.end());
	}
	void printPathAndClear()
	{
		for (std::vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
			std::cout << *it;
		std::cout << " ";

		this->vect.clear();
	}
	void ultimatePrint()
	{
		std::cout << std::endl;
		//std::cout << std::endl;
		std::cout << "------------------------------------\n";
		printTree();
		std::cout << "------------------------------------\n";
		//std::cout << std::endl;
		//std::cout << std::endl;
	}
	void printTree() {
		printSubtree(root, 0);
	}
	void printSubtree(huffNode* node, int level)
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
		std::cout << node->ch << " " << node->count << std::endl;

		printSubtree(node->leftChild, level + 1);
	}
};