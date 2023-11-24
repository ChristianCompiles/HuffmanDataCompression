#pragma once
#include "huffTree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

void huffTree::encode(std::string messageToEncode)
{
	// string encoded in lecture: aabccdaef 
	std::string binaryStr; // hold string of char ascii in binary
	char tmpCh = 0;
	int i = 0;

	while (i < messageToEncode.length())
	{
		tmpCh = messageToEncode[i];
		std::cout << "Char to encode: " << tmpCh << std::endl;
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

			binaryStr = std::bitset<8>(tmpCh).to_string(); // represent the char in its ascii binary
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
	writeStringtoBinaryFile();
}
void huffTree::decode(std::string messageToDecode)
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
	}
}
void huffTree::writeStringtoBinaryFile()
{
	// package message into bytes to write to binary file
	std::ofstream writeFile(this->readFileName + ".encoded", std::ios::binary);
	// std::bitset<8> eightBits;

	int lenLastByte = strToEncode.length() % 8;
	std::cout << "\nlength of string to encode: " << strToEncode.length() << std::endl;
	std::cout << "length of last byte: " << lenLastByte << std::endl;
	std::string stringbinRepofLenLastByte = std::bitset<3>(lenLastByte).to_string();
	
	std::string lenPlusEncoding = stringbinRepofLenLastByte + strToEncode; // first 3 bits hold length of last byte
	std::cout << "len + encoding: " << lenPlusEncoding << std::endl;

	for (int i = 0; i < lenPlusEncoding.size(); i++) // iterate through entire encoding string
	{
		std::bitset<8> eightBits; // initialized to all zeros
		for (int j = 0; j < 8; j++) // package up encoding into bytes
		{
			if (i == lenPlusEncoding.length()) // if we have reached end of string
				break;

			if (lenPlusEncoding[i] == '0')
			{
				eightBits[j] = 0;
			}
			else
			{
				eightBits[j] = 1;
			}
			i++;
		}
		writeFile.write(reinterpret_cast<char*>(&eightBits), sizeof(eightBits));
	}
	writeFile.close();
}
int huffTree::eightBitsToAscii(std::vector<char> eightBitPath)
{
	int total = 0;
	int count = 0;
	for (std::vector<char>::iterator it = eightBitPath.begin(); it != eightBitPath.end(); ++it)
	{
		if (*it == '0')
		{
			count++;
			continue;
		}

		else
		{
			total += std::pow(2, 7 - count);
		}
		count++;
	}
	return total;
}
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
	std::string path = "";
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