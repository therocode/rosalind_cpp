#include <fstream>
#include <string>
#include <iostream>

std::string loadFile(const std::string& path)
{
    std::ifstream inFile(path);
    std::string result((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    return result;
}

int main()
{
    std::cout << "loading file\n";
    std::string data = loadFile("../tools_datageneration/textout.txt");

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
