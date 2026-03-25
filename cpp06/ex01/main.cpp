#include "Serializer.hpp"
#include <iostream>

int main()
{
	Data data;
	data._num = 42;
	data._str = "hello";

	std::cout << "--- Creating Data struct ---" << std::endl;
	std::cout << "data._num = " << data._num << std::endl;
	std::cout << "data._str = " << data._str << std::endl;
	std::cout << "Address of data: " << &data << std::endl;
	std::cout << std::endl;

	std::cout << "--- Serializing (pointer -> uintptr_t) ---" << std::endl;
	uintptr_t raw = Serializer::serialize(&data);
	std::cout << "Result of serialize(&data): " << raw << std::endl;
	std::cout << std::endl;

	std::cout << "--- Deserializing (uintptr_t -> pointer) ---" << std::endl;
	Data *ptr = Serializer::deserialize(raw);
	std::cout << "Result of deserialize(raw): " << ptr << std::endl;
	std::cout << std::endl;

	std::cout << "--- Comparing pointers ---" << std::endl;
	std::cout << "Original address: " << &data << std::endl;
	std::cout << "Deserialized address: " << ptr << std::endl;
	if (ptr == &data)
		std::cout << "Pointers are equal: serialization works correctly" << std::endl;
	else
		std::cout << "Pointers differ: something went wrong" << std::endl;
	std::cout << std::endl;

	std::cout << "--- Accessing data through deserialized pointer ---" << std::endl;
	std::cout << "ptr->_num = " << ptr->_num << std::endl;
	std::cout << "ptr->_str = " << ptr->_str << std::endl;

	return (0);
}