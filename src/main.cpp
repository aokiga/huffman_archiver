#include "huffman.h"
#include <string>
#include <string.h>
#include <stdexcept>
#include <iostream>

class ArgumentParser {
public:
	enum TYPE {
		NONE,
		ARCHIVE,
		EXTRACT
	} type;

	std::string input_file, output_file;

	ArgumentParser() : type(NONE), input_file(""), output_file("") {}

	void parse(int argc, char** argv) {
		for (int i = 1; i < argc; ++i) {

			if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "-u") == 0) {
				if (type == NONE) {
					if (strcmp(argv[i], "-c") == 0) type = ARCHIVE;
					else type = EXTRACT;
				}
				else
					throw std::invalid_argument("Error: You have to choose ONLY ONE type.");
				continue;
			}

			if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
				++i;
				if (input_file != "")
					throw std::invalid_argument("Error: You have to give ONLY ONE input file.");
				if (i == argc)
					throw std::invalid_argument("Error: You have to give an input file.");
				input_file = std::string(argv[i]);
				continue;
			}

			if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
				++i;
				if (output_file != "")
					throw std::invalid_argument("Error: You have to give ONLY ONE output file.");
				if (i == argc)
					throw std::invalid_argument("Error: You have to give an output file.");
				output_file = std::string(argv[i]);
				continue;
			}

			throw std::invalid_argument("Error: Invalid arguments.");
		}
		if (type == NONE)
			throw std::invalid_argument("Error: You have to choose the type.");
		if (input_file == "")
			throw std::invalid_argument("Error: You have to give an input file.");
		if (output_file == "")
			throw std::invalid_argument("Error: You have to give an output file.");
	}
};

int main(int argc, char** argv) {
	ArgumentParser parser;
	try {
		parser.parse(argc, argv);
	}
	catch (std::invalid_argument& exc) {
		std::cout << std::string(exc.what()) << '\n';
		return 1;
	}

	switch (parser.type) {
		case (ArgumentParser::ARCHIVE): {
			HuffmanArchiver archiver;
			archiver.encode_file(parser.input_file, parser.output_file);
			break;
		}
		case (ArgumentParser::EXTRACT): {
			HuffmanExtracter extracter;
			try {
				extracter.decode_file(parser.input_file, parser.output_file);
			}
			catch (std::exception &e) {
				std::cout << std::string(e.what()) << '\n';
				return 1;
			}
			break;
		}
		case (ArgumentParser::NONE):
			break;
	}
	return 0;
}