#include <random>
#include <iostream>
#include <fstream>

constexpr size_t bytesToGenerate = size_t(1024 * 1024 * 1024);
constexpr char nucleotides[4] = {'A', 'C', 'G', 'T'};
constexpr char nucleotidesBin[4] = {0, 1, 2, 3};

int main()
{
    std::string outPath = "textout.txt";
    std::string outPathBin = "binout.txt";

    std::ranlux48_base randomEngine((std::random_device()()));
    //std::uniform_int_distribution<size_t> randomIndex(0, 3);

    {
        std::cout << "allocating string\n";
        std::string textResult(bytesToGenerate, 'X');


        std::cout << "filling randomly\n";

        for(size_t byte = 0; byte < bytesToGenerate; ++byte)
        {
            uint8_t index = randomEngine() % 4;

            //text
            textResult[byte] = nucleotides[index];
        }

        std::cout << "saving to file\n";
        std::ofstream outFile(outPath);
        outFile << textResult;
    }

    {
        std::cout << "allocating bin out\n";
        std::vector<char> binResult;
        binResult.resize(bytesToGenerate / 4); //NOTE: will cut off if in-data is not /4
        int8_t currentInByte = 0;
        char currentByte = 0;

        std::cout << "filling randomly\n";

        for(size_t byte = 0; byte < bytesToGenerate; ++byte)
        {
            uint8_t index = randomEngine() % 4;

            //bin
            currentByte |= (nucleotidesBin[index] << currentInByte);

            currentInByte += 2;

            if(currentInByte == 8)
            {
                binResult[byte / 4] = currentByte;
                currentInByte = 0;
                currentByte = 0;
            }
        }

        std::cout << "saving to file\n";
        std::ofstream outFileBin(outPathBin, std::ios::binary | std::ios::out);
        outFileBin.write(binResult.data(), binResult.size());
    }
}
