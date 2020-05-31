#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <iostream>
#include <string>
#include <tuple>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cassert>

#include "Table.h"

/// \brief Trims white spaces at the beginning and the end of a string
/// <param name="str">String to be trimmed</param>
void trimWhiteSpaces(std::string&);

/// \brief Checks if a char is a valid arithmetic operator
/// <param name="ch">Char to be checked</param>
/// <returns>
/// \b *true*: if \b ch is a valid arithmetic operator\n
/// \b *false*: else
/// </returns>
bool isValidOperator(char);

/// \brief Checks if a char is a digit
/// <param name="ch">Char to be checked</param>
/// <returns>
/// \b *true*: if \b ch is a digit\n
/// \b *false*: else
/// </returns>
bool isDigit(char);

/// \brief Checks if string can be an integer value
/// <param name="str">String to be checked</param>
/// <returns>
/// \b *true*: if \b str is convertible to int\n
/// \b *false*: else
/// </returns>
bool isConvertibleToInt(const std::string&);

/// \brief Checks if string can be aa double value
/// <param name="str">String to be checked</param>
/// <returns>
/// \b *true*: if \b str is convertible to double\n
/// \b *false*: else
/// </returns>
bool isConvertibleToDouble(const std::string&);

/// \brief Checks if string adhere to the cell reference format
/// <param name="str">String to be checked</param>
/// <returns>
/// \b *true*: if \b str is a valid cell reference\n
/// \b *false*: else
/// </returns>
bool isValidCellReference(const std::string&);

/// \brief Retrieves cell indices from a cell reference
/// <param name="str">Cell reference</param>
/// <returns>
/// A std::tuple<size_t, size_t> containing the row and the column of the reference
/// </returns>
/// \attention \b str should be checked if it is a valid cell reference beforehand
/// \sa isValidCellReference
std::tuple<size_t, size_t> retrieveCellReferenceIndices(const std::string&);

/// \brief Performs an arithmetic operation with to operands
/// <param name="operation">Char representing a binary operator: +, -, *, / or ^</param>
/// <param name="leftOperand">double value</param>
/// <param name="rightOperand">double value</param>
/// <returns>
/// The result of the operation as a string.\n
/// If the operation violates the arithmetic rules the function returns "ERROR".
/// </returns>
std::string performOperation(char, double, double);

void displaySupportedCommands();

/// \brief Retrieves the indices of a cell to be edited from a string
/// <param name="input">String of the parameters of the 'edit' console command</param>
/// <returns>
/// A std::tuple<size_t, size_t> containing the row and the column of the cell
/// </returns>
std::tuple<size_t, size_t, std::string> retrieveEditInfo(const std::string&);

/// \brief Opens a file and loads the info into a table
/// <param name="fileName">Name of the input file</param>
/// <param name="table">Table that will contain the file info</param>
/// <returns>
/// \b *true*: if opening and loading were successful\n
/// \b *false*: else
/// </returns>
/// \attention If a file with \b fileName does not exist it will be created.
bool OpenFile(const std::string&, Table&);

/// \brief Saves info from a table into a file
/// <param name="fileName">Name of the output file</param>
/// <param name="table">Table the info of which will be saved</param>
/// <returns>
/// \b *true*: Saving was successful\n
/// \b *false*: else
/// </returns>
bool SaveInFile(const std::string&, const Table&);

#endif // !_FUNCTIONS_H