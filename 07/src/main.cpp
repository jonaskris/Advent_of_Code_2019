#include <iostream>     // std::cout
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <string>       // std::string && std::getline && std::stoul && std::stoi
#include <deque>        // std::deque
#include <algorithm>    // std::next_permutation && std::max

unsigned int factorial(unsigned int i)
{
    return (i == 1 || i == 0) ? 1 : factorial(i - 1) * i;
}

class IntcodeComputer
{
private:
    std::vector<int> intcode;
    unsigned int ip = 0; // Instruction pointer

    std::deque<int> lastOutput;
public:
    enum EXIT_CODE{ TERMINATED, WAITING };

    IntcodeComputer(std::vector<int> intcode) : intcode(intcode) {}

    EXIT_CODE runIntcode(std::deque<int> input = {})
    {
        lastOutput.clear();

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
            int& firstParameter = (c)?(intcode[ip + 1]):(intcode[intcode[ip + 1]]);
            int& secondParameter = (b)?(intcode[ip + 2]):(intcode[intcode[ip + 2]]);
            int& thirdParameter = (a)?(intcode[ip + 3]):(intcode[intcode[ip + 3]]);

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
                ip += 2;

                if(!input.empty())
                {
                    firstParameter = input.front();
                    input.pop_front();
                } else
                    return WAITING;
            } else if(de == 4)  // Output integer
            {
                lastOutput.clear();
                lastOutput.push_back(firstParameter);
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
            } else if(de == 99)  // Terminate
                return TERMINATED;
        }
    }

    std::deque<int> getLastOutput()
    {
        return lastOutput;
    }
};


int main()
{
    // Variables
    std::vector<int> intcode;

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
        std::vector<int> phaseSettings {0, 1, 2, 3, 4};

        int maxOutput = 0;
        for(size_t i = 0; i < factorial(phaseSettings.size()); i++) // For every possible permutation of phase settings
        {
            std::vector<IntcodeComputer> computers(5, intcode);

            std::deque<int> input = {0};
            for(size_t j = 0; j < computers.size(); j++) // Run intcode once for every amplifier
            {
                if(j != 0)
                    input = computers[(j-1) % computers.size()].getLastOutput();
                
                // Use last computers output as input
                input.push_front(phaseSettings[j]);

                computers[j].runIntcode(input);
            }

            maxOutput = std::max(maxOutput, computers[4].getLastOutput().back());
            std::next_permutation(phaseSettings.begin(), phaseSettings.end());
        }

        std::cout << "Part 1: \n\tThe highest signal that can be sent to the thrusters is: " << maxOutput << std::endl;     
    }

    // Part 2
    {
        //std::vector<int> phaseSettings {5, 6, 7, 8, 9};
        std::vector<int> phaseSettings {9,7,8,5,6};

        int maxOutput = 0;
        for(size_t i = 0; i < factorial(phaseSettings.size()); i++) // For every possible permutation of phase settings
        {
            std::vector<IntcodeComputer> computers(5, intcode);

            unsigned int terminatedExitCodeCounter = 0;

            std::deque<int> input = {0};
            int j = 0;
            while(terminatedExitCodeCounter < 5) // Run intcode for every computer until all have terminated
            {
                // Use last computers output as input
                if(j != 0)
                    input = computers[(j-1) % computers.size()].getLastOutput();
                
                // Specify phase setting for first run of every computer
                if(j < 5)
                    input.push_front(phaseSettings[j]);

                if(computers[j % computers.size()].runIntcode(input) == IntcodeComputer::TERMINATED)
                    terminatedExitCodeCounter++;

                j++;
            }

            maxOutput = std::max(maxOutput, computers[4].getLastOutput().back());

            //std::cout << maxOutput << std::endl;    // Temporary to test examples with specific phase settings
            //std::cin.get();                         // Temporary to test examples with specific phase settings

            std::next_permutation(phaseSettings.begin(), phaseSettings.end());
        }

        std::cout << "Part 2: \n\tThe highest signal that can be sent to the thrusters is: " << maxOutput << std::endl;       
    }
}