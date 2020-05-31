#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

#include "Cell.h"
#include "Table.h"
#include "Functions.h"

const size_t COMMAND_MAX_LENGTH = 6; // "saveas" is the longest one - 6 chars

int main() {
	std::string input, command, value, loadedFileName;
	Table table;
	bool isProgramRunning = true, isInfoLoaded = false;

	while (isProgramRunning) {
		std::getline(std::cin, input, '\n');
		if (input.length() > COMMAND_MAX_LENGTH) {
			size_t i = 0;
			while (input[i] != ' ') { // separating the command and its parameters
				i++;
			}
			command = input.substr(0, i);
			value = input.substr(i + 1);

			if (!command.compare("open")) {
				loadedFileName = value;
				if (!OpenFile(loadedFileName, table)) return 0;
				isInfoLoaded = true;
			}
			else if (!command.compare("saveas") && isInfoLoaded) {
				if (SaveInFile(value, table)) { // value holds the new file's name
					std::cout << "Successfully saved at " << value << std::endl;
				}
				else {
					std::cout << "Error in saving file.\n";
				}
			}
			else if (!command.compare("edit") && isInfoLoaded) {
				std::tuple<size_t, size_t, std::string> editInfo = retrieveEditInfo(value);
				
				size_t row = std::get<0>(editInfo), column = std::get<1>(editInfo);
				std::string content = std::get<2>(editInfo);

				if (row >= table.getRows() || column >= table.getColumns()) {
					std::cout << "Rown or column index out of range\n";
				}
				else {
					table[row][column].setContent(content);
				}
			}
			else {
				if (!isInfoLoaded) {
					std::cout << "There is no info loaded. Try opening a file.\n";
				}
				else {
					std::cout << "Error: Unsupported command!\n";
				}
			}
		}
		else {
			command = input;
			if (!command.compare("close") && isInfoLoaded) {
				table.~Table();
				isInfoLoaded = false;
			}
			else if (!command.compare("save") && isInfoLoaded) {
				if (SaveInFile(loadedFileName, table)) {
					std::cout << "Successfully saved at " << loadedFileName << std::endl;
				}
				else {
					std::cout << "Error in saving file.\n";
				}
			}
			else if (!command.compare("print") && isInfoLoaded) {
				table.print();
			}
			else if (!command.compare("help")) {
				displaySupportedCommands();
			}
			else if (!command.compare("exit")) {
				isProgramRunning = false;
			}
			else {
				if (!isInfoLoaded) {
					std::cout << "There is no info loaded. Try opening a file.\n";
				}
				else {
					std::cout << "Error: Unsupported command!\n";
				}
			}
		}
	}
}