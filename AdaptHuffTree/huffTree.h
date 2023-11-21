#pragma once
#include "huffNode.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <bitset>
#include <cmath>


class huffTree
{
private:

	huffNode* root = nullptr; // root of the tree
	std::string alphabet = ""; // alphabet used in message
	std::vector <char> alphabetArray;
	std::string readFileName = ""; // file to read from
	huffNode* zeroNode = new huffNode; // store unused characters
	huffNode* hashTable[128] = { nullptr }; // has potential to hold all 128 ascii char
	std::string strToEncode = "";

	// private member functions
	void setUpAlphabetArray();
	huffNode* findLeader(int count, huffNode* node);
	void nodeSwap(huffNode* node1, huffNode* node2);
	void increment(huffNode* node);
	void calcPathToRootAndAppend(huffNode* node);
	void printCharAsBin(char tmpCh);
	void printTree();
	void printSubtree(huffNode* node, int level);
public:
	huffTree(){}
	huffTree(std::string passedInMsgAlphabet) { this->alphabet = passedInMsgAlphabet; setUpAlphabetArray(); }

	void encode(std::string messageToEncode)
	{
		// string encoded in lecture: aabccdaef 
		std::string binaryStr; // hold string of char ascii in binary
		char tmpCh = 0;
		int i = 0;

		while (i < messageToEncode.length())
		{
			tmpCh = messageToEncode[i];
		
			bool flag = 0;
			for (std::vector<char>::iterator it = alphabetArray.begin(); it != alphabetArray.end(); ++it)
			{
				if (tmpCh == *it)
				{
					flag = 1; // char was found in alphabetArray
					break;
				}
			}
			if (!flag) 
			{ 
				std::cout << "Char in message that is not in alphabet\n";
				return; 
			}

			printTree();
			
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
				
				binaryStr = std::bitset<8>(tmpCh).to_string();
				this->strToEncode += binaryStr; // add ch to string to encode 
				
				increment(charNode);
				i++;
				continue;
			}

			// logic for non-empty tree
			huffNode* foundNode = this->hashTable[tmpCh]; // get address of node containing that character

			if (foundNode) // node w/ char has address, so its been found in tree
			{
				calcPathToRootAndAppend(foundNode); // find path to root from char
				increment(foundNode); // performs parent increment and necessary remeditations
			}
			else // char not in tree
			{
				calcPathToRootAndAppend(this->zeroNode);
				binaryStr = std::bitset<8>(tmpCh).to_string();
				this->strToEncode += binaryStr; // add ch to string to encode 
				
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
			printTree();
		}

		std::cout << this->strToEncode << std::endl;
	}
	void decode(std::string messageToDecode)
	{
		std::string binaryStr; // hold string of char ascii in binary
		std::vector<char> eightBitVector;
		char tmpCh = 0;
		int i = 0;

		while (i < messageToDecode.length())
		{
			printTree();

			if (!root) // if tree empty
			{
				for (int j = 0; j < 8; j++)
				{
					eightBitVector.push_back(messageToDecode[j]);
					i++;
				}
				tmpCh = eightBitsToAscii(eightBitVector); // convert binary to int and store as char
				eightBitVector.clear();

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


				increment(charNode);
				i++;
				continue;
			}

			huffNode* node = this->root;
			if (node != nullptr)
			{
				while (node->leftChild != nullptr || node->rightChild != nullptr) // take in binary from message until we reach a leaf node
				{
					if (messageToDecode[i] == '0') // travel down left path
					{
						node = node->leftChild;
					}
					else // travel down right path
					{
						node = node->rightChild;
					}
					i++;
				}
				if (node->ch == '0') // if the node is the zero Node
				{
					for (int j = 0; j < 8; j++)
					{
						eightBitVector.push_back(messageToDecode[j]);
						i++;
					}
					tmpCh = eightBitsToAscii(eightBitVector); // convert binary to int and store as char
					
					eightBitVector.clear();

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
				else // node is char node
				{
					increment(node);
				}

			}
			std::cout << tmpCh;
			i++;			
		}
	}

	int eightBitsToAscii(std::vector<char> eightBitPath)
	{
		int total = 0;
		int count = 0;
		for (std::vector<char>::iterator it = eightBitPath.begin(); it != eightBitPath.end(); ++it)
		{
			if (*it == '0')
				continue;
			else
			{
				total += std::pow(2,8-count);
			}
			count++;
		}
		return total;
	}
};