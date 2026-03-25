#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"

#include <cstdlib>
#include <iostream>
#include <ctime>

static Base *generate(void)
{
	int i = rand() % 3;
	if (i == 0)
		return (new A);
	else if (i == 1)
		return (new B);
	else
		return (new C);
}

static void identify(Base *p)
{
	if (dynamic_cast<A*>(p))
		std::cout << "A" << std::endl;
	else if (dynamic_cast<B*>(p))
		std::cout << "B" << std::endl;
	else if (dynamic_cast<C*>(p))
		std::cout << "C" << std::endl;
}

static void identify(Base &p)
{
	try
	{
		A &a = dynamic_cast<A&>(p);
		(void)a;
		std::cout << "A" << std::endl;
		return;
	}
	catch (std::exception &e)
	{
	}
	try
	{
		B &b = dynamic_cast<B&>(p);
		(void)b;
		std::cout << "B" << std::endl;
		return;
	}
	catch (std::exception &e)
	{
	}
	try
	{
		C &c = dynamic_cast<C&>(p);
		(void)c;
		std::cout << "C" << std::endl;
		return;
	}
	catch (std::exception &e)
	{
	}
}

int main()
{
	std::srand(std::time(NULL));

	std::cout << "--- Generating 5 random objects ---" << std::endl;
	for (int i = 0; i < 5; i++)
	{
		Base *obj = generate();
		std::cout << "Object " << i + 1 << ": ";
		std::cout << "pointer: ";
		identify(obj);
		std::cout << "Object " << i + 1 << ": ";
		std::cout << "reference: ";
		identify(*obj);
		delete obj;
		std::cout << std::endl;
	}
	return (0);
}