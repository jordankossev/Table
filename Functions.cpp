#ifndef _FUNCTIONS_CPP
#define _FUNCTIONS_CPP

#include "Functions.h"
#include "Table.h"

void trimWhiteSpaces(std::string& str) {
	int i = 0, length = str.length();
	if (length != 0) {
		while (str[i] == ' ' && i < length) {
			i++;
		}
		str.erase(0, i);

		length = str.length();
		if (length != 0) {
			i = length - 1;
			while (str[i] == ' ' && i >= 0) {
				i--;
			}
			str.erase(i + 1, length - i);
		}
	}
}

bool isValidOperator(char ch) {
	if (ch == '+' || ch == '-' ||
		ch == '*' || ch == '/' || ch == '^') {
		return true;
	}
	return false;
}

bool isDigit(char ch) {
	if (ch >= '0' && ch <= '9') {
		return true;
	}
	return false;
}

bool isConvertibleToInt(const std::string& str) {
	size_t length = str.length(), i = 0;

	if (str[0] == '-' || str[0] == '+') {
		i = 1;
	}

	for (i; i < length; i++) {
		if (!isDigit(str[i])) {
			return false;
		}
	}
	return true;
}

bool isConvertibleToDouble(const std::string& str) {
	size_t length = str.length(), pointsCount = 0, i = 0;

	if (str[0] == '-' || str[0] == '+') {
		i = 1;
	}

	for (i; i < length; i++) {
		if (!isDigit(str[i])) {
			if (str[i] == '.') {
				pointsCount++;
			}
			else {
				return false;
			}
		}
	}

	if (pointsCount > 1) {
		return false;
	}
	return true;
}

bool isValidCellReference(const std::string& str) {
	size_t length = str.length();

	if (str[0] != 'R' || !isDigit(str[length - 1])) {
		return false;
	}

	size_t coutC = 0;
	if (isDigit(str[1])) {
		for (size_t i = 2; i < length; i++) {
			if (!isDigit(str[i])) {
				if (str[i] == 'C') {
					coutC++;
				}
				else {
					return false;
				}
			}
		}
	}
	else {
		return false;
	}

	if (coutC != 1) {
		return false;
	}

	return true;
}

std::tuple<size_t, size_t> retrieveCellReferenceIndices(const std::string& str) {
	size_t length = str.length(), i = 1;
	std::tuple<size_t, size_t> indices;
	std::string row, column;

	while (isDigit(str[i]) && i < length) {
		row.push_back(str[i]);
		i++;
	}
	std::get<0>(indices) = stoi(row);

	i++;
	while (isDigit(str[i]) && i < length) {
		column.push_back(str[i]);
		i++;
	}
	std::get<1>(indices) = stoi(column);

	return indices;
}

std::string performOperation(char operation, double leftOperand, double rightOperand) {
	double result = 0;
	switch (operation) {
		case '+':
			result = leftOperand + rightOperand;
			break;
		case '-':
			result = leftOperand - rightOperand;
			break;
		case '*':
			result = leftOperand * rightOperand;
			break;
		case '/':
			if (rightOperand == 0) {
				return "ERROR";
			}
			else {
				result = leftOperand / rightOperand;
				break;
			}
		case '^':
			result = pow(leftOperand, rightOperand);
			break;
		default:
			return "ERROR";
	}

	if (result == (int)result) {
		int intResult = (int)result;
		return std::to_string(intResult);
	}
	return std::to_string(result);
}

void displaySupportedCommands() {
	std::cout << "The app supports the following commands:\n";
	std::cout << std::setw(30) << std::left << "open <file>" << "open <file>\n";
	std::cout << std::setw(30) << std::left << "close" << "close the currently opened file\n";
	std::cout << std::setw(30) << std::left << "save" << "save changes to the currently opened file\n";
	std::cout << std::setw(30) << std::left << "saveas <file>" << "saves changes to <file>\n";
	std::cout << std::setw(30) << std::left << "print" << "print table's content\n";
	std::cout << std::setw(30) << std::left << "edit <row> <column> <content>" << "set the content of a cell with indices (<row>, <column>) to <content>\n";
	std::cout << std::setw(30) << std::left << "exit" << "exit the program\n";
}

std::tuple<size_t, size_t, std::string> retrieveEditInfo(const std::string& input) {
	size_t spacesCount = 0, inputLength = input.length();
	std::string rowStr, columnStr, content;
	for (size_t i = 0; i < inputLength; i++) {
		if (input[i] == ' ') {
			spacesCount++;
		}
		else {
			if (spacesCount == 0) {
				rowStr.push_back(input[i]);
			}
			else if (spacesCount == 1) {
				columnStr.push_back(input[i]);
			}
			else {
				content.push_back(input[i]);
			}
		}
	}

	size_t row = stoi(rowStr) - 1, column = stoi(columnStr) - 1;
	return std::make_tuple(row, column, content);
}

bool OpenFile(const std::string& fileName, Table& table) {
	std::ifstream file(fileName, std::ios::in | std::ios::beg);
	if (!file) {
		std::ofstream outFile(fileName, std::ios::out);
		outFile.close();
		file.open(fileName, std::ios::in | std::ios::beg);
	}
	assert(file);
	if (!table.loadFromFile(file)) {
		file.close();
		return false;
	}
	file.close();
	return true;
}

bool SaveInFile(const std::string& fileName, const Table& table) {
	std::ofstream file(fileName, std::ios::out | std::ios::beg);
	assert(file);
	if (!table.saveChangesInFile(file)) {
		file.close();
		return false;
	}
	file.close();
	return true;
}

#endif // !_FUNCTIONS_CPP