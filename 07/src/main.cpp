#include <iostream>     // std::cout
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <string>       // std::string && std::getline && std::stoul && std::stoi
#include <queue>        // std::queue
#include <algorithm>    // std::next_permutation && std::max

unsigned int factorial(unsigned int i)
{
    return (i == 1 || i == 0) ? 1 : factorial(i - 1) * i;
}

class IntcodeComputer
{
public:
    enum State{ READY, RUNNING, WAITING, TERMINATED };

private:
    std::vector<int> intcode;
    unsigned int ip = 0; // Instruction pointer
    State state = READY;

    std::queue<int> input;
    std::queue<int> output;

    State step(unsigned int op, int& first, int& second, int& third)
    {
        switch(op)
        {
            case 1: // Add
                third = first + second;

                ip += 4;
                return RUNNING;
            case 2: // Multiply
                third = first * second;

                ip += 4;
                return RUNNING;
            case 3: // Input
                if(input.empty())
                    return WAITING;

                first = input.front();
                input.pop();

                ip += 2;
                return RUNNING;
            case 4: // Output
                output.push(first);

                ip += 2;
                return RUNNING;
            case 5: // Jump-if-true
                if(first)
                    ip = second;
                else
                    ip += 3;
                return RUNNING;
            case 6: // Jump-if-false
                 if(!first)
                    ip = second;
                else
                    ip += 3;
                return RUNNING;
            case 7: // Less than
                if(first < second)
                    third = 1;
                else
                    third = 0;
                
                ip += 4;
                return RUNNING;
            case 8: // Equals
                if(first == second)
                    third = 1;
                else
                    third = 0;
                ip += 4;
                return RUNNING;
            case 99: // Terminate
                return TERMINATED;

            default: // Undefined operator
                std::cerr << "Tried executing undefined operator, exiting..." << std::endl;
                exit(EXIT_FAILURE);
        }
    }

public:
    IntcodeComputer(std::vector<int> intcode) : intcode(intcode) {}

    std::queue<int> process()
    {
        this->output = {};

        do
        {
            // Read operation
            unsigned int op = intcode[ip];
            std::string opStr = std::to_string(op);

            unsigned int    de =(opStr.size() >= 2)?( std::stoul(opStr.substr(opStr.size() - 2, 2)) ):( std::stoul(opStr.substr(opStr.size() - 1, 1)) );   // Operation
            int c = (opStr.size() >= 3)?( std::stoul(opStr.substr(opStr.size() - 3, 1)) ):(0),  // Mode first parameter
                b = (opStr.size() >= 4)?( std::stoul(opStr.substr(opStr.size() - 4, 1)) ):(0),  // Mode second parameter
                a = (opStr.size() >= 5)?( std::stoul(opStr.substr(opStr.size() - 5, 1)) ):(0);  // Mode third parameter

            // Parameters point to either the immediate value or value at address depending on the three modes
            int& firstParameter = (c)?(intcode[ip + 1]):(intcode[intcode[ip + 1]]);
            int& secondParameter = (b)?(intcode[ip + 2]):(intcode[intcode[ip + 2]]);
            int& thirdParameter = (a)?(intcode[ip + 3]):(intcode[intcode[ip + 3]]);

            // Execute operation
            state = step(de, firstParameter, secondParameter, thirdParameter);
        } while(state == RUNNING);

        return output;
    }

    void addInput(std::queue<int> input)
    {
        while(!input.empty())
        {
            this->input.push(input.front());
            input.pop();
        }
    }

    void addInput(int input)
    {
        this->input.push(input);
    }

    State getState() const { return state; }
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
            std::vector<IntcodeComputer> amplifiers(5, intcode);

            // Set initial input
            for(size_t j = 0; j < amplifiers.size(); j++)
                amplifiers[j].addInput(phaseSettings[j]);

            amplifiers[0].addInput(0);

            std::queue<int> lastOutput;

            // Run code once for every amplifier
            for(size_t j = 0; j < amplifiers.size(); j++)
            {   
                lastOutput = amplifiers[j].process();

                // Set input of next amplifier
                if(j != 4)
                    amplifiers[j+1].addInput(lastOutput);
            }

            maxOutput = std::max(maxOutput, lastOutput.back());
            std::next_permutation(phaseSettings.begin(), phaseSettings.end());
        }

        std::cout << "Part 1: \n\tThe highest signal that can be sent to the thrusters is: " << maxOutput << std::endl;     
    }

    // Part 2
    {
        std::vector<int> phaseSettings {5, 6, 7, 8, 9};

        int maxOutput = 0;
        for(size_t i = 0; i < factorial(phaseSettings.size()); i++) // For every possible permutation of phase settings
        {
            std::vector<IntcodeComputer> amplifiers(5, intcode);

            // Set initial input
            for(size_t j = 0; j < amplifiers.size(); j++)
                amplifiers[j].addInput(phaseSettings[j]);

            amplifiers[0].addInput(0);

            std::queue<int> lastOutput;
            int j = 0;

            // Run code for every amplifier until last has terminated
            while(amplifiers[4].getState() != IntcodeComputer::State::TERMINATED)
            {
                // Process input
                lastOutput = amplifiers[j%5].process();
                
                // Set input of next amplifier
                amplifiers[(j+1)%5].addInput(lastOutput);

                j++;
            }

            maxOutput = std::max(maxOutput, lastOutput.back());
            std::next_permutation(phaseSettings.begin(), phaseSettings.end());
        }

        std::cout << "Part 2: \n\tThe highest signal that can be sent to the thrusters is: " << maxOutput << std::endl;       
    }
    
}