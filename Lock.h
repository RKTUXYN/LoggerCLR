#pragma once
#ifndef _def
#include "Def.h"
#endif
#define __LOCK
namespace SOW {
	ref class Lock {
		Object^ m_pObject;
	public:
		Lock(Object ^ pObject);
		~Lock();
	};
}
