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

	//std::string actionWord = argv[1];
	//std::string alphabetFileName = argv[2];
	//std::string sourceFile = argv[3];

	// read in alphabet from file
	std::string alphabetFileContent;
	std::getline(std::ifstream("alphabetfile.txt"), alphabetFileContent, '\0');
	std::cout << "Alphabet of message: " << alphabetFileContent << std::endl;

	////huffTree tree1(argv[1], argv[2], argv[3]);
	//huffTree tree3(actionWord, alphabetFileContent, sourceFile);
	//tree3.encode();

	// debug test env
	huffTree tree("encode", alphabetFileContent, "filetoencode.txt");
	return 0;
}