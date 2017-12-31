#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <array>
#include <thread>
#include <atomic>

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

struct CountData
{
    std::atomic<int32_t> aAmount;
    std::atomic<int32_t> cAmount;
    std::atomic<int32_t> gAmount;
    std::atomic<int32_t> tAmount;
};

int main()
{
    std::cout << "loading file\n";
    std::vector<uint8_t> data = loadBinFile("binout.txt");

    CountData sharedResult{};

    auto job = [&sharedResult] (uint8_t* dataPointerStart, uint8_t* dataEnd)
    {
        int32_t aAmount = 0;
        int32_t cAmount = 0;
        int32_t gAmount = 0;
        int32_t tAmount = 0;

        uint64_t compressedCount = 0;
        int32_t currentCompressedCountCount = 0;

        for(uint8_t* currentData = dataPointerStart; currentData != dataEnd; ++ currentData)
        {
            compressedCount += countTable[*currentData];
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

        //any residual counts will be picked up here
        aAmount += extractACount(compressedCount);
        cAmount += extractCCount(compressedCount);
        gAmount += extractGCount(compressedCount);
        tAmount += extractTCount(compressedCount);

        sharedResult.aAmount += aAmount;
        sharedResult.cAmount += cAmount;
        sharedResult.gAmount += gAmount;
        sharedResult.tAmount += tAmount;
    };

    int32_t threadCount = 4;
    std::vector<std::thread> threads;

    size_t threadSegmentSize = data.size() / 4;
    size_t currentThreadStartIndex = 0;

    std::cout << "counting\n";

    for(int32_t i = 0; i < threadCount; ++i)
    {
        uint8_t* currentThreadStart = &(data[currentThreadStartIndex]);
        threads.emplace_back(job, currentThreadStart, currentThreadStart + threadSegmentSize);
        currentThreadStartIndex += threadSegmentSize;
    }

    for(int32_t i = 0; i < threadCount; ++i)
    {
        threads[i].join();
    }

    std::cout << sharedResult.aAmount << " " << sharedResult.cAmount << " " << sharedResult.gAmount << " " << sharedResult.tAmount << "\n";
}
