#include "Lock.h"
namespace SOW {
	Lock::Lock(Object ^ pObject) : m_pObject(pObject) {
		_th::Monitor::Enter(m_pObject);
	};
	Lock::~Lock() {
		_th::Monitor::Exit(m_pObject);
	};
}