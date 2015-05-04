//
// 3.4. Towers of Hanoi using stacks
//

#include "stack.h"
#include <iostream>
#include <assert.h>

class tower 
{
public:

	explicit tower(size_t disks = 0)
	{
		for (size_t i = disks; i > 0; --i)
		{
			put_disk(i);
		}
	}

	void put_disk(int size) 
	{
		if (!m_stack.empty() && (m_stack.top() <= size)) {
			std::cerr << "Attempter to put disk size " << size << " on top of " << m_stack.top() << std::endl;
			throw std::invalid_argument("Disk can only be put on top of a larger disk");
		}

		m_stack.push(size);
	}

	int take_disk()
	{
		return m_stack.pop();
	}

	size_t total_disks() const
	{
		return m_stack.size();
	}

	bool is_empty() const 
	{
		return total_disks() == 0;
	}

	void move_top_to(tower& other)
	{
		other.put_disk(take_disk());
	}

	void move_all_to(tower& dest, tower& buffer)
	{
		move_disks(total_disks(), dest, buffer);
		assert(is_empty());
	}

protected:

	void move_disks(size_t n, tower& dest, tower& buffer)
	{
		if (n > 0) {
			// Move all but the last one to buffer
			move_disks(n - 1, buffer, dest);

			// Move the last one to destination
			move_top_to(dest);

			// Move all the other
			buffer.move_disks(n - 1, dest, *this);
		}
	}

private:

	stack<int> m_stack;
};

static void solve(size_t disks)
{
	tower a(disks);
	tower b;
	tower c;

	std::cout << "Solving for " << disks << std::endl;
	std::cout << "a = " << a.total_disks() << "; b = " << b.total_disks() << "; c = " << c.total_disks() << std::endl;

	a.move_all_to(c, b);

	std::cout << "a = " << a.total_disks() << "; b = " << b.total_disks() << "; c = " << c.total_disks() << std::endl;

	assert(a.is_empty());
	assert(c.total_disks() == disks);
}

int main(void)
{
	for (size_t i = 0; i <= 5; ++i) {
		solve(i);
	}

	return 0;
}
