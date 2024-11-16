#include <algorithm>
#include <string>
#include <cstring>
using namespace std;

namespace formula
{
    namespace utils
    {
        template <typename Predicate>
        void strip(std::string &s, const Predicate addIf)
        {
            std::string sTmp(std::move(s));

            s.reserve(sTmp.length());
            std::copy_if(sTmp.begin(), sTmp.end(),
                         std::back_inserter(s),
                         addIf);
        }
        // template <typename _InputIterator, typename _OutputIterator,
        //           typename _Predicate>

        void replace(std::string &s, const std::string find, const std::string replace)
        {
            if (find.length() < replace.length())
            {
                const unsigned sizeReplace = replace.length();
                const std::string &cReplace = replace;

                std::size_t i = s.find(find);
                while (i != std::string::npos)
                {
                    std::memcpy(&s[i], &cReplace[0], sizeReplace);
                    i = s.find(find);
                }
            }
            else if (find.length() == replace.length())
            {
                const unsigned sizeReplace = replace.length();
                const std::string &cReplace = replace;

                std::size_t i = s.find(find);
                while (i != std::string::npos)
                {
                    std::memcpy(&s[i], &cReplace[0], sizeReplace);
                    i = s.find(find, i);
                }
            }
            else //(find.length() > replace.length())
            {
                const size_t sizeReplace = replace.size();
                const size_t sizeTarget = find.size();
                const std::string &cReplace = replace;// by optimization

                size_t indexOldStr = s.find(find);
                size_t indexNewStr = indexOldStr;
                while (indexOldStr != std::string::npos)
                {
                    std::memcpy(&s[indexNewStr], &cReplace[0], sizeReplace); // copy replace string
                    indexNewStr += sizeReplace;
                    indexOldStr += sizeTarget;

                    size_t nextFindIndexOldStr = s.find(find, indexOldStr);
                    size_t sizePart = (nextFindIndexOldStr != std::string::npos) ? (nextFindIndexOldStr - indexOldStr) : (s.size() - indexOldStr);
                    std::memcpy(&s[indexNewStr], &s[indexOldStr], sizePart); // copy last path source string
                    indexNewStr += sizePart;
                    indexOldStr = nextFindIndexOldStr;
                }
                // delete diff size find and replace substrings
                if (indexNewStr != std::string::npos)
                {
                    s.resize(indexNewStr + 1);
                }
            }
        }
    };
};