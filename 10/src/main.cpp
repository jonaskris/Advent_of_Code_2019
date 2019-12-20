#include <iostream>     // std::cout
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <string>       // std::string && std::getline && std::stoul && std::stoi
#include <set>          // std::set
#include <map>          // std::map
#include <math.h>       // atan2
#include <algorithm>    // std::sort

// Finds the asteroid that can detect the most other asteroids and how many asteroids it can detect
std::pair<unsigned int, unsigned int> findBestAsteroid(const std::vector<std::vector<bool>>& asteroidMap, unsigned int& detectableCount)
{
    unsigned int maxDetectableCount = 0, bestX = 0, bestY = 0;

    for(size_t y = 0; y < asteroidMap.size(); y++)
    {
        for(size_t x = 0; x < asteroidMap.back().size(); x++)
        {
            if(!asteroidMap[y][x]) continue;
            std::set<float> angles;

            for(size_t i = 0; i < asteroidMap.size(); i++)
            {
                for(size_t j = 0; j < asteroidMap.back().size(); j++)
                {
                    if(!asteroidMap[i][j]) continue;
                    if(i == y && j == x) continue;

                    float distanceX = (int)j - (int)x;
                    float distanceY = (int)i - (int)y;

                    angles.insert(atan2(distanceY, distanceX));
                }
            }

            if(angles.size() > maxDetectableCount)
            {
                maxDetectableCount = angles.size();
                bestX = x;
                bestY = y;
            }
        }
    }

    detectableCount = maxDetectableCount;
    return std::make_pair(bestX, bestY);
}

// Given an asteroid, returns a map from all angles to a sorted vector of distances to asteroids along those angles.
// The vectors are sorted by their angle.
std::map<float, std::vector<std::pair<unsigned int, unsigned int>>> evaluateAsteroid(const std::vector<std::vector<bool>>& asteroidMap, const std::pair<unsigned int, unsigned int> asteroidPos)
{
    static const std::pair<float, float> axis = { 0.0f, 1.0f };

    std::map<float, std::vector<std::pair<unsigned int, unsigned int>>> distancesByAngle;
    for(size_t y = 0; y < asteroidMap.size(); y++)
    {
        for(size_t x = 0; x < asteroidMap.front().size(); x++)
        {
            if(!asteroidMap[y][x]) continue;
            if(x == asteroidPos.first && y == asteroidPos.second) continue;

            std::pair<float, float> direction = {(float)x - (float)asteroidPos.first, (float)y - (float)asteroidPos.second};
            float angle = std::fmod(atan2(axis.first * direction.second - axis.second * direction.first,  axis.first * direction.first + axis.second * direction.second) - 2.0f * M_PI + 2.0f * M_PI, 2.0f * M_PI);

            distancesByAngle[angle].push_back({x, y});
        }
    }

    //for(auto it = distancesByAngle.begin(); it != distancesByAngle.end(); it++)
    //{
    //    for(auto jt = it->second.begin(); jt != it->second.end(); jt++)
    //    {
    //        std::cout << jt->first << ", " << jt->second << std::endl;
    //    }
    //    std::cout << std::endl;
    //}

    // Sort positions in descending order by distance.
    // Descending because we later will repeatedly remove smallest element from the vector,
    // which is much faster to do from the back of the vector. Back: Θ(1), front: Θ(n)
    for(auto it = distancesByAngle.begin(); it != distancesByAngle.end(); it++)
    {
        auto& distancesVector = it->second;
        std::sort(distancesVector.begin(), distancesVector.end(), 
        [&](std::pair<unsigned int, unsigned int> l, std::pair<unsigned int, unsigned int> r)
        {
            float distanceL = sqrt(
                (l.first - asteroidPos.first)*(l.first - asteroidPos.first) + 
                (l.second - asteroidPos.second)*(l.second - asteroidPos.second)
            );

            float distanceR = sqrt(
                (r.first - asteroidPos.first)*(r.first - asteroidPos.first) + 
                (r.second - asteroidPos.second)*(r.second - asteroidPos.second)
            );

            return distanceL > distanceR;
        });
    }

    return distancesByAngle;
}

int main()
{
    // Variables
    std::vector<std::vector<bool>> asteroidMap;

    // Open file
    std::ifstream input("resources/input.txt");

    // Read asteroid map to vector
    std::string line;
    while(std::getline(input, line))
    {
        asteroidMap.push_back({});
        
        for(char c : line)
            asteroidMap.back().push_back((c == '#')?(true):(false)); // Convert char to bool
    }

    // Close file
    input.close();


    std::pair<unsigned int, unsigned int> bestAsteroidPos;
    // Part 1
    {
        unsigned int detectableCount;
        bestAsteroidPos = findBestAsteroid(asteroidMap, detectableCount);

        std::cout << "Part 1: \n\tThe asteroid with most detectable asteroids from it is [" << bestAsteroidPos.first << ", " << bestAsteroidPos.second <<  "] and can detect: " << detectableCount << std::endl;     
    }

    // Part 2
    {   
        std::map<float, std::vector<std::pair<unsigned int, unsigned int>>> distancesByAngle = evaluateAsteroid(asteroidMap, bestAsteroidPos);

        std::pair<unsigned int, unsigned int> lastVaporized;

        auto it = distancesByAngle.begin();
        unsigned int counter = 0;
        while(counter < 200)
        {
            if(it == distancesByAngle.end())
                it = distancesByAngle.begin();

            if(it->second.empty())
            {
                auto newIt = it;
                newIt++;

                distancesByAngle.erase(it);
                it = newIt;
            }

            lastVaporized = it->second.back();
            it->second.pop_back();

            it++;
            counter++;
        }

        std::cout << "Part 2: \n\tThe last asteroid to be destroyed is at [" << lastVaporized.first << ", " << lastVaporized.second << "], and its x*100+y = " << lastVaporized.first * 100 + lastVaporized.second << std::endl;     
    }


    //std::pair<float, float> point = { 1.0f, 0.0f };
    //std::pair<float, float> axis = { 1.0f, 0.0f};
    //
    //std::cout << std::fmod(atan2(axis.first * point.second - axis.second * point.first,  axis.first * point.first + axis.second * point.second) + 2.0f * M_PI, 2.0f * M_PI) << std::endl;
}