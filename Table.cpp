#ifndef _TABLE_CPP
#define _TABLE_CPP

#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

#include "Table.h"
#include "Functions.h"

void Table::copyData(Cell** data, size_t rows, size_t columns) {
	this->data = new Cell * [rows];

	for (size_t i = 0; i < rows; i++) {
		this->data[i] = new Cell[columns];

		for (size_t j = 0; j < columns; j++) {
			this->data[i][j] = data[i][j];
		}
	}

	this->rows = rows;
	this->columns = columns;
}

void Table::deleteData() {
	for (size_t i = 0; i < this->rows; i++) {
		delete[] this->data[i];
	}
	delete[] this->data;
}

std::string Table::evaluateOperand(const std::string& operand) {
	if (operand.length() == 0) {
		return "0";
	}

	if (isConvertibleToDouble(operand)) {
		return operand;
	}

	if (isValidCellReference(operand)) {
		std::tuple<size_t, size_t> indices = retrieveCellReferenceIndices(operand);
		size_t row = std::get<0>(indices) - 1;		// for the user the indices start from 1
		size_t column = std::get<1>(indices) - 1;	// whereas in the code they star from 0

		if (row >= this->rows || column >= this->columns) {
			return "0";
		}
		else {
			Cell referredCell = this->data[row][column];
			std::string referredCellContent = referredCell.getContent();
			Type referredCellType = referredCell.getType();
			switch (referredCellType) {
				case Type::INTEGER: return referredCellContent;
				case Type::DOUBLE: return referredCellContent;
				case Type::STRING: { 
					// trim the quotation marks and evaluate the string between them
					size_t trimmedContentLength = referredCellContent.length() - 2;
					std::string trimmedContent = referredCellContent.substr(1, trimmedContentLength);
					return evaluateOperand(trimmedContent); 
				}
				case Type::FORMULA: {
					std::string calculatedContent = this->calculateFormula(referredCell);
					referredCell.setContent(calculatedContent);
					return calculatedContent;
				}
			}
		}
	}

	return "0";
}

std::string Table::calculateFormula(const Cell& cell) {
	std::string leftOperand, rightOperand, content = cell.getContent().substr(1); // getting the string after =
	double leftNumber, rightNumber;
	char operation = ' ';
	size_t contentLength = content.length(), i = 0;

	// separating the operands and the operator
	if (content[i] == '-' || content[i] == '+') { // checking if the first operand is signed
		leftOperand.push_back(content[i]);
		i++;
	}

	while (i < contentLength) {
		if (operation == ' ') {
			if (isValidOperator(content[i])) {
				operation = content[i];
			}
			else {
				leftOperand.push_back(content[i]);
			}
		}
		else {
			rightOperand.push_back(content[i]);
		}
		i++;
	}

	std::string result;
	trimWhiteSpaces(leftOperand);
	leftOperand = this->evaluateOperand(leftOperand);
	leftNumber = stod(leftOperand);

	if (operation == ' ') { // if this is true => there is only one operand
		result = leftOperand;
	}
	else {
		trimWhiteSpaces(rightOperand);
		rightOperand = this->evaluateOperand(rightOperand);
		rightNumber = stod(rightOperand);

		result = performOperation(operation, leftNumber, rightNumber);
	}

	return result;
}

void Table::evaluateAllFormulas() {
	for (size_t i = 0; i < this->rows; i++) {
		for (size_t j = 0; j < this->columns; j++) {
			if (this->data[i][j].getType() == Type::FORMULA) {
				std::string newContent = this->calculateFormula(this->data[i][j]);
				this->data[i][j].setContent(newContent);
			}
		}
	}
}

size_t Table::maxCellWidth() const {
	size_t maxWidth = 0;
	for (size_t i = 0; i < this->rows; i++) {
		for (size_t j = 0; j < this->columns; j++) {
			size_t currenCelltWidth = this->data[i][j].getContent().length();
			if (maxWidth < currenCelltWidth) {
				maxWidth = currenCelltWidth;
			}
		}
	}
	return maxWidth;
}

Table::Table() {
	this->data = nullptr;
	this->rows = 0;
	this->columns = 0;
}

Table::Table(Cell** data, size_t rows, size_t columns) {
	this->copyData(data, rows, columns);
}

Table::Table(const Table& other) {
	this->copyData(other.data, other.rows, other.columns);
}

Table::~Table() {
	this->deleteData();
}

Table& Table::operator=(const Table& other) {
	assert(this != &other);

	this->deleteData();
	this->copyData(other.data, other.rows, other.columns);

	return *this;
}

Cell* Table::operator[](size_t rowIndex) {
	assert(rowIndex >= 0 && rowIndex < this->rows);
	return this->data[rowIndex];
}

const Cell* Table::operator[](size_t rowIndex) const {
	assert(rowIndex >= 0 && rowIndex < this->rows);
	return this->data[rowIndex];
}

size_t Table::getRows() const {
	return this->rows;
}

size_t Table::getColumns() const {
	return this->columns;
}

bool Table::loadFromFile(std::ifstream& file) {
	size_t rows = 0, maxColumns = 0;
	std::string inputLine;

	// determining the number of rows and columns
	while (std::getline(file, inputLine, '\n')) {
		size_t length = inputLine.length(), currentColumns = 1;	// columns = commas + 1
		
		for (size_t i = 0; i < length; i++) {
			if (inputLine[i] == ',') currentColumns++;
		}

		if (maxColumns < currentColumns) maxColumns = currentColumns;
		rows++;
	}
	file.clear();

	// initialising the data matrix
	this->rows = rows;
	this->columns = maxColumns;
	this->data = new Cell * [this->rows];
	for (size_t i = 0; i < this->rows; i++) {
		this->data[i] = new Cell[this->columns];
	}
	
	// copying the info from the file into the data matrix
	file.seekg(0, std::ios::beg);
	size_t i = 0;
	while (std::getline(file, inputLine, '\n') && i < this->rows) {
		size_t j = 0, cellIndex = 0, length = inputLine.length();
		std::string content;
		while (j <= length) {
			if (j == length || inputLine[j] == ',') {
				trimWhiteSpaces(content);
				this->data[i][cellIndex] = Cell(content);
				
				// checking for an unknown type
				if (this->data[i][cellIndex].getType() == Type::UNKNOWN) {
					std::cout << "Error: row " << i + 1 << " column " << cellIndex + 1 << " ";
					std::cout << this->data[i][cellIndex].getContent() << " is unknown data type\n";
					return false;
				}

				cellIndex++;
				content = "";
			}
			else {
				content.push_back(inputLine[j]);
			}
			j++;
		}
		i++;
	}

	this->evaluateAllFormulas();
	return true;
}

bool Table::saveChangesInFile(std::ofstream& file) const {
	for (size_t i = 0; i < this->rows; i++) {
		for (size_t j = 0; j < this->columns; j++) {
			if (j == this->columns - 1) {
				if (!(file << this->data[i][j].getContent() << std::endl)) {
					return false;
				}
			}
			else {
				if (!(file << this->data[i][j].getContent() << ", ")) {
					return false;
				}
			}
		}
	}
	return true;
}

void Table::print() const {
	size_t cellWidth = this->maxCellWidth();

	for (size_t i = 0; i < this->rows; i++) {
		std::cout << "|";
		for (size_t j = 0; j < this->columns; j++) {
			if (j == this->columns - 1) {
				std::cout << std::setw(cellWidth) << std::left << this->data[i][j].getContent() << std::endl;
			}
			else {
				std::cout << std::setw(cellWidth) << std::left << this->data[i][j].getContent() << "|";
			}
		}
	}
}

#endif // !_TABLE_CPP