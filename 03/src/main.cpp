#include <iostream>     // std::cout
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <string>       // std::string && std::getline && std::stoul
#include <sstream>      // std::istringstream
#include <utility>      // std::pair
#include <optional>     // std::optional

struct Vec2i { int x, y; };

struct WireSegment
{
    Vec2i start, end;
    unsigned int steps; // Steps to end of WireSegment
};

bool betweenOrEq(int a, int b1, int b2)
{
    return (a <= b1 && a >= b2) || (a <= b2 && a >= b1);
}

// Checks for intersection between horizontal and vertical WireSegments
// Both the check and figuring out the point of intersection takes advantage of horizontal segment having yStart=yEnd and the same for vertical segment x
std::optional<std::pair<Vec2i, unsigned int>> getIntersection(const WireSegment& horizontal, const WireSegment& vertical)
{
    if(betweenOrEq(vertical.start.x, horizontal.start.x, horizontal.end.x) &&
        betweenOrEq(horizontal.start.y, vertical.start.y, vertical.end.y))
    {
        // Return point of intersection and combined steps of the two wires up until the intersection
        return std::make_pair
        (
            Vec2i{vertical.start.x, horizontal.start.y}, 
            (horizontal.steps - abs(horizontal.end.x - vertical.start.x)) + (vertical.steps - abs(vertical.end.y - horizontal.start.y))
        );
    }

    return std::nullopt; // No intersection
}

struct Wire
{
private:
    // Separating horizontal and vertical WireSegments simplifies the intersection test
    // and lets us decrease the number of tests as parallel lines cant intersect.
    std::vector<WireSegment> horizontalWireSegments;
    std::vector<WireSegment> verticalWireSegments;

public:
    Wire(const std::string& wirePath)
    {
        std::istringstream wirePathSS(wirePath);

        Vec2i positionSoFar = { 0, 0 }; // The position of the wire so far, begins in origin
        unsigned int stepsSoFar = 0;    // The count of steps of the wire so far

        std::string pathComponent;
        while(std::getline(wirePathSS, pathComponent, ',')) // Get every substring delimited by comma
        {
            char direction = pathComponent[0];                                                      // First character is direction
            unsigned int distance = std::stoul( pathComponent.substr(1, pathComponent.size()) );    // Rest is distance

            // Create new line segment
            WireSegment newWireSegment{
                Vec2i   // From
                {
                    positionSoFar.x, 
                    positionSoFar.y
                },
                Vec2i   // To
                {
                    positionSoFar.x + (direction == 'R' ? (int)distance :       // If R, positive distance
                                        direction == 'L' ? -(int)distance :     // If L, negative distance
                                        0),                                     // If neither, 0

                    positionSoFar.y + (direction == 'U' ? (int)distance :       // If U, positive distance
                                 direction == 'D' ? -(int)distance :            // If D, negative distance
                                    0)                                          // If neither, 0
                },
                stepsSoFar + distance
            };

            // Add to horizontal/vertical line segment vectors
            if(direction == 'R' || direction == 'L')
                horizontalWireSegments.push_back(newWireSegment);
            else if(direction == 'U' || direction == 'D')
                verticalWireSegments.push_back(newWireSegment);

            positionSoFar = newWireSegment.end; // Update position so far
            stepsSoFar += distance;             // Update steps so far
        }
    }

    std::vector<std::pair<Vec2i, unsigned int>> getIntersections(const Wire& other) const
    {
        std::vector<std::pair<Vec2i, unsigned int>> intersections;

        // Check own horizontal against others vertical
        for(const WireSegment& horizontal : horizontalWireSegments)
            for(const WireSegment& vertical : other.verticalWireSegments)
                if(std::optional<std::pair<Vec2i, unsigned int>> intersection = getIntersection(horizontal, vertical))
                    intersections.push_back(intersection.value());

        // Check own vertical against others horizontal
        for(const WireSegment& vertical : verticalWireSegments)
            for(const WireSegment& horizontal : other.horizontalWireSegments)
                if(std::optional<std::pair<Vec2i, unsigned int>> intersection = getIntersection(horizontal, vertical))
                    intersections.push_back(intersection.value());

        return intersections;
    }
};

int main()
{
    // Variables
    std::vector<Wire> wires;

    // Open file
    std::ifstream input("resources/input.txt");

    // Read each line and use lines to construct Wire objects
    std::string wireStr;
    while(std::getline(input, wireStr))
        wires.emplace_back(wireStr);    // Construct in-place

    // Close file
    input.close();

    // Find intersections
    std::vector<std::pair<Vec2i, unsigned int>> intersections = wires.at(0).getIntersections(wires.at(1));

    // Part 1
    {
        int smallestDistanceIndex = -1;
        unsigned int smallestDistance;

        for(size_t i = 0; i < intersections.size(); i++)
        {
            unsigned int distance = abs(intersections[i].first.x) + abs(intersections[i].first.y);

            if(smallestDistanceIndex == -1 || smallestDistance == 0 || (distance < smallestDistance))
            {
                smallestDistanceIndex = i;
                smallestDistance = distance;
            }
        }

        std::cout << "Part1: \n\tThe distance between the central port and the closest intersection is: " << smallestDistance << std::endl;
    }

    // Part 2
    {
        int fewestStepsIndex = -1;
        unsigned int fewestSteps;

        for(size_t i = 0; i < intersections.size(); i++)
        {
            if(fewestStepsIndex == -1 || fewestStepsIndex == 0 || (intersections[i].second < fewestSteps))
            {
                fewestStepsIndex = i;
                fewestSteps = intersections[i].second;
            }
        }

        std::cout << "Part2: \n\tThe fewest steps to reach an intersection is: " << fewestSteps << std::endl;
    }
}