#include <iostream>     // std::cout
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <string>       // std::string && std::getline

unsigned int runIntcode(std::vector<unsigned int> intcode, unsigned int noun, unsigned int verb)
{
    int i = 0;

    intcode[1] = noun;
    intcode[2] = verb;

    while(true)
    {
        unsigned int 
        op = intcode[i],        // Operation
        a = intcode[i + 1],     // First address
        b = intcode[i + 2],     // Second address
        res = intcode[i + 3];   // Third address

        if(op == 1)         // Add
            intcode[res] = intcode[a] + intcode[b];

        else if (op == 2)   // Multiply
            intcode[res] = intcode[a] * intcode[b];

        else                // Terminate
            return intcode[0];
            
        i += 4;
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
        intcode.push_back(std::stoul(numberStr)); // Convert string to unsigned (long) int. (There are no equivalent in std for unsigned int)

    // Close file
    input.close();

    // Part 1
    {
        // Run program with inputs [12, 2]
        std::cout << "Part 1: \n\tThe value left at position 0 after running is: " << runIntcode(intcode, 12, 2) << std::endl;
    }

    // Part 2
    {
        // Run program with inputs [0-99, 0-99] until desired result
        for(size_t i = 0; i < 100; i++)
            for(size_t j = 0; j < 100; j++)
                if(runIntcode(intcode, i, j) == 19690720)
                {
                    std::cout << "Part 2: \n\tThe parameters that give 19690720 as result are:\n\tNoun: " << i  << "\n\tVerb: " << j << std::endl;       
                    return 0;
                }    
    }
}