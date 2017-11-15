#pragma once

#define S2_USING_REF

namespace s2
{
	template<typename T>
	class ref
	{
	private:
		T* m_ptr;
		int* m_count;

	public:
		ref()
		{
			m_ptr = nullptr;
			m_count = nullptr;
		}

		ref(T* ptr)
		{
			m_ptr = ptr;
			if (ptr == nullptr) {
				m_count = nullptr;
			} else {
				m_count = new int(1);
			}
		}

		ref(const ref &copy)
		{
			m_ptr = copy.m_ptr;
			m_count = copy.m_count;
			addref();
		}

		~ref()
		{
			release();
		}

		int count() const
		{
			if (m_count == nullptr) {
				return 0;
			}
			return *m_count;
		}

		T* ptr()
		{
			return m_ptr;
		}

		const T* ptr() const
		{
			return m_ptr;
		}

		operator T*()
		{
			return m_ptr;
		}

		operator const T*() const
		{
			return m_ptr;
		}

		ref &operator =(T* o)
		{
			if (m_ptr != nullptr) {
				release();
			}
			m_ptr = o;
			if (o == nullptr) {
				m_count = nullptr;
			} else {
				m_count = new int(1);
			}
			return *this;
		}

		ref &operator =(const ref &other)
		{
			if (m_ptr != nullptr) {
				release();
			}
			m_ptr = other.m_ptr;
			m_count = other.m_count;
			addref();
			return *this;
		}

		T* operator ->()
		{
			return m_ptr;
		}

		const T* operator ->() const
		{
			return m_ptr;
		}

		T &operator *()
		{
			return *m_ptr;
		}

		const T &operator *() const
		{
			return *m_ptr;
		}

	private:
		void addref()
		{
			if (m_count == nullptr) {
				return;
			}

			(*m_count)++;
		}

		void release()
		{
			if (m_count == nullptr) {
				return;
			}

			if (--(*m_count) == 0) {
				delete m_ptr;
				delete m_count;
			}

			m_ptr = nullptr;
			m_count = nullptr;
		}
	};
}
