#include <iostream>     // std::cout
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <string>       // std::string && std::getline
#include <map>          // std::map
#include <algorithm>    // std::copy && std::reverse_copy
#include <queue>        // std::queue

class PlanetTree
{
private:
public:
    std::map<std::string, std::pair<std::string, std::vector<std::string>>> nodes; // Node, parent, children

    void addOrbit(const std::string& orbitedPlanet, const std::string& orbitingPlanet)
    {
        nodes[orbitingPlanet].first = orbitedPlanet;
        nodes[orbitedPlanet].second.push_back(orbitingPlanet);
    }

    unsigned int indirectOrbitsTo(const std::string& planet)
    {
        std::queue<std::pair<std::string, unsigned int>> queue;
        queue.push({planet, 0});
        
        unsigned int orbitCount = 0;
        while(!queue.empty())
        {
            auto& children = nodes[queue.front().first].second;

            if(!children.empty())
                for(std::string& child : children)
                {
                    queue.push({child, queue.front().second + 1});
                    orbitCount += queue.front().second + 1;
                }                

            queue.pop();
        }

        return orbitCount;
    }

    std::vector<std::string> pathStartEnd(const std::string& start, const std::string& end)
    {
        std::vector<std::string> startPath = {start};  // Path from start to COM
        std::vector<std::string> endPath = {end};    // Path from end to COM

        // Find path from start to COM
        {
            std::string branchTop = start;
            while(branchTop != "COM")
            {
                startPath.push_back(nodes[branchTop].first);
                branchTop = startPath[startPath.size() - 1];
            }
        }

        // Find path from end to COM
        {
            std::string branchTop = end;
            while(branchTop != "COM")
            {
                endPath.push_back(nodes[branchTop].first);
                branchTop = endPath[endPath.size() - 1];
            }
        }

        // Remove common elements of paths to find intersection
        std::string lastRemoved;
        while(startPath[startPath.size() - 1] == endPath[endPath.size() - 1])
        {
            lastRemoved = startPath[startPath.size() - 1];
            startPath.pop_back();
            endPath.pop_back();
        }

        std::vector<std::string> path;
        std::copy(startPath.begin(), startPath.end(), std::back_inserter(path));
        path.push_back(lastRemoved);
        std::reverse_copy(endPath.begin(), endPath.end(), std::back_inserter(path));

        return path;
    }
};

int main()
{
    // Variables
    PlanetTree planets;

    // Open file
    std::ifstream input("resources/input.txt");

    // Read orbited and orbiting planets delimited by new line and add to tree
    std::string line;
    while(std::getline(input, line))
        planets.addOrbit(line.substr(0, line.find(')')), line.substr(line.find(')') + 1)); // Extract planet identifiers and add to tree

    // Close file
    input.close();

    // Part 1
    {
        std::cout << "Part1: \n\tThe count of direct and indirect orbits to COM is: " << planets.indirectOrbitsTo("COM") << std::endl;
    }

    // Part 2
    {        
        std::cout << "Part2: \n\tThe minimum number of orbital transfers required: " << planets.pathStartEnd("YOU", "SAN").size() - 3 << std::endl;
    }
}