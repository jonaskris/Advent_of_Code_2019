#include <iostream>     // std::cout
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <string>       // std::string && std::getline && std::stoul && std::stoi
#include <queue>        // std::queue
#include <algorithm>    // std::next_permutation && std::max
#include <map>          // std::map

class IntcodeComputer
{
public:
    enum State{ READY, RUNNING, WAITING, TERMINATED };

private:
    std::vector<long int> intcode;
    unsigned int ip = 0; // Instruction pointer
    unsigned int base = 0; // Current relative base
    State state = READY;


    std::queue<long int> inputQueue;
    std::queue<long int> outputQueue;
    std::map<unsigned int, long int> memory;

    void add(long int& first, long int& second, long int& third)
    {
        third = first + second;

        ip += 4;
        state = RUNNING;
    }

    void multiply(long int& first, long int& second, long int& third)
    {
        third = first * second;

        ip += 4;
        state = RUNNING;
    }

    void input(long int& first)
    {
        if(inputQueue.empty())
        {
            state = WAITING;
            return;
        }

        first = inputQueue.front();
        inputQueue.pop();

        ip += 2;
        state = RUNNING;
    }

    void output(long int& first)
    {
        outputQueue.push(first);

        ip += 2;
        state = RUNNING;
    }

    void jumpIfTrue(long int& first, long int& second)
    {
        if(first)
            ip = second;
        else
            ip += 3;
        state = RUNNING;
    }

    void jumpIfFalse(long int& first, long int& second)
    {
        if(!first)
            ip = second;
        else
            ip += 3;
        state = RUNNING;
    }

    void lessThan(long int& first, long int& second, long int& third)
    {
        if(first < second)
            third = 1;
        else
            third = 0;
                
        ip += 4;
        state = RUNNING;
    }

    void equals(long int& first, long int& second, long int& third)
    {
        if(first == second)
            third = 1;
        else
            third = 0;

        ip += 4;
        state = RUNNING;
    }

    void adjustBase(long int& first)
    {
        base += first;

        ip += 2;
        state = RUNNING;
    }

    void terminate()
    {
        state = State::TERMINATED;
    }

    long int& write(unsigned int location, unsigned int mode)
    {
        if(mode == 0) // Position mode
            return write(read(location, 1), 1);
        else if(mode == 1) // Value mode
            return memory[location];
        else if(mode == 2) // Relative mode
            return write(read(location, 1) + base, 1);
    }

    long int& read(unsigned int location, unsigned int mode)
    {
        auto it = memory.find(location);
        if(it != memory.end())
            if(mode == 0) // Position mode
                return read(it->second, 1);
            else if (mode == 1) // Value mode
                return it->second;
            else // Relative mode
                return read(it->second + base, 1);
        else
            if(mode == 0) // Position mode
                return read(intcode.at(location), 1);
            else if(mode == 1) // Value mode
                return intcode.at(location);
            else // Relative mode
                return read(intcode.at(location) + base, 1);
                
    }

public:
    IntcodeComputer(std::vector<long int> intcode) : intcode(intcode) 
    {
        //for(auto i : intcode)
        //    std::cout << i << ", ";
        //std::cout << std::endl;
    }

