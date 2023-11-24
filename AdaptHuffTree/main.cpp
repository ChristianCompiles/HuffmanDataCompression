#include "huffTree.h"
#include <fstream>
#include <string>

int main(int argc, const char* argv[])
{
	//if (argc != 4) // make sure 4 command line statements are given
	//{
	//	std::cout << "Require 4 command line args total\n";
	//	return 1;
	//}

	// argv[0] is executable file name
	// argv[1] is action string (encode or decode)
	// argv[2] is name of file containing alphabet
	// argv[3] is name of file containing message to encode or decode

	
	 
	// debug test env
	std::string alphabetFileContent; // string to hold contents of file that has alphabet
	std::string messageFileContent;
	
	// read in alphabet from file
	std::getline(std::ifstream("alphabetfile.txt"), alphabetFileContent, '\0');
	std::cout << "Alphabet of message: " << alphabetFileContent << "\n";

	huffTree tree(alphabetFileContent); // constructor will take alphabet string as parameter
	// read in message to encode from file (txt file)
	/*std::getline(std::ifstream("filetoencode.txt"), messageFileContent, '\0');
	std::cout << "Message to encode: " << messageFileContent << "\n";
	tree.encode(messageFileContent);*/

	// read in message to decode from file (txt file)
	std::getline(std::ifstream("filetodecode.txt"), messageFileContent, '\0');
	std::cout << "Message to decode: " << messageFileContent << "\n";
	tree.decode(messageFileContent);
	
	
	
	//// cmd line env
	//if (argv[1] == "encode")
	//{
	//	tree.encode(messageFileContent);
	//}
	return 0;
}