#pragma once

#include <atomic>

class RefCounted
{
public:
	std::atomic<int> m_refCount;
#ifndef NDEBUG
	bool m_refCountDebug;
	std::string m_refCountDebugName;
#endif

public:
	RefCounted()
	{
		m_refCount = 1;
	}

	virtual ~RefCounted()
	{
		assert(m_refCount == 0);
	}

	virtual void AddRef()
	{
#ifndef NDEBUG
		if (m_refCountDebug) {
			printf("[Refcount Debug] %s ++ now: %d\n", m_refCountDebugName.c_str(), m_refCount + 1);
		}
#endif
		m_refCount++;
	}

	virtual void Release()
	{
#ifndef NDEBUG
		if (m_refCountDebug) {
			printf("[Refcount Debug] %s -- now: %d\n", m_refCountDebugName.c_str(), m_refCount - 1);
		}
#endif
		if (--m_refCount == 0) {
			delete this;
		}
	}

	virtual void ReleaseForce()
	{
#ifndef NDEBUG
		if (m_refCount > 1) {
			printf("WARNING: Forcing full Release of object at %p (refcount is %d)", this, (int)m_refCount);
		}
#endif
		m_refCount = 0;
		delete this;
	}
};