    std::queue<long int> process()
    {
        outputQueue = {}; // Reset output

        do
        {
            // Interpret instruction
            unsigned int instruction = intcode[ip];
            std::string instructionStr = std::to_string(instruction);

            unsigned int    op = (instructionStr.size() >= 2)?( std::stoul(instructionStr.substr(instructionStr.size() - 2, 2)) ):( std::stoul(instructionStr.substr(instructionStr.size() - 1, 1)) );   // Operation
            unsigned int modeFirst = (instructionStr.size() >= 3)?( std::stoul(instructionStr.substr(instructionStr.size() - 3, 1)) ):(0),  // Mode first parameter
                modeSecond = (instructionStr.size() >= 4)?( std::stoul(instructionStr.substr(instructionStr.size() - 4, 1)) ):(0),  // Mode second parameter
                modeThird = (instructionStr.size() >= 5)?( std::stoul(instructionStr.substr(instructionStr.size() - 5, 1)) ):(0);  // Mode third parameter

            //std::cout << "op: " << op << std::endl;
            //std::cout << "modeFirst: " << modeFirst << std::endl;
            //std::cout << "modeSecond: " << modeSecond << std::endl;
            //std::cout << "modeThird: " << modeThird << std::endl;

            // Execute operation
            switch(op)
            {
                case 1:
                    add
                    (
                        read(ip+1, modeFirst),
                        read(ip+2, modeSecond),
                        write(ip+3, modeThird)
                    );//memoryAt(ip+1, modeFirst, true), memoryAt(ip+2, modeSecond, true), memoryAt(ip+3, modeThird, false));
                    break;
                case 2:
                    multiply
                    (
                        read(ip+1, modeFirst),
                        read(ip+2, modeSecond),
                        write(ip+3, modeThird)
                    );//memoryAt(ip+1, modeFirst, true), memoryAt(ip+2, modeSecond, true), memoryAt(ip+3, modeThird, false));
                    break;
                case 3:
                    input
                    (
                        write(ip+1, modeFirst)
                    );//memoryAt(ip+1, modeFirst, false));
                    break;
                case 4:
                    output
                    (
                        read(ip+1, modeFirst)
                    );//memoryAt(ip+1, modeFirst, true));
                    break;
                case 5:
                    jumpIfTrue
                    (
                        read(ip+1, modeFirst),
                        read(ip+2, modeSecond)
                    );//memoryAt(ip+1, modeFirst, true), memoryAt(ip+2, modeSecond, true));
                    break;
                case 6:
                    jumpIfFalse
                    (
                        read(ip+1, modeFirst),
                        read(ip+2, modeSecond)
                    );//memoryAt(ip+1, modeFirst, true), memoryAt(ip+2, modeSecond, true));
                    break;
                case 7:
                    lessThan
                    (
                        read(ip+1, modeFirst),
                        read(ip+2, modeSecond),
                        write(ip+3, modeThird)
                    );//memoryAt(ip+1, modeFirst, true), memoryAt(ip+2, modeSecond, true), memoryAt(ip+3, modeThird, false));
                    break;
                case 8:
                    equals
                    (
                        read(ip+1, modeFirst),
                        read(ip+2, modeSecond),
                        write(ip+3, modeThird)                    
                    );//memoryAt(ip+1, modeFirst, true), memoryAt(ip+2, modeSecond, true), memoryAt(ip+3, modeThird, false));
                    break;
                case 9:
                    adjustBase
                    (
                        read(ip+1, modeFirst)
                    );//memoryAt(ip+1, modeFirst, true));
                    break;
                case 99:
                    terminate();
                    break;
                default:
                    std::cerr << "Tried executing undefined operator, exiting..." << std::endl;
                    exit(EXIT_FAILURE);
                    break;
            }
        } while(state == RUNNING);

        return outputQueue;
    }

    void addInput(std::queue<long int> input)
    {
        while(!input.empty())
        {
            inputQueue.push(input.front());
            input.pop();
        }
    }

    void addInput(long int input)
    {
        inputQueue.push(input);
    }

    State getState() const { return state; }
};


int main()
{
    // Variables
    std::vector<long int> intcode;

    // Open file
    std::ifstream input("resources/input.txt");

    // Read integers delimited by comma from file to vector
    std::string numberStr;
    while(std::getline(input, numberStr, ','))
        intcode.push_back(std::stol(numberStr)); // Convert string to int

    // Close file
    input.close();

    // Part 1
    {
        IntcodeComputer c(intcode);
        c.addInput(1);
        std::queue<long int> output = c.process();

        std::cout << "Part 1: \n\tThe BOOST keycode is: " << output.back() << std::endl;     
    }

    // Part 2
    {
        IntcodeComputer c(intcode);
        c.addInput(2);
        std::queue<long int> output = c.process();

        std::cout << "Part 2: \n\tThe coordinates of the distress signal is: " << output.back() << std::endl;     
    }
    
}