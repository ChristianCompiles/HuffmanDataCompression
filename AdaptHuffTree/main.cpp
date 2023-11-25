#include "huffTree.h"
#include <fstream>
#include <string>

void printCharAsBin(char tmpCh)
{
	for (int i = 0; i < 8; i++) // convert ascii decimal to string of zeros and ones (binary)
	{
		std::cout << !!((tmpCh << i) & 0x80);
	}
	std::cout << " ";
}

int main(int argc, const char* argv[])
{
	int option = 1;
	std::string alphabetFileContent; // string to hold contents of file that has alphabet
	
	// cmd line env
	if (option == 1)
	{
		// argv[0] is executable file name
		// argv[1] is action string (encode or decode)
		// argv[2] is name of file containing alphabet
		// argv[3] is name of file containing message to encode or decode

		if (argc != 4) // make sure 4 command line statements are given
		{
			std::cout << "Require 4 command line args total\n";
			std::cout << "Executable | encode/decode | alphabetfile | messagefile\n";
			return 1;
		}

		std::string argvOne = argv[1];
		std::string argvTwo = argv[2];
		std::string argvThree = argv[3];

		for (int i = 0; i < 4; i++)
		{
			std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
		}
		
		// read in alphabet from file
		std::getline(std::ifstream(argvTwo), alphabetFileContent, '\0');
		std::cout << "Alphabet of message: " << alphabetFileContent << "\n";

		huffTree tree(argv[3], alphabetFileContent); // constructor will take alphabet string as parameter

		if (argvOne == "encode")
		{
			// read in message to encode from txt file
			std::string txtMsgFileContent;
			std::getline(std::ifstream(argvThree), txtMsgFileContent, '\0');
			std::cout << "Message to encode: " << txtMsgFileContent << "\n";

			tree.encode(txtMsgFileContent);
		}
		else if (argvOne == "decode")
		{
			std::ifstream inputFile(argvThree, std::ios::binary);
			std::string stringOfBinaryToDecode;
			char byte;
			
			while (inputFile.read(&byte, 1))
			{
				//std::cout << "Read byte: " << std::bitset<8>(byte).to_string() << std::endl;
				stringOfBinaryToDecode += std::bitset<8>(byte).to_string();
			}
			std::cout << "Message to decode: " << stringOfBinaryToDecode << std::endl;
			
			tree.decode(stringOfBinaryToDecode);
		}
		else
		{
			std::cout << "Please choose encode or decode\n";
		}
	}
	
	// debug test env
	//if (option == 2) // encode to binary file
	//{
	//	// read in alphabet from file
	//	std::getline(std::ifstream("alphabetfile.txt"), alphabetFileContent, '\0');
	//	std::cout << "Alphabet of message: " << alphabetFileContent << "\n";
	//	huffTree encodeTree(alphabetFileContent); // constructor will take alphabet string as parameter
	//	huffTree decodeTree(alphabetFileContent);

	//	// read in message to encode from file (txt file)
	//	std::string txtMsgFileContent;
	//	std::getline(std::ifstream("filetoencode.txt"), txtMsgFileContent, '\0');
	//	std::cout << "Message to encode: " << txtMsgFileContent << "\n";
	//	encodeTree.encode(txtMsgFileContent);
	//
	//	std::ifstream inputFile(".encoded", std::ios::binary);
	//	std::string stringOfBinaryToDecode;
	//	char byte;

	//	while (inputFile.read(&byte, 1)) 
	//	{
	//		// Process the byte as needed
	//		//std::cout << "Read byte: " << std::bitset<8>(byte).to_string() << std::endl;
	//		stringOfBinaryToDecode += std::bitset<8>(byte).to_string();
	//	}
	//	std::cout << "Message to decode: " << stringOfBinaryToDecode << std::endl;
	//	decodeTree.decode(stringOfBinaryToDecode);
	//}
	
	return 0;
}