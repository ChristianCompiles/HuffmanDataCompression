#pragma once

class huffNode
{
public:

	int count = 0;
	char ch = '0';
	huffNode* parent = nullptr; // point to parent if applicable
	huffNode* prev = nullptr;
	huffNode* next = nullptr;
	huffNode* leftChild = nullptr;
	huffNode* rightChild = nullptr;

	huffNode() {};
	huffNode(char letter) { this->ch = letter; }
};