#include <iostream>
#include <string>
#include "Array.hpp"

int main(void)
{
    std::cout << "--- Array vacio ---" << std::endl;
    Array<int> empty;
    std::cout << "Size: " << empty.size() << std::endl;

    std::cout << std::endl << "--- Array de 5 ints ---" << std::endl;
    Array<int> nums(5);
    for (size_t i = 0; i < nums.size(); i++)
        std::cout << "nums[" << i << "] = " << nums[i] << std::endl;

    std::cout << std::endl << "--- Modificar elementos ---" << std::endl;
    for (size_t i = 0; i < nums.size(); i++)
        nums[i] = i * 10;
    for (size_t i = 0; i < nums.size(); i++)
        std::cout << "nums[" << i << "] = " << nums[i] << std::endl;

    std::cout << std::endl << "--- Copia profunda ---" << std::endl;
    Array<int> copy(nums);
    copy[0] = 999;
    std::cout << "copy[0] = " << copy[0] << std::endl;
    std::cout << "nums[0] = " << nums[0] << std::endl;

    std::cout << std::endl << "--- Asignacion ---" << std::endl;
    Array<int> assigned;
    assigned = nums;
    assigned[1] = 777;
    std::cout << "assigned[1] = " << assigned[1] << std::endl;
    std::cout << "nums[1] = " << nums[1] << std::endl;

    std::cout << std::endl << "--- Indice fuera de rango ---" << std::endl;
    try
    {
        std::cout << nums[100] << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Excepcion capturada: indice fuera de rango" << std::endl;
    }

    std::cout << std::endl << "--- Array de strings ---" << std::endl;
    Array<std::string> strs(3);
    strs[0] = "hola";
    strs[1] = "mundo";
    strs[2] = "!";
    for (size_t i = 0; i < strs.size(); i++)
        std::cout << "strs[" << i << "] = " << strs[i] << std::endl;

    return 0;
}