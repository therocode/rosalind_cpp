#include <fstream>
#include <string>
#include <iostream>
#include <vector>

std::vector<uint8_t> loadBinFile(const std::string& path)
{
    std::ifstream inFile(path);

    //determine the file length
    inFile.seekg(0, std::ios_base::end);
    size_t sizeInBytes = inFile.tellg();
    inFile.seekg(0, std::ios_base::beg);

    //create a vector to store the data
    std::vector<uint8_t> result(sizeInBytes);

    //load the data
    inFile.read(reinterpret_cast<char*>(&result[0]), sizeInBytes);

    return result;
}

int main()
{
    std::cout << "loading file\n";
    std::vector<uint8_t> data = loadBinFile("binout.txt");

    int32_t aAmount = 0;
    int32_t cAmount = 0;
    int32_t gAmount = 0;
    int32_t tAmount = 0;

    std::cout << "counting\n";

    for(uint8_t compressed : data)
    {
        for(int8_t charInByte = 0; charInByte < 8; charInByte += 2)
        {
            uint8_t nucleotideIndex = (compressed >> charInByte) & 0b00000011;

            switch(nucleotideIndex)
            {
                case 0:
                    ++aAmount;
                    break;
                case 1:
                    ++cAmount;
                    break;
                case 2:
                    ++gAmount;
                    break;
                case 3:
                    ++tAmount;
                    break;
            }
        }
    }

    std::cout << aAmount << " " << cAmount << " " << gAmount << " " << tAmount << "\n";
}
