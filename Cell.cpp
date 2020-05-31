#ifndef _CELL_CPP
#define _CELL_CPP

#include <iostream>
#include <cassert>

#include "Cell.h"
#include "Functions.h"

void Cell::determineType() {
	if (this->content.length() == 0) {
		this->type = Type::EMPTY;
		return;
	}

	if (this->content[0] == '=') {
		this->type = Type::FORMULA;
		return;
	}

	if (this->content.front() == '\"' && this->content.back() == '\"') {
		this->type = Type::STRING;
		return;
	}

	if (isConvertibleToInt(this->content)) {
		this->type = Type::INTEGER;
		return;
	}

	if (isConvertibleToDouble(this->content)) {
		this->type = Type::DOUBLE;
		return;
	}

	this->type = Type::UNKNOWN;
	return;
}

void Cell::copyData(const std::string& content) {
	this->content = content;
	this->determineType();
}

Cell::Cell() {
	this->content = "";
	this->type = Type::EMPTY;
}

Cell::Cell(const std::string& content) {
	this->copyData(content);
}

Cell::Cell(const Cell& other) {
	this->copyData(other.content);
}

Cell& Cell::operator=(const Cell& other) {
	assert(this != &other);

	this->copyData(other.content);
	return *this;
}

const std::string& Cell::getContent() const {
	return this->content;
}

Type Cell::getType() const {
	return this->type;
}

void Cell::setContent(const std::string& content) {
	this->content = content;
	this->determineType();

	//if (this->type == Type::UNKNOWN) {
	//	std::cout << this->content << " is an unknown data type!\n";
	//}
}

void Cell::print() const {
	std::cout << "Content: " << this->content << std::endl;
	std::cout << "Type: " << (int)this->type << std::endl;
}

#endif // !_CELL_CPP