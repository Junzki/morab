
#include "morab.h"

const char begin_needle = '/';
const char end_needle = '.';

auto
takeout::extract_filename_from_url(const std::string& in,
                                   bool remove_ext) -> std::string
{
    auto begin = 0;
    auto end = in.length() - 1;

    auto tail = in.length();
    if (begin_needle == in[tail - 1])
        tail -= 1;

    for (auto i = 0; i < tail; ++i)
    {
        if (begin_needle == in[i])
            begin = i;

        if (end_needle == in[i])
            end = i;
    }

    begin += 1;

    if (!remove_ext)
        end = in.length();

    const auto size = end - begin;
    auto substr = in.substr(begin, size);

    return substr;
};

void
takeout::mkdir(const std::string& path, bool exist_ok)
{
#ifdef WIN32
    const auto* path_name = path.c_str();
    const auto result = CreateDirectoryA(path_name, nullptr);

    if (0 == result)
    {
        const auto err = GetLastError();
        if (ERROR_ALREADY_EXISTS == err && exist_ok)
            return;

        throw err;
    }
#endif
}

auto
takeout::combine_path(const std::string& base,
                      const std::string& extra) -> std::string
{
#ifdef WIN32
    auto* buf = new char[MAX_PATH];

    const auto* result = PathCombineA(buf, base.c_str(), extra.c_str());

    return std::string(result);
#else
    return std::string();
#endif
}
