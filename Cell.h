#ifndef _CELL_H
#define _CELL_H

#include <string>

#include "CellType.h"

/// \brief Represents a cell from the table
class Cell {
private:
	/// \brief Content of the cell
	std::string content;
	/// \brief Type of the cell
	Type type;

	/// \brief Determines cell's type
	/// \details Determines the type of the cell based on the content
	void determineType();
	/// \brief Copies the cell's data
	void copyData(const std::string&);
public:
	Cell();
	Cell(const std::string&);
	Cell(const Cell&);
	Cell& operator=(const Cell&);

	const std::string& getContent() const;
	Type getType() const;

	/// \brief Sets the cell's contents
	/// \details when the content is changed the type is re-evaluated
	void setContent(const std::string&);

	void print() const;
};

#endif // !_CELL_H