#include <iostream> // std::cout
#include <fstream>  // ifstream
#include <vector>   // std::vector
#include <array>    // std::array

int main()
{
    // Variables
    std::vector<unsigned int> image;

    // Open file
    std::ifstream input("resources/input.txt");

    // Read integers delimited by comma from file to vector
    char integerChar;
    while(input.get(integerChar))
        image.push_back(integerChar - 48); // Convert char to int

    // Close file
    input.close();

    // Part 1
    {
        const unsigned int width = 25, height = 6;
        const unsigned int layers = image.size() / (width * height);

        unsigned int fewestZeroes, onesMultTwos;
        for(size_t i = 0; i < layers; i++)
        {
            unsigned int numCount[3] {0, 0, 0}; // Count of zeroes, ones and twos

            for(size_t j = 0; j < width * height; j++)
            {
                numCount[image[(i * width * height) + j]]++;
            }

            if(numCount[0] < fewestZeroes || i == 0)
            {
                fewestZeroes = numCount[0];
                onesMultTwos = numCount[1] * numCount[2];
                std::cout << "t" << std::endl;
            }
        }

        std::cout << "Part 1: \n\tThe number of 1 digits multiplied number of 2 digits on layer with fewest zeroes is: " << onesMultTwos << std::endl;     
    }

    // Part 2
    {
        const unsigned int width = 25, height = 6;
        const unsigned int layers = image.size() / (width * height);

        std::array<unsigned int, width * height> decoded;
        decoded.fill(2);

        for(size_t i = 0; i < layers; i++)
            for(size_t j = 0; j < width * height; j++)
                if(decoded[j] == 2)
                {
                    unsigned int pixel = image[i * width * height + j];    
                    decoded[j] = pixel;
                }

        std::cout << "Part 2: \n\tThe decoded image is: " << std::endl; 

        for(size_t i = 0; i < height; i++)
        {
            std::cout << "\t";
            for(size_t j = 0; j < width; j++)
                std::cout << (decoded[width * i + j]?'X':'.');
            std::cout << std::endl;
        }    
    }
}