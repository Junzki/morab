
#include "morab.h"

const char path_sep = '/';
const char end_needle = '.';


auto
takeout::extract_filename_from_url(const std::string& in,
                                   bool remove_ext) -> std::string
{
    auto begin = 0;
    auto end = in.length() - 1;

    auto tail = in.length();
    if (path_sep == in[tail - 1])
        tail -= 1;

    for (auto i = 0; i < tail; ++i)
    {
        if (path_sep == in[i])
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
takeout::create_directory(const std::string& path, bool exist_ok)
{
#ifdef _WIN32
    const auto* path_name = path.c_str();
    const auto result = CreateDirectoryA(path_name, nullptr);

    if (0 == result)
    {
        const auto err = GetLastError();
        if (ERROR_ALREADY_EXISTS == err && exist_ok)
            return;

        throw err;
    }
#elif defined(__GENERIC_UNIX__)
    const auto path_ = path.c_str();
    if (0 != mkdir(path_, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {
        if (EEXIST == errno and exist_ok) {
            struct stat stat_{};
            stat(path_, &stat_);

            // Check if target file is a directory.
            if (S_ISDIR(stat_.st_mode))
                return;
        }

        throw errno;
    }  // mode: 0775
#else
    throw "Cannot create directory, method not implmented.";
#endif
}

auto
takeout::combine_path(const std::string& base,
                      const std::string& extra) -> std::string
{
#ifdef _WIN32
    auto* buf = new char[max_path];
    const auto* result = PathCombineA(buf, base.c_str(), extra.c_str());
    return std::string(result);
#else
    return base + path_sep + extra;
#endif
}

void
takeout::morab::change_directory(const std::string &path) {
    const auto* path_ = path.c_str();

#ifdef _WIN32
    if (0 == SetCurrentDirectory(path_))
    {
        const auto err = GetLastError();
        throw err;
    }
#elif defined(__GENERIC_UNIX__)
    if (0 != chdir(path_)) {
        throw errno;
    }
#endif
}

std::string
takeout::morab::get_current_working_dir() {
    auto* buf = new char[max_path];

#ifdef _WIN32
    if (0 == GetCurrentDirectory(max_path, buf))
    {
        const auto err = GetLastError();
        throw err;
    }
#elif defined(__GENERIC_UNIX__)
    getcwd(buf, max_path);
#endif

    auto cwd = std::string(buf);
    delete[] buf;
    return cwd;
}
