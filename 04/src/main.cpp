#include <iostream>     // std::cout
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <algorithm>    // std::remove_if

bool satisfiesPart1Criteria(unsigned int e)
{
    std::string eStr = std::to_string(e); // Convert to string for easier per-digit checks
    char lastDigit; // Helper variable

    // Check length == 6
    if(eStr.length() != 6)
        return false;    // Does not satisfy criteria

    // Two adjacent digits are the same
    lastDigit = eStr[0];
    bool foundAdj = false;     // Found two adjacent digits
    for(size_t i = 1; i < eStr.size(); i++)
    {
        if(eStr[i] == lastDigit)
        {
            foundAdj = true;
            break;
        }
        lastDigit = eStr[i];
    }
    if(!foundAdj)
        return false;   // Does not satisfy criteria

    
    // Never decrease from left to right
    lastDigit = eStr[0];
    bool foundDecr = false;     // Found decreasing from left to right
    for(size_t i = 1; i < eStr.size(); i++)
    {
        if(eStr[i] < lastDigit)
        {
            foundDecr = true;
            break;
        }
        lastDigit = eStr[i];
    }
    if(foundDecr)
        return false;   // Does not satisfy criteria

    return true;        // Satisfies all criteria
}

bool satisfiesPart2Criteria(unsigned int e)
{
    std::string eStr = std::to_string(e); // Convert to string for easier per-digit checks
    char lastDigit; // Helper variable

    // Check length == 6
    if(eStr.length() != 6)
        return false;    // Does not satisfy criteria

    // Two adjacent digits are the same, AND cant be part of a bigger group
    lastDigit = eStr[0];
    bool foundAdj = false;     // Found two adjacent digits
    unsigned int countAdj = 1;
    for(size_t i = 1; i < eStr.size(); i++)
    {
        if(eStr[i] == lastDigit)
        {
            countAdj++;

            if(countAdj == 2 && i == eStr.size() - 1) // Extra check for last two digits
                foundAdj = true;
        } else
        {
            if(countAdj == 2)
            {
                foundAdj = true;
                break;
            }
            countAdj = 1;
        }
        
        lastDigit = eStr[i];
    }
    if(!foundAdj)
        return false;   // Does not satisfy criteria

    
    // Never decrease from left to right
    lastDigit = eStr[0];
    bool foundDecr = false;     // Found decreasing from left to right
    for(size_t i = 1; i < eStr.size(); i++)
    {
        if(eStr[i] < lastDigit)
        {
            foundDecr = true;
            break;
        }
        lastDigit = eStr[i];
    }
    if(foundDecr)
        return false;   // Does not satisfy criteria

    return true;        // Satisfies all criteria
}

int main()
{
    // Variables
    unsigned int rangeStart = 147981, rangeEnd = 691423;

    // Part 1
    {
        size_t count = 0;
        for(size_t i = rangeStart; i <= rangeEnd; i++)
            if(satisfiesPart1Criteria(i))
                count++;

        std::cout << "Part1: \n\tThe count of passwords within the range that satisfies the criteria is: " << count << std::endl;
    }

    // Part 2
    {
        size_t count = 0;
        for(size_t i = rangeStart; i <= rangeEnd; i++)
            if(satisfiesPart2Criteria(i))
                count++;
        
        std::cout << "Part2: \n\tThe count of passwords within the range that satisfies the criteria is: " << count << std::endl;
    }
}