
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

int main()
{
    std::string str;
    str.resize(1000);
    for (auto it = str.begin(); it != str.end();it++)
    {
        if (rand() % 2)
        {
            *it = 'a';
        }
        else
        {
            *it = (rand() % 2) ? ' ' : '\t';
        }
    }
    string str2 = str;

    auto start = system_clock::now();
    strip(str2);
    auto end = system_clock::now();
    std::cout << "strip=" + to_string(duration_cast<nanoseconds>(start - end).count()) + "ns\n";
    
    start = system_clock::now();
    UtilsRemove(str);
    end = system_clock::now();
    std::cout << "UtilsRemove=" + to_string(duration_cast<nanoseconds>(start - end).count()) + "ns\n";
// UtilsRemove=-78499ns
// strip=-104078ns
    return 0;
}