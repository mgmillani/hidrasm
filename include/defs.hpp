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

#define LOG10 1.301029995663981195

#define ISWHITESPACE(c) ((c)==' ' || (c)=='\t')
#define ISEOL(c) ((c)=='\r' || (c)=='\n')

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
	eUndefinedLabel

} e_exception;

#endif // DEFS_HPP

