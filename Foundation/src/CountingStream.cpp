//
// CountingStream.cpp
//
// $Id: //poco/1.4/Foundation/src/CountingStream.cpp#1 $
//
// Library: Foundation
// Package: Streams
// Module:  CountingStream
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/CountingStream.h"


namespace Poco {


CountingStreamBuf::CountingStreamBuf(): 
	_pIstr(0), 
	_pOstr(0), 
	_chars(0), 
	_lines(0), 
	_pos(0)
{
}


CountingStreamBuf::CountingStreamBuf(std::istream& istr): 
	_pIstr(&istr), 
	_pOstr(0), 
	_chars(0), 
	_lines(0), 
	_pos(0)
{
}


CountingStreamBuf::CountingStreamBuf(std::ostream& ostr): 
	_pIstr(0), 
	_pOstr(&ostr), 
	_chars(0), 
	_lines(0), 
	_pos(0)
{
}


CountingStreamBuf::~CountingStreamBuf()
{
}


int CountingStreamBuf::readFromDevice()
{
	if (_pIstr)
	{
		int c = _pIstr->get();
		if (c != -1)
		{
			++_chars;
			if (_pos++ == 0) ++_lines;
			if (c == '\n') _pos = 0;
		}
		return c;
	}
	return -1;
}


int CountingStreamBuf::writeToDevice(char c)
{
	++_chars;
	if (_pos++ == 0) ++_lines;
	if (c == '\n') _pos = 0;
	if (_pOstr) _pOstr->put(c);
	return charToInt(c);
}


void CountingStreamBuf::reset()
{
	_chars = 0;
	_lines = 0;
	_pos   = 0;
}


void CountingStreamBuf::setCurrentLineNumber(int line)
{
	_lines = line;
}


void CountingStreamBuf::addChars(int chars)
{
	_chars += chars;
}

		
void CountingStreamBuf::addLines(int lines)
{
	_lines += lines;
}

		
void CountingStreamBuf::addPos(int pos)
{
	_pos += pos;
}


CountingIOS::CountingIOS()
{
	poco_ios_init(&_buf);
}


CountingIOS::CountingIOS(std::istream& istr): _buf(istr)
{
	poco_ios_init(&_buf);
}


CountingIOS::CountingIOS(std::ostream& ostr): _buf(ostr)
{
	poco_ios_init(&_buf);
}


CountingIOS::~CountingIOS()
{
}


void CountingIOS::reset()
{
	_buf.reset();
}


void CountingIOS::setCurrentLineNumber(int line)
{
	_buf.setCurrentLineNumber(line);
}


void CountingIOS::addChars(int chars)
{
	_buf.addChars(chars);
}

		
void CountingIOS::addLines(int lines)
{
	_buf.addLines(lines);
}

		
void CountingIOS::addPos(int pos)
{
	_buf.addPos(pos);
}


CountingStreamBuf* CountingIOS::rdbuf()
{
	return &_buf;
}


CountingInputStream::CountingInputStream(std::istream& istr): CountingIOS(istr), std::istream(&_buf)
{
}


CountingInputStream::~CountingInputStream()
{
}


CountingOutputStream::CountingOutputStream(): std::ostream(&_buf)
{
}


CountingOutputStream::CountingOutputStream(std::ostream& ostr): CountingIOS(ostr), std::ostream(&_buf)
{
}


CountingOutputStream::~CountingOutputStream()
{
}


} // namespace Poco
