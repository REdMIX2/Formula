
#include <bits/stdc++.h>
#include <algorithm>
#include <functional>
#include <locale>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

bool filter(char i)
{
    switch (i)
    {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
        return false;
    default:
        return true;
    }
}
void strip(std::string &s)
{
    std::copy_if(s.begin(),
                 s.end(),
                 std::back_inserter(s),
                 filter);
}
void UtilsRemove(string &s)
{
    for (auto it = s.begin(); it != s.end();)
    {
        if (*it == ' ' || *it == '\t' || *it == '\n' || *it == '\r')
        {
            it = s.erase(it);
        }
        else
        {
            it++;
        }
    }
}

std::vector<size_t> findPositions(const std::string &s, const std::string &substr, size_t startPos = 0)
{
    std::vector<size_t> positions;
    size_t i = s.find(substr, startPos);
    while (i != std::string::npos)
    {
        positions.push_back(i);
        i = s.find(substr, i + substr.size());
    }
    return positions;
}
uint count(const std::string &s, const std::string &substr, size_t pos = 0)
{
    size_t i = s.find(substr, pos);
    size_t cnt = 0;
    while (i != std::string::npos)
    {
        ++cnt;
        i = s.find(substr, i + substr.size());
    }
    return cnt;
}

void replace(std::string &s, const std::string &before, const std::string &after)
{
    const size_t sizeBefore = before.size();
    const size_t sizeAfter = after.size();

    if (sizeBefore > sizeAfter)
    {
        size_t preLastIndex = 0;
        size_t newSize = 0;

        for (size_t i = s.find(before); i != std::string::npos; i = s.find(before, i + sizeBefore))
        {
            // copy last notBeforeStr
            std::memcpy(&s[newSize], &s[preLastIndex], i - preLastIndex);
            newSize += (i - preLastIndex);
            // copy afterStr
            std::memcpy(&s[newSize], &after[0], sizeAfter);
            newSize += sizeAfter;

            preLastIndex = i + sizeBefore;
        }
        // copy last notBeforeStr
        std::memcpy(&s[newSize], &s[preLastIndex], s.size() - preLastIndex);
        newSize += (s.size() - preLastIndex);

        s.resize(newSize);
    }
    else if (sizeBefore == sizeAfter)
    {
        for (size_t i = s.find(before); i != std::string::npos; i = s.find(before, i + sizeBefore))
        {
            std::memcpy(&s[i], &after[0], sizeAfter);
        }
    }
    else //(sizeBefore > sizeAfter)
    {    // Not optimize for flash

        std::vector<size_t> positions = findPositions(s, before);
        std::string sNew(s.size() + positions.size() * (sizeAfter - sizeBefore), '\0');

        size_t preLastIndex = 0;
        size_t newSize = 0;

        for (auto &pos : positions)
        {
            // copy last notBeforeStr
            std::memcpy(&sNew[newSize], &s[preLastIndex], pos - preLastIndex);
            newSize += (pos - preLastIndex);
            // copy afterStr
            std::memcpy(&sNew[newSize], &after[0], sizeAfter);
            newSize += sizeAfter;

            preLastIndex = pos + sizeBefore;
        }
        // copy last notBeforeStr
        std::memcpy(&sNew[newSize], &s[preLastIndex], s.size() - preLastIndex);
        s = std::move(sNew);
    }
}

int main()
{
    // std::string str;
    // str.resize(1000);
    // for (auto it = str.begin(); it != str.end();it++)
    // {
    //     if (rand() % 2)
    //     {
    //         *it = 'a';
    //     }
    //     else
    //     {
    //         *it = (rand() % 2) ? ' ' : '\t';
    //     }
    // }
    // string str2 = str;

    // auto start = system_clock::now();
    // strip(str2);
    // auto end = system_clock::now();
    // std::cout << "strip=" + to_string(duration_cast<nanoseconds>(start - end).count()) + "ns\n";

    // start = system_clock::now();
    // UtilsRemove(str);
    // end = system_clock::now();
    // std::cout << "UtilsRemove=" + to_string(duration_cast<nanoseconds>(start - end).count()) + "ns\n";
    // UtilsRemove=-78499ns
    // strip=-104078ns

    std::string s1 = "abcttttttabc dpab";
    std::string s2 = "abcttttttabc dpab";
    std::string s3 = "abcttttttabc dpab";
    replace(s1, "abc", "1");
    replace(s2, "b", "1");
    replace(s3, "a", "aaa");
    return 0;
}