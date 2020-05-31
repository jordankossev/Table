#ifndef _TABLE_H
#define _TABLE_H

#include "Cell.h"

/// \brief Represents a table
class Table {
private:
	/// \brief 2-Dimensional array of cells
	Cell** data;
	size_t rows;
	size_t columns;

	void copyData(Cell**, size_t, size_t);
	void deleteData();
	/// \brief Evaluates an operand
	/// <param name="operand">Operand from a formula</param>
	/// <returns>The value of the operand as a string</returns>
	/// \details if \b operand's type is formula it is fist calculated by calculateFormula
	/// \sa calculateFormula()
	std::string evaluateOperand(const std::string&);
	/// \brief Evaluates all formulas
	/// \details Iterates through all cell. If a cell's type is a formula calculateFormula is called.
	void evaluateAllFormulas();
	/// \brief Calculates formula
	/// <param name="cell">Cell with a formula</param>
	/// <returns>The calculated result of the formula as a string</returns>
	/// \sa evaluateOperand()
	std::string calculateFormula(const Cell&);
	/// \brief Calculates the maximum width a cell has
	/// <returns>The maximum width a cell has</returns>
	size_t maxCellWidth() const;
public:
	Table();
	Table(Cell**, size_t, size_t);
	Table(const Table&);
	~Table();
	Table& operator=(const Table&);
	Cell* operator[](size_t);
	/// \brief Overloading \b operator[]
	/// <param name="">rowIndex</param>
	/// <returns>Pointer to row with index \b rowIndex</returns>
	const Cell* operator[](size_t) const;

	size_t getRows() const;
	size_t getColumns() const;

	/// \brief Loads table info from a file
	/// <param name="file">Input text file</param>
	/// <returns>
	/// \b *false*: if there is unknown cell type\n
	/// \b *true*: else
	/// </returns>
	/// \details First determines the rows and columns in the file matrix so thatthe cell matrix can be initialised.\n
	/// \details Then copies the information from the file and after that evaluates all fromulas
	/// \sa evaluateAllFormulas
	/// \attention The function raises the eof() flag.
	bool loadFromFile(std::ifstream&); // returns false if there is UNKNOWN type, else -> true;
	/// \brief Saves info from the cell matrix to the file in a formatted way
	/// <param name=""></param>
	/// <returns>
	/// \b *true*: if there is saving was successful\n
	/// \b *false*: else
	/// </returns>
	bool saveChangesInFile(std::ofstream&) const;

	void print() const;
};

#endif // !_TABLE_H