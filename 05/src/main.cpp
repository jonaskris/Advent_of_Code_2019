#include <iostream>     // std::cout
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <string>       // std::string && std::getline && std::stoul && std::stoi

std::vector<int> runIntcodePart1(std::vector<unsigned int> intcode)
{
    std::vector<int> output;

    unsigned int ip = 0; // Instruction pointer
    while(true)
    {
        // Read operation
        unsigned int op = intcode[ip];
        std::string opStr = std::to_string(op);

        unsigned int    de =(opStr.size() >= 2)?( std::stoul(opStr.substr(opStr.size() - 2, 2)) ):( std::stoul(opStr.substr(opStr.size() - 1, 1)) ),   // Operation
                        c = (opStr.size() >= 3)?( std::stoul(opStr.substr(opStr.size() - 3, 1)) ):(0),  // Mode first parameter
                        b = (opStr.size() >= 4)?( std::stoul(opStr.substr(opStr.size() - 4, 1)) ):(0),  // Mode second parameter
                        a = (opStr.size() >= 5)?( std::stoul(opStr.substr(opStr.size() - 5, 1)) ):(0);  // Mode third parameter

        // Parameters point to either the immediate value or value at address depending on the three modes
        unsigned int& firstParameter = (c)?(intcode[ip + 1]):(intcode[intcode[ip + 1]]);
        unsigned int& secondParameter = (b)?(intcode[ip + 2]):(intcode[intcode[ip + 2]]);
        unsigned int& thirdParameter = (a)?(intcode[ip + 3]):(intcode[intcode[ip + 3]]);

        // Execute OP
        if(de == 1)         // Add
            thirdParameter = firstParameter + secondParameter;
        else if(de == 2)    // Multiply
            thirdParameter = firstParameter * secondParameter;
        else if(de == 3)    // Input integer
            std::cin >> firstParameter;
        else if(de == 4)    // Output integer
            output.push_back(firstParameter);
        else if(de == 99)   // Terminate
            return output;

        // Advance instruction pointer
        if(de == 1) ip += 4;
        else if(de == 2) ip += 4;
        else if(de == 3) ip += 2;
        else if(de == 4) ip += 2;
        else if(de == 99) ip += 1;
    }
}

std::vector<int> runIntcodePart2(std::vector<unsigned int> intcode)
{
    std::vector<int> output;

    unsigned int ip = 0; // Instruction pointer
    while(true)
    {
        // Read operation
        unsigned int op = intcode[ip];
        std::string opStr = std::to_string(op);

        unsigned int    de =(opStr.size() >= 2)?( std::stoul(opStr.substr(opStr.size() - 2, 2)) ):( std::stoul(opStr.substr(opStr.size() - 1, 1)) ),   // Operation
                        c = (opStr.size() >= 3)?( std::stoul(opStr.substr(opStr.size() - 3, 1)) ):(0),  // Mode first parameter
                        b = (opStr.size() >= 4)?( std::stoul(opStr.substr(opStr.size() - 4, 1)) ):(0),  // Mode second parameter
                        a = (opStr.size() >= 5)?( std::stoul(opStr.substr(opStr.size() - 5, 1)) ):(0);  // Mode third parameter

        // Parameters point to either the immediate value or value at address depending on the three modes
        unsigned int& firstParameter = (c)?(intcode[ip + 1]):(intcode[intcode[ip + 1]]);
        unsigned int& secondParameter = (b)?(intcode[ip + 2]):(intcode[intcode[ip + 2]]);
        unsigned int& thirdParameter = (a)?(intcode[ip + 3]):(intcode[intcode[ip + 3]]);

        // Execute OP
        if(de == 1)         // Add
        {
            thirdParameter = firstParameter + secondParameter;
            ip += 4;
        } else if(de == 2)  // Multiply
        {
            thirdParameter = firstParameter * secondParameter;
            ip += 4;
        } else if(de == 3)  // Input integer
        {
            std::cin >> firstParameter;
            ip += 2;
        } else if(de == 4)  // Output integer
        {
            output.push_back(firstParameter);
            ip += 2;
        } else if(de == 5)  // Jump-if-true
        {
            if(firstParameter)
                ip = secondParameter;
            else
                ip += 3;
        } else if(de == 6)  // Jump-if-false
        {
            if(!firstParameter)
                ip = secondParameter;
            else
                ip += 3;
        } else if(de == 7)  // Less than
        {
            if(firstParameter < secondParameter)
                thirdParameter = 1;
            else
                thirdParameter = 0;
            ip += 4;
        } else if(de == 8)  // Equals
        {
            if(firstParameter == secondParameter)
                thirdParameter = 1;
            else
                thirdParameter = 0;
            ip += 4;
        }else if(de == 99)  // Terminate
            return output;
    }
}

int main()
{
    // Variables
    std::vector<unsigned int> intcode;

    // Open file
    std::ifstream input("resources/input.txt");

    // Read integers delimited by comma from file to vector
    std::string numberStr;
    while(std::getline(input, numberStr, ','))
        intcode.push_back(std::stoi(numberStr)); // Convert string to int

    // Close file
    input.close();

    // Part 1
    {
        std::cout << "Input '1' to run tests: ";
        std::vector<int> output = runIntcodePart1(intcode);

        for(size_t i = 0; i < output.size(); i++)
            if(i != output.size() - 1)
                std::cout << "Test " << i << ": " << ((output[i])?("Failed!"):("Succeeded!")) << std::endl;
            else
                std::cout << "Diagnostic code: " << output[i] << std::endl;
    }

    // Part 2
    {
        std::cout << "Input '5' to diagnose the ships thermal radiator controller: ";
        std::vector<int> output = runIntcodePart2(intcode);

        std::cout << "Diagnostic code: " << output.at(0) << std::endl;
    }
}