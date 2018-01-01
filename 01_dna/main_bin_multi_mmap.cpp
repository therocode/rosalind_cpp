#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <array>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

constexpr uint64_t countCompressed(uint8_t compressed)
{
    uint64_t aAmount = 0;
    uint64_t cAmount = 0;
    uint64_t gAmount = 0;
    uint64_t tAmount = 0;

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

    return aAmount << 48 | cAmount << 32 | gAmount << 16 | tAmount;
}

//0xAAAACCCCGGGGTTTT

constexpr uint16_t extractACount(uint64_t multiCount)
{
    return static_cast<uint16_t>(multiCount >> 48);
}

constexpr uint16_t extractCCount(uint64_t multiCount)
{
    return static_cast<uint16_t>(multiCount >> 32 & 0x000000000000ffff);
}

constexpr uint16_t extractGCount(uint64_t multiCount)
{
    return static_cast<uint16_t>(multiCount >> 16 & 0x000000000000ffff);
}

constexpr uint16_t extractTCount(uint64_t multiCount)
{
    return static_cast<uint16_t>(multiCount & 0x000000000000ffff);
}

constexpr std::array<uint64_t, 256> countTableFactory()
{
    std::array<uint64_t, 256> result{};

    for(size_t i = 0; i < 256; ++i)
        result[i] = countCompressed(i);

    return result;
}

constexpr std::array<uint64_t, 256> countTable = countTableFactory();

//tests to ensure table existing in compile time and to ensure the correctness of it
static_assert(extractACount(countTable[0]) == 4, "countTable is incorrect");
static_assert(extractCCount(countTable[0]) == 0, "countTable is incorrect");
static_assert(extractGCount(countTable[0]) == 0, "countTable is incorrect");
static_assert(extractTCount(countTable[0]) == 0, "countTable is incorrect");

constexpr uint8_t testCompressed = static_cast<uint8_t>(1 | 3 << 2 | 0 << 4 | 3 << 6);
static_assert(extractACount(countTable[testCompressed]) == 1, "countTable is incorrect");
static_assert(extractCCount(countTable[testCompressed]) == 1, "countTable is incorrect");
static_assert(extractGCount(countTable[testCompressed]) == 0, "countTable is incorrect");
static_assert(extractTCount(countTable[testCompressed]) == 2, "countTable is incorrect");

struct BinFile
{
    char* start;
    size_t size;
};

BinFile loadBinFile(const std::string& path)
{
    BinFile result;

    int32_t file = open(path.c_str(), O_RDONLY);

    struct stat fileStat;
    //determine the file length
    fstat(file, &fileStat);
    result.size = fileStat.st_size;
    //close(file); close should happen after mmap is freed, but we don't do that so we don't do this. who cares

    result.start = static_cast<char*>(mmap(nullptr, result.size, PROT_READ, MAP_SHARED, file, 0));
    if(result.start == MAP_FAILED)
        std::cout << "error mapping file\n";

    return result;
}

int main()
{
    std::cout << "loading file\n";
    BinFile data = loadBinFile("binout.txt");

    int32_t aAmount = 0;
    int32_t cAmount = 0;
    int32_t gAmount = 0;
    int32_t tAmount = 0;

    uint64_t compressedCount = 0;
    int32_t currentCompressedCountCount = 0;

    std::cout << "counting\n";
    char* end = data.start + data.size;
    int64_t count = 0;
    for(char* current = data.start; current != end; ++current)
    {
        ++count;
        compressedCount += countTable[static_cast<uint8_t>(*current)];
        ++currentCompressedCountCount;

        //to prevent overflow of our individual 16-bit counters, we need to extract and restart counters every uint16_t-max / 4 iterations, based on worst case of data containing only 1 nucleotide
        if(currentCompressedCountCount >= std::numeric_limits<uint16_t>::max() / 4)
        {
            aAmount += extractACount(compressedCount);
            cAmount += extractCCount(compressedCount);
            gAmount += extractGCount(compressedCount);
            tAmount += extractTCount(compressedCount);

            compressedCount = 0;
            currentCompressedCountCount = 0;
        }
    }
    std::cout << count << '\n';

    //any residual counts will be picked up here
    aAmount += extractACount(compressedCount);
    cAmount += extractCCount(compressedCount);
    gAmount += extractGCount(compressedCount);
    tAmount += extractTCount(compressedCount);

    std::cout << aAmount << " " << cAmount << " " << gAmount << " " << tAmount << "\n";
}
