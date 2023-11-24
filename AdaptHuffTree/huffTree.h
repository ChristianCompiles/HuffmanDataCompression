#pragma once
#include "huffNode.h"
#include <iostream>
#include <vector>
#include <bitset>

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
	//std::string encodingWithSpaces;
	// private member functions
	void setUpAlphabetArray();
	huffNode* findLeader(int count, huffNode* node);
	void nodeSwap(huffNode* node1, huffNode* node2);
	void increment(huffNode* node);
	void calcPathToRootAndAppend(huffNode* node);
	int eightBitsToAscii(std::vector<char> eightBitPath);
	void printCharAsBin(char tmpCh);
	void writeStringtoBinaryFile();
	void printTree();
	void printSubtree(huffNode* node, int level);
public:
	huffTree(){}
	huffTree(std::string passedInMsgAlphabet) { this->alphabet = passedInMsgAlphabet; setUpAlphabetArray(); }
	void encode(std::string messageToEncode);
	void decode(std::string messageToDecode)
	{
		std::string binaryStr; // hold string of char ascii in binary
		std::vector<char> eightBitVector;
		char tmpCh = 0;
		std::string charMessage;
		int i = 0;

		while (i < messageToDecode.length())
		{
			printTree();

			if (!root) // if tree empty
			{
				for (int j = 0; j < 8; j++)
				{
					eightBitVector.push_back(messageToDecode[i]);
					i++;
				}
				tmpCh = eightBitsToAscii(eightBitVector); // convert binary to int and store as char
				std::cout << "Empty tree first char: " << tmpCh << std::endl;
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
				//i++;
				charMessage.push_back(tmpCh);
				std::cout << charMessage;
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
						eightBitVector.push_back(messageToDecode[i]);
						i++;
					}
					tmpCh = eightBitsToAscii(eightBitVector); // convert binary to int and store as char
					std::cout << "we are going to make a node with char: " << tmpCh << std::endl;
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
					tmpCh = node->ch;
					std::cout << "Char: " << tmpCh << " was found\n";

					increment(node);
				}

			}
			printTree();

			charMessage.push_back(tmpCh);
			std::cout << charMessage;
			//i++;			
		}
	}
	
};