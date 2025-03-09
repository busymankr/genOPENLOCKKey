#ifndef SIERRAGENERATOR_H
#define SIERRAGENERATOR_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

class SierraGenerator
{
public:
    std::vector<uint8_t> tbl;
    std::vector<uint8_t> rtbl;

    SierraGenerator()
    {
        tbl.resize(256);
        rtbl.resize(10);
    }

    std::vector<int32_t> SierraPreInit(int32_t counter, int32_t challengelen, int32_t mcount)
    {
        int32_t tmp3 = 0;
        if (counter != 0)
        {
            int32_t tmp2 = 0;
            int32_t i = 1;
            while (i < counter)
            {
                i = 2 * i + 1;
            }

            std::vector<uint8_t> key = { 0x4F, 0x4D, 0x41, 0x52, 0x20, 0x44, 0x49, 0x44, 0x20, 0x54, 0x48, 0x49, 0x53, 0x2E, 0x2E, 0x2E };

            while (true)
            {
                int32_t tmp = mcount;
                mcount = tmp + 1;
                challengelen = (key[tmp & 0xFF] + tbl[challengelen & 0xFF]) & 0xFF;

                if (mcount >= 16)
                {
                    mcount = 0;
                    challengelen = ((challengelen & 0xFF) + 16) & 0xFF;
                }

                tmp2++;
                tmp3 = (challengelen & 0xFF) & i;

                if (tmp2 >= 0xB)
                {
                    tmp3 = (tmp3 != 0) ? counter % tmp3 : counter; // 0으로 나누는 경우 방지
                }

                if (tmp3 <= counter)
                {
                    break;
                }
            }
            counter = tmp3 & 0xFF;
        }

        return {counter, challengelen, mcount};
    }

    std::vector<int32_t> SierraInit(int32_t keylen)
    {
        for (int32_t i = 0; i < 256; i++)
        {
            tbl[i] = i & 0xFF;
        }

        int32_t mcount = 0;
        keylen = 16;
        int32_t cl = keylen & 0xFFFFFF00;
        int32_t i = 0xFF;

        while (i > -1)
        {
            std::vector<int32_t> result = SierraPreInit(i, cl, mcount);
            int32_t t = result[0];
            cl = result[1];
            mcount = result[2];

            int32_t m = tbl[i];
            tbl[i] = tbl[t & 0xFF];
            i -= 1;
            tbl[t & 0xFF] = m;
        }

        rtbl[0] = tbl[7];
        rtbl[1] = tbl[3];
        rtbl[2] = tbl[cl & 0xFF];
        rtbl[3] = tbl[1];
        rtbl[4] = tbl[5];

        return {1, keylen};
    }

    uint8_t SierraAlgo(uint8_t challenge)
    {
        uint8_t v6 = rtbl[3];
        uint8_t v0 = (v6 + 1) & 0xFF;
        rtbl[3] = v0;

        rtbl[1] = (tbl[v6] + rtbl[1]) & 0xFF;
        uint8_t v4 = rtbl[1] & 0xFF;
        uint8_t v2 = rtbl[2] & 0xFF;
        uint8_t v1 = tbl[v2];

        tbl[v2] = tbl[v4];
        uint8_t v5 = rtbl[0] & 0xFF;
        tbl[v4] = tbl[v5];
        tbl[v5] = tbl[v0];
        tbl[v0] = v1;

        uint8_t u = tbl[(tbl[(tbl[(rtbl[4] + tbl[v1]) & 0xFF] + tbl[v5] + tbl[v2]) & 0xFF]) & 0xFF];
        uint8_t v = tbl[(tbl[v4] + v1) & 0xFF];

        rtbl[2] = u ^ v ^ challenge;
        rtbl[4] = (tbl[v1] + rtbl[4]) & 0xFF;
        rtbl[0] = challenge;

        return rtbl[2];
    }

    std::vector<uint8_t> SierraKeygen(const std::vector<uint8_t> &challenge)
    {
        SierraInit(16);
        std::vector<uint8_t> response;

        for (uint8_t c : challenge)
        {
            response.push_back(SierraAlgo(c));
        }

        return response;
    }

    // Hex 변환 함수
    static std::vector<uint8_t> unhexlify(const std::string &hex)
    {
        std::vector<uint8_t> output;
        for (size_t i = 0; i < hex.length(); i += 2)
        {
            uint8_t byte = stoi(hex.substr(i, 2), nullptr, 16);
            output.push_back(byte);
        }
        return output;
    }

    static std::string hexlify(const std::vector<uint8_t> &input)
    {
        std::stringstream ss;
        for (uint8_t byte : input)
        {
            ss << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int)byte;
        }
        return ss.str();
    }
};

#endif // SIERRAGENERATOR_H
