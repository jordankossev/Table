# Spreadsheet App
A spredsheet is represented as a text file with each row of the file corresponding to a row of the spreadsheet.
The information in every row is separated by commas. Within the commas are the values of the cells.
Any white spaces next to the commas are ignored.


### Commands
The 'help' command prints all the supported commands.


### Supported data types
A value of a cell can be
  - an integer: 8, -23, 0
  - a decimal: 12.005, -0.236
  - a string: "Hello world!"
  - a formula


### Formulae
A formula starts with '=' and can include the operations '+', '-', '*', '^'.
The arguments can be integers, decimals or references to other cells.
R<x>C<y> is a reference to the cell in row <x> and column <y>.

NOTE 1: The indexation starts from 1.

NOTE 2: When calculating the formulae if an argument is a string that cannot be converted to a numeric value then the string is evaluated to 0.
