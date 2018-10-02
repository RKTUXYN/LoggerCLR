#include "Logger.h"

void SOW::Logger::WriteToFile() {
	Stream^ fs;
	try {
		if (File::Exists(this->_physicalPath)) {
			fs = gcnew FileStream(this->_physicalPath, FileMode::Append, FileAccess::Write, FileShare::Read);
		}
		else {
			fs = gcnew FileStream(this->_physicalPath, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::ReadWrite);
		}
	} catch (_exp^ e) {
		Write(e->Message);
		Write(e->StackTrace);
		this->_physicalPath = _string::Concat(this->_physicalPath, "__", Guid::NewGuid().ToString("N"));
		Thread::Sleep(100);
		this->WriteToFile();
		return;
	}
	array<Byte>^ bytes = this->_ms->ToArray();
	fs->Write(bytes, 0, bytes->Length);
	this->_ms->Close();
	fs->Flush();
	this->_ms->Flush();
	this->_ms = nullptr;
}
SOW::Logger::Logger() {
	this->_closed = false;
	this->_iSUserInteractive = Environment::UserInteractive;
	this->_utf8 = gcnew UTF8Encoding;
}
SOW::Logger::Logger(_string^ physicalPath, _string^ dir) {
	this->_physicalPath = physicalPath;
	this->_closed = false;
	this->_dir = dir;
	this->_iSUserInteractive = Environment::UserInteractive;
	this->_utf8 = gcnew UTF8Encoding;
}

SOW::Logger^ SOW::Logger::Open(_string^ physicalPath) {
	if (_string::IsNullOrEmpty(this->_physicalPath) || _string::IsNullOrEmpty(this->_dir)) {
		if (!_string::IsNullOrEmpty(physicalPath)) {
			this->_physicalPath = physicalPath;
			this->_dir = Path::GetDirectoryName(physicalPath);
		}
	}
	if (_string::IsNullOrEmpty(this->_physicalPath) || _string::IsNullOrEmpty(this->_dir))
		throw gcnew ArgumentNullException("Physical path required");

	if (!_dire::Exists(this->_dir)) {
		_dire::CreateDirectory(this->_dir);
	}
	this->_ms = gcnew MemoryStream();
	return this;
}

SOW::Logger^ SOW::Logger::Open() {
	this->_dir = _string::Concat(Path::GetDirectoryName(System::Reflection::Assembly::GetEntryAssembly()->Location), "\\log\\");
	this->_physicalPath = _string::Concat(this->_dir, DateTime::Now.ToString("yyyy'-'MM'-'dd"), ".log");
	this->Open(this->_physicalPath);
	return this;
};
array<Byte>^ SOW::Logger::GetCurLog() {
	if (this->_ms == nullptr || this->_ms->CanRead == false) return nullptr;
	return _ms->ToArray();
};

void SOW::Logger::Write(array<Byte>^ buffer) {
	if (this->_ms == nullptr || !this->_ms->CanWrite || this->_closed) {
		Open(_physicalPath);
		this->_closed = false;
	}
	Lock lock(this->_ms);
	this->_ms->Write(buffer, 0, buffer->Length);
};

void SOW::Logger::Write(_string^ data) {
	this->Write(this->_utf8->GetBytes(_string::Concat(DateTime::Now.ToString(), "\t\t", data, "\r\n")));
};

void SOW::Logger::Close() {
	if (this->_closed) return;
	this->WriteToFile();
	this->_closed = true;
	GC::Collect();
}
SOW::Logger::~Logger() {
	if (this->_closed) return;
	this->WriteToFile();
	this->_closed = true;
	GC::Collect();
};
