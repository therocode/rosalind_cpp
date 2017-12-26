#include <fstream>
#include <string>
#include <iostream>

std::string loadFile(const std::string& path)
{
    std::ifstream inFile(path);

    //determine the file length
    inFile.seekg(0, std::ios_base::end);
    size_t sizeInBytes = inFile.tellg();
    inFile.seekg(0, std::ios_base::beg);

    //create a string to store the data
    std::string result;
    result.resize(sizeInBytes + 1);

    //load the data
    inFile.read((&result[0]), sizeInBytes);
    result.back() = '\0';

    return result;
}

int main()
{
    std::cout << "loading file\n";
    std::string data = loadFile("textout.txt");

    int32_t aAmount = 0;
    int32_t cAmount = 0;
    int32_t gAmount = 0;
    int32_t tAmount = 0;

    std::cout << "counting\n";

    for(uint8_t nucleotide : data)
    {
        switch(nucleotide)
        {
            case 'A':
                ++aAmount;
                break;
            case 'C':
                ++cAmount;
                break;
            case 'G':
                ++gAmount;
                break;
            case 'T':
                ++tAmount;
                break;
        }
    }

    std::cout << aAmount << " " << cAmount << " " << gAmount << " " << tAmount << "\n";
}
