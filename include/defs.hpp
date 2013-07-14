/**
* Copyright 2013 Marcelo Millani
*	This file is part of hidrasm.
*
* hidrasm is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* hidrasm is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with hidrasm.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef DEFS_HPP
#define DEFS_HPP

#define LOG210 1.301029995663981195
#define LOG102 0.30102999566398119801746702250966
#define LOG2(n) (log10((double)n)/LOG102)

#define ISWHITESPACE(c) ((c)==' ' || (c)=='\t')
#define ISEOL(c) ((c)=='\r' || (c)=='\n')

#define ISADDRESS(t) ((t)==TYPE_ADDRESS || (t)==TYPE_NUMBER || (t)==TYPE_LABEL)

#define SYMB_COMMENT ';'

#define REGISTER   'r'
#define OPERAND    'o'
#define ADDRESSING 'm'
#define ADDRESS    'a'
#define NUMBER     'n'
#define LABEL      'l'

typedef enum Exceptions
{
	eFileNotFound,
	eUnknownMnemonic,
	eInvalidFormat,
	eIncorrectOperands,
	eUnmatchedExpression,
	eOpenString,
	eRedefinedLabel,
	eUndefinedLabel,
	eInvalidExpressionVariable,
	eAddressingNotFound,
	eOperandNotFound

} e_exception;

#endif // DEFS_HPP

