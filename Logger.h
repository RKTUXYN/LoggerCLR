#pragma once
#ifndef _def
#include "Def.h"
#endif
#ifndef  __LOCK
#include "Lock.h"
#endif
using namespace System;
using namespace System::IO;
using namespace System::Text;
#define __LOGGER
namespace SOW {
	public ref class Logger {
	private:
		MemoryStream^ _ms;
		_string^ _physicalPath;
		bool _closed = false;
		_string^ _dir;
		UTF8Encoding^ _utf8;
		bool _iSUserInteractive;
		void WriteToFile();
	public:
		Logger();
		Logger(_string^ physicalPath, _string^ dir);
		Logger^ Open(_string^ physicalPath);
		Logger^ Open();
		array<System::Byte>^ GetCurLog();
		void Write(array<System::Byte>^ buffer);
		void Write(_string^ data);
		void Close();
		~Logger();
	};
}