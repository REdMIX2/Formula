#include <algorithm>
#include <string>
#include <cstring>
using namespace std;

namespace formula
{
    namespace utils
    {
        template <typename Predicate>
        inline void strip(std::string &s, const Predicate addIf)
        {
            std::string sTmp(std::move(s));

            s.reserve(sTmp.length());
            std::copy_if(sTmp.begin(), sTmp.end(),
                         std::back_inserter(s),
                         addIf);
        }

        inline std::vector<size_t> findPositions(const std::string &s, const std::string &substr, size_t startPos = 0)
        {
            std::vector<size_t> positions;
            for (size_t i = s.find(substr, startPos); i != std::string::npos; i = s.find(substr, i + substr.size()))
            {
                positions.push_back(i);
            }
            return positions;
        }

        inline uint count(const std::string &s, const std::string &substr, size_t startPos = 0)
        {
            size_t cnt = 0;
            for (size_t i = s.find(substr, startPos); i != std::string::npos; i = s.find(substr, i + substr.size()))
            {
                ++cnt;
            }
            return cnt;
        }

        inline void replace(std::string &s, const std::string &before, const std::string &after)
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
                if (positions.empty())
                {
                    return;
                }

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
    };
};