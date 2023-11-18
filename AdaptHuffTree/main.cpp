#include <iostream>
#include <string>
#include <fstream>
#include "huffTree.h"

int main(int argc, const char* argv[])
{
	//if (argc != 4) // make sure 4 command line statements are given
	//{
	//	std::cout << "Require 4 command line args total\n";
	//	return 1;
	//}
	//huffTree tree(argv[2], argv[3]); // pass in alphabet, name of file to encode

	huffTree tree;
	tree.encode();

	return 0;
}