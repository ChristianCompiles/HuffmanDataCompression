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
	void decode(std::string messageToDecode);
};
