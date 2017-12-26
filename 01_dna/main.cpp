#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <limits>

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

constexpr uint64_t countTable[256] =
{
    countCompressed(static_cast<uint8_t>(0)), countCompressed(static_cast<uint8_t>(1)), countCompressed(static_cast<uint8_t>(2)), countCompressed(static_cast<uint8_t>(3)),
    countCompressed(static_cast<uint8_t>(4)), countCompressed(static_cast<uint8_t>(5)), countCompressed(static_cast<uint8_t>(6)), countCompressed(static_cast<uint8_t>(7)),
    countCompressed(static_cast<uint8_t>(8)), countCompressed(static_cast<uint8_t>(9)), countCompressed(static_cast<uint8_t>(10)), countCompressed(static_cast<uint8_t>(11)),
    countCompressed(static_cast<uint8_t>(12)), countCompressed(static_cast<uint8_t>(13)), countCompressed(static_cast<uint8_t>(14)), countCompressed(static_cast<uint8_t>(15)),
    countCompressed(static_cast<uint8_t>(16)), countCompressed(static_cast<uint8_t>(17)), countCompressed(static_cast<uint8_t>(18)), countCompressed(static_cast<uint8_t>(19)),
    countCompressed(static_cast<uint8_t>(20)), countCompressed(static_cast<uint8_t>(21)), countCompressed(static_cast<uint8_t>(22)), countCompressed(static_cast<uint8_t>(23)),
    countCompressed(static_cast<uint8_t>(24)), countCompressed(static_cast<uint8_t>(25)), countCompressed(static_cast<uint8_t>(26)), countCompressed(static_cast<uint8_t>(27)),
    countCompressed(static_cast<uint8_t>(28)), countCompressed(static_cast<uint8_t>(29)), countCompressed(static_cast<uint8_t>(30)), countCompressed(static_cast<uint8_t>(31)),
    countCompressed(static_cast<uint8_t>(32)), countCompressed(static_cast<uint8_t>(33)), countCompressed(static_cast<uint8_t>(34)), countCompressed(static_cast<uint8_t>(35)),
    countCompressed(static_cast<uint8_t>(36)), countCompressed(static_cast<uint8_t>(37)), countCompressed(static_cast<uint8_t>(38)), countCompressed(static_cast<uint8_t>(39)),
    countCompressed(static_cast<uint8_t>(40)), countCompressed(static_cast<uint8_t>(41)), countCompressed(static_cast<uint8_t>(42)), countCompressed(static_cast<uint8_t>(43)),
    countCompressed(static_cast<uint8_t>(44)), countCompressed(static_cast<uint8_t>(45)), countCompressed(static_cast<uint8_t>(46)), countCompressed(static_cast<uint8_t>(47)),
    countCompressed(static_cast<uint8_t>(48)), countCompressed(static_cast<uint8_t>(49)), countCompressed(static_cast<uint8_t>(50)), countCompressed(static_cast<uint8_t>(51)),
    countCompressed(static_cast<uint8_t>(52)), countCompressed(static_cast<uint8_t>(53)), countCompressed(static_cast<uint8_t>(54)), countCompressed(static_cast<uint8_t>(55)),
    countCompressed(static_cast<uint8_t>(56)), countCompressed(static_cast<uint8_t>(57)), countCompressed(static_cast<uint8_t>(58)), countCompressed(static_cast<uint8_t>(59)),
    countCompressed(static_cast<uint8_t>(60)), countCompressed(static_cast<uint8_t>(61)), countCompressed(static_cast<uint8_t>(62)), countCompressed(static_cast<uint8_t>(63)),
    countCompressed(static_cast<uint8_t>(64)), countCompressed(static_cast<uint8_t>(65)), countCompressed(static_cast<uint8_t>(66)), countCompressed(static_cast<uint8_t>(67)),
    countCompressed(static_cast<uint8_t>(68)), countCompressed(static_cast<uint8_t>(69)), countCompressed(static_cast<uint8_t>(70)), countCompressed(static_cast<uint8_t>(71)),
    countCompressed(static_cast<uint8_t>(72)), countCompressed(static_cast<uint8_t>(73)), countCompressed(static_cast<uint8_t>(74)), countCompressed(static_cast<uint8_t>(75)),
    countCompressed(static_cast<uint8_t>(76)), countCompressed(static_cast<uint8_t>(77)), countCompressed(static_cast<uint8_t>(78)), countCompressed(static_cast<uint8_t>(79)),
    countCompressed(static_cast<uint8_t>(80)), countCompressed(static_cast<uint8_t>(81)), countCompressed(static_cast<uint8_t>(82)), countCompressed(static_cast<uint8_t>(83)),
    countCompressed(static_cast<uint8_t>(84)), countCompressed(static_cast<uint8_t>(85)), countCompressed(static_cast<uint8_t>(86)), countCompressed(static_cast<uint8_t>(87)),
    countCompressed(static_cast<uint8_t>(88)), countCompressed(static_cast<uint8_t>(89)), countCompressed(static_cast<uint8_t>(90)), countCompressed(static_cast<uint8_t>(91)),
    countCompressed(static_cast<uint8_t>(92)), countCompressed(static_cast<uint8_t>(93)), countCompressed(static_cast<uint8_t>(94)), countCompressed(static_cast<uint8_t>(95)),
    countCompressed(static_cast<uint8_t>(96)), countCompressed(static_cast<uint8_t>(97)), countCompressed(static_cast<uint8_t>(98)), countCompressed(static_cast<uint8_t>(99)),
    countCompressed(static_cast<uint8_t>(100)), countCompressed(static_cast<uint8_t>(101)), countCompressed(static_cast<uint8_t>(102)), countCompressed(static_cast<uint8_t>(103)),
    countCompressed(static_cast<uint8_t>(104)), countCompressed(static_cast<uint8_t>(105)), countCompressed(static_cast<uint8_t>(106)), countCompressed(static_cast<uint8_t>(107)),
    countCompressed(static_cast<uint8_t>(108)), countCompressed(static_cast<uint8_t>(109)), countCompressed(static_cast<uint8_t>(110)), countCompressed(static_cast<uint8_t>(111)),
    countCompressed(static_cast<uint8_t>(112)), countCompressed(static_cast<uint8_t>(113)), countCompressed(static_cast<uint8_t>(114)), countCompressed(static_cast<uint8_t>(115)),
    countCompressed(static_cast<uint8_t>(116)), countCompressed(static_cast<uint8_t>(117)), countCompressed(static_cast<uint8_t>(118)), countCompressed(static_cast<uint8_t>(119)),
    countCompressed(static_cast<uint8_t>(120)), countCompressed(static_cast<uint8_t>(121)), countCompressed(static_cast<uint8_t>(122)), countCompressed(static_cast<uint8_t>(123)),
    countCompressed(static_cast<uint8_t>(124)), countCompressed(static_cast<uint8_t>(125)), countCompressed(static_cast<uint8_t>(126)), countCompressed(static_cast<uint8_t>(127)),
    countCompressed(static_cast<uint8_t>(128)), countCompressed(static_cast<uint8_t>(129)), countCompressed(static_cast<uint8_t>(130)), countCompressed(static_cast<uint8_t>(131)),
    countCompressed(static_cast<uint8_t>(132)), countCompressed(static_cast<uint8_t>(133)), countCompressed(static_cast<uint8_t>(134)), countCompressed(static_cast<uint8_t>(135)),
    countCompressed(static_cast<uint8_t>(136)), countCompressed(static_cast<uint8_t>(137)), countCompressed(static_cast<uint8_t>(138)), countCompressed(static_cast<uint8_t>(139)),
    countCompressed(static_cast<uint8_t>(140)), countCompressed(static_cast<uint8_t>(141)), countCompressed(static_cast<uint8_t>(142)), countCompressed(static_cast<uint8_t>(143)),
    countCompressed(static_cast<uint8_t>(144)), countCompressed(static_cast<uint8_t>(145)), countCompressed(static_cast<uint8_t>(146)), countCompressed(static_cast<uint8_t>(147)),
    countCompressed(static_cast<uint8_t>(148)), countCompressed(static_cast<uint8_t>(149)), countCompressed(static_cast<uint8_t>(150)), countCompressed(static_cast<uint8_t>(151)),
    countCompressed(static_cast<uint8_t>(152)), countCompressed(static_cast<uint8_t>(153)), countCompressed(static_cast<uint8_t>(154)), countCompressed(static_cast<uint8_t>(155)),
    countCompressed(static_cast<uint8_t>(156)), countCompressed(static_cast<uint8_t>(157)), countCompressed(static_cast<uint8_t>(158)), countCompressed(static_cast<uint8_t>(159)),
    countCompressed(static_cast<uint8_t>(160)), countCompressed(static_cast<uint8_t>(161)), countCompressed(static_cast<uint8_t>(162)), countCompressed(static_cast<uint8_t>(163)),
    countCompressed(static_cast<uint8_t>(164)), countCompressed(static_cast<uint8_t>(165)), countCompressed(static_cast<uint8_t>(166)), countCompressed(static_cast<uint8_t>(167)),
    countCompressed(static_cast<uint8_t>(168)), countCompressed(static_cast<uint8_t>(169)), countCompressed(static_cast<uint8_t>(170)), countCompressed(static_cast<uint8_t>(171)),
    countCompressed(static_cast<uint8_t>(172)), countCompressed(static_cast<uint8_t>(173)), countCompressed(static_cast<uint8_t>(174)), countCompressed(static_cast<uint8_t>(175)),
    countCompressed(static_cast<uint8_t>(176)), countCompressed(static_cast<uint8_t>(177)), countCompressed(static_cast<uint8_t>(178)), countCompressed(static_cast<uint8_t>(179)),
    countCompressed(static_cast<uint8_t>(180)), countCompressed(static_cast<uint8_t>(181)), countCompressed(static_cast<uint8_t>(182)), countCompressed(static_cast<uint8_t>(183)),
    countCompressed(static_cast<uint8_t>(184)), countCompressed(static_cast<uint8_t>(185)), countCompressed(static_cast<uint8_t>(186)), countCompressed(static_cast<uint8_t>(187)),
    countCompressed(static_cast<uint8_t>(188)), countCompressed(static_cast<uint8_t>(189)), countCompressed(static_cast<uint8_t>(190)), countCompressed(static_cast<uint8_t>(191)),
    countCompressed(static_cast<uint8_t>(192)), countCompressed(static_cast<uint8_t>(193)), countCompressed(static_cast<uint8_t>(194)), countCompressed(static_cast<uint8_t>(195)),
    countCompressed(static_cast<uint8_t>(196)), countCompressed(static_cast<uint8_t>(197)), countCompressed(static_cast<uint8_t>(198)), countCompressed(static_cast<uint8_t>(199)),
    countCompressed(static_cast<uint8_t>(200)), countCompressed(static_cast<uint8_t>(201)), countCompressed(static_cast<uint8_t>(202)), countCompressed(static_cast<uint8_t>(203)),
    countCompressed(static_cast<uint8_t>(204)), countCompressed(static_cast<uint8_t>(205)), countCompressed(static_cast<uint8_t>(206)), countCompressed(static_cast<uint8_t>(207)),
    countCompressed(static_cast<uint8_t>(208)), countCompressed(static_cast<uint8_t>(209)), countCompressed(static_cast<uint8_t>(210)), countCompressed(static_cast<uint8_t>(211)),
    countCompressed(static_cast<uint8_t>(212)), countCompressed(static_cast<uint8_t>(213)), countCompressed(static_cast<uint8_t>(214)), countCompressed(static_cast<uint8_t>(215)),
    countCompressed(static_cast<uint8_t>(216)), countCompressed(static_cast<uint8_t>(217)), countCompressed(static_cast<uint8_t>(218)), countCompressed(static_cast<uint8_t>(219)),
    countCompressed(static_cast<uint8_t>(220)), countCompressed(static_cast<uint8_t>(221)), countCompressed(static_cast<uint8_t>(222)), countCompressed(static_cast<uint8_t>(223)),
    countCompressed(static_cast<uint8_t>(224)), countCompressed(static_cast<uint8_t>(225)), countCompressed(static_cast<uint8_t>(226)), countCompressed(static_cast<uint8_t>(227)),
    countCompressed(static_cast<uint8_t>(228)), countCompressed(static_cast<uint8_t>(229)), countCompressed(static_cast<uint8_t>(230)), countCompressed(static_cast<uint8_t>(231)),
    countCompressed(static_cast<uint8_t>(232)), countCompressed(static_cast<uint8_t>(233)), countCompressed(static_cast<uint8_t>(234)), countCompressed(static_cast<uint8_t>(235)),
    countCompressed(static_cast<uint8_t>(236)), countCompressed(static_cast<uint8_t>(237)), countCompressed(static_cast<uint8_t>(238)), countCompressed(static_cast<uint8_t>(239)),
    countCompressed(static_cast<uint8_t>(240)), countCompressed(static_cast<uint8_t>(241)), countCompressed(static_cast<uint8_t>(242)), countCompressed(static_cast<uint8_t>(243)),
    countCompressed(static_cast<uint8_t>(244)), countCompressed(static_cast<uint8_t>(245)), countCompressed(static_cast<uint8_t>(246)), countCompressed(static_cast<uint8_t>(247)),
    countCompressed(static_cast<uint8_t>(248)), countCompressed(static_cast<uint8_t>(249)), countCompressed(static_cast<uint8_t>(250)), countCompressed(static_cast<uint8_t>(251)),
    countCompressed(static_cast<uint8_t>(252)), countCompressed(static_cast<uint8_t>(253)), countCompressed(static_cast<uint8_t>(254)), countCompressed(static_cast<uint8_t>(255))
};

static_assert(extractACount(countTable[0]) == 4, "countTable is incorrect");
static_assert(extractCCount(countTable[0]) == 0, "countTable is incorrect");
static_assert(extractGCount(countTable[0]) == 0, "countTable is incorrect");
static_assert(extractTCount(countTable[0]) == 0, "countTable is incorrect");

constexpr uint8_t testCompressed = static_cast<uint8_t>(1 | 3 << 2 | 0 << 4 | 3 << 6);

static_assert(extractACount(countTable[testCompressed]) == 1, "countTable is incorrect");
static_assert(extractCCount(countTable[testCompressed]) == 1, "countTable is incorrect");
static_assert(extractGCount(countTable[testCompressed]) == 0, "countTable is incorrect");
static_assert(extractTCount(countTable[testCompressed]) == 2, "countTable is incorrect");

std::string loadFile(const std::string& path)
{
    std::ifstream inFile(path);
    std::string result((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    return result;
}

std::vector<uint8_t> loadBinFile(const std::string& path)
{
    std::ifstream inFile(path, std::ios::binary);

    std::vector<uint8_t> result((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    return result;
}

int main()
{
    std::cout << "loading file\n";
    //std::string data = loadFile("../tools_datageneration/textout.txt");
    std::vector<uint8_t> data = loadBinFile("../tools_datageneration/binout.txt");

    int32_t aAmount = 0;
    int32_t cAmount = 0;
    int32_t gAmount = 0;
    int32_t tAmount = 0;

    std::cout << "counting\n";

    /*
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
    */

    uint64_t compressedCount = 0;
    int32_t currentCompressedCountCount = 0;

    for(uint8_t compressed : data)
    {
        compressedCount += countTable[compressed];
        ++currentCompressedCountCount;

        if(currentCompressedCountCount >= std::numeric_limits<uint16_t>::max() / 4)
        {
            aAmount += extractACount(compressedCount);
            cAmount += extractCCount(compressedCount);
            gAmount += extractGCount(compressedCount);
            tAmount += extractTCount(compressedCount);

            compressedCount = 0;
            currentCompressedCountCount = 0;
        }
        //for(int8_t charInByte = 0; charInByte < 8; charInByte += 2)
        //{
        //    uint8_t nucleotideIndex = (compressed >> charInByte) & 0b00000011;

        //    switch(nucleotideIndex)
        //    {
        //        case 0:
        //            ++aAmount;
        //            break;
        //        case 1:
        //            ++cAmount;
        //            break;
        //        case 2:
        //            ++gAmount;
        //            break;
        //        case 3:
        //            ++tAmount;
        //            break;
        //    }
        //}
    }

    aAmount += extractACount(compressedCount);
    cAmount += extractCCount(compressedCount);
    gAmount += extractGCount(compressedCount);
    tAmount += extractTCount(compressedCount);

    std::cout << aAmount << " " << cAmount << " " << gAmount << " " << tAmount << "\n";
}
