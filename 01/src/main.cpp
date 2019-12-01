#include <iostream> // std::cout
#include <fstream>  // ifstream
#include <vector>   // std::vector
#include <iterator> // std::istream_iterator

int main()
{
    // Variables
    std::vector<unsigned int> modulesMass;

    // Open file
    std::ifstream input("resources/input.txt");

    // Read masses from file to vector
    std::copy(
        std::istream_iterator<unsigned int>(input), // input first
        std::istream_iterator<unsigned int>(),      // input last
        std::back_inserter(modulesMass)             // output first
    );

    // Close file
    input.close();

    // Part 1
    {
        unsigned int result;

        // Calculate sum of fuel requirements
        for(unsigned int moduleMass : modulesMass)
            result += moduleMass / 3 - 2;

        std::cout << "Part 1: \n\tThe sum of the fuel requirements for all modules is: " << result << std::endl;
    }

    // Part 2
    {   
        std::vector<unsigned int> modulesFuelTotal(modulesMass.size());
        
        // Calculate sum of fuel requirements per module
        for(unsigned int i = 0; i < modulesFuelTotal.size(); i++)
        {
            int addedMass = modulesMass[i]; // Mass added for every iteration. Initially the mass of the module.

            while(addedMass > 0)
            {
                float addedFuel = addedMass / 3 - 2;

                if(addedFuel > 0) // Any mass that would require negative fuel should instead be treated as if it requires zero fuel.
                    modulesFuelTotal[i] += addedFuel;

                addedMass = addedFuel;
            }
        }

        unsigned int result;

        // Calculate sum of fuel requirements
        for(unsigned int moduleFuel : modulesFuelTotal)
            result += moduleFuel;

        std::cout << "Part 2: \n\tThe sum of the fuel requirements for all modules is: " << result << std::endl;
    }
}