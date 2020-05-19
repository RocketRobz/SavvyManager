/*---------------------------------------------------------------------------------


Copyright (C) 2007 Acekard, www.acekard.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


---------------------------------------------------------------------------------*/

#include "stringtool.h"
#include <cstdarg>
#include <cstdio>
#include <vector>
#include <malloc.h>

std::u16string utf8_to_utf16(const std::string& utf8)
{
    std::vector<unsigned long> unicode;
    size_t i = 0;
    while (i < utf8.size())
    {
        unsigned long uni;
        size_t todo;
        unsigned char ch = utf8[i++];
        if (ch <= 0x7F)
        {
            uni = ch;
            todo = 0;
        }
        else if (ch <= 0xBF)
        {
            return NULL;
        }
        else if (ch <= 0xDF)
        {
            uni = ch&0x1F;
            todo = 1;
        }
        else if (ch <= 0xEF)
        {
            uni = ch&0x0F;
            todo = 2;
        }
        else if (ch <= 0xF7)
        {
            uni = ch&0x07;
            todo = 3;
        }
        else
        {
            return NULL;
        }
        for (size_t j = 0; j < todo; ++j)
        {
            if (i == utf8.size())
				return NULL;
            unsigned char ch = utf8[i++];
            if (ch < 0x80 || ch > 0xBF)
				return NULL;
            uni <<= 6;
            uni += ch & 0x3F;
        }
        if (uni >= 0xD800 && uni <= 0xDFFF)
            return NULL;
        if (uni > 0x10FFFF)
            return NULL;
        unicode.push_back(uni);
    }
    std::u16string utf16;
    for (size_t i = 0; i < unicode.size(); ++i)
    {
        unsigned long uni = unicode[i];
        if (uni <= 0xFFFF)
        {
            utf16 += (wchar_t)uni;
        }
        else
        {
            uni -= 0x10000;
            utf16 += (wchar_t)((uni >> 10) + 0xD800);
            utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
        }
    }
    return utf16;
}

std::u16string UTF8toUTF16(const char* src)
{
    return utf8_to_utf16(src);
}

std::string formatString( const char* fmt, ... )
{
    const char * f = fmt;
    va_list argList;
    va_start(argList, fmt);
    char * ptempStr = NULL;
    size_t max_len = vasiprintf( &ptempStr, f, argList);
    std::string str( ptempStr );
    str.resize( max_len );
    free( ptempStr );
    va_end(argList);
    return str;
}

std::string replaceAll(std::string str, const std::string &from, const std::string &to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}