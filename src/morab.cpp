// morab.cpp : Defines the entry point for the application.
//

#include "morab.h"

#ifdef _WIN32
#include "wgetopt.hpp"
#endif


void run_single(const std::string& url) {
    const auto res = takeout::morab::object().get_html(url);
    auto results = takeout::extract_images(res);

    const auto fn = takeout::extract_filename_from_url(url, true);
    std::cout << "Target Directory: " << fn << std::endl;

    takeout::create_directory(fn, true);

    for (const auto& image_url : results)
    {
        const auto name = takeout::extract_filename_from_url(image_url);
        const auto joined = takeout::combine_path(fn, name);
        takeout::morab::object().download(image_url, joined);
        std::cout << "Downloaded: " << image_url << std::endl;
    }
}


int main(const int argc, char* argv[])
{
	std::string path;
	auto server_mode = 0;

	auto opt = -1;
    while (true) {
        static struct option options[] = {
            {"verbose", no_argument, nullptr, 'v'},
            {"config", required_argument, nullptr, 'c'},
            {"daemon", no_argument, nullptr, 'd'},
            {nullptr, 0, nullptr, 0}
        };
        auto option_index = 0;

        opt = getopt_long(argc, argv, "vc:d", options, &option_index);
        if (-1 == opt)
            break;

        switch (opt)
        {
        case 0:
            break;

        case 'v':
            takeout::morab::object().set_verbose(true);
            break;

        case 'c':
            path = std::string(optarg);
            takeout::morab::object().setup(path);
            std::cout << "Loaded config file from: " << path << std::endl;

            break;
        case 'd':
            server_mode = true;
            break;

        case '?':
            break;

        default:
            abort();
        }
    }

    char* sources_list = nullptr;
    if (optind < argc)  // Has additional non-option argv elements
    {  
        sources_list = argv[optind];
        if (nullptr == sources_list) {
            return EXIT_FAILURE;
        }
    }

	if (!takeout::morab::object().settings().chdir().empty())
	    takeout::morab::object().change_directory(takeout::morab::object().settings().chdir());

    std::cout << "Working directory: " << takeout::morab::get_current_working_dir() << std::endl;
	std::cout << "Proxy: " << takeout::morab::object().settings().proxy_address() << std::endl;

    std::string url;
    if ((!server_mode) && nullptr == sources_list) {
#ifdef __GENERIC_UNIX__
        if (!isatty(STDIN_FILENO)) {  // Not a console.
#elif defined(_WIN32)
        auto* const stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
        if (INVALID_HANDLE_VALUE == stdin_handle)
            return GetLastError();  // EXIT_FAILURE

        if (FILE_TYPE_CHAR != GetFileType(stdin_handle))  // Not a console.
        {
#endif
            while (std::getline(std::cin, url)) {
                if (url.empty())
                    break;

                std::cout << "Received Task: " << url << std::endl;
                run_single(url);
            }
        }
        return EXIT_SUCCESS;
    }
    else if (nullptr != sources_list) {
        std::ifstream sources(sources_list);
        if (!sources.is_open())
            return EXIT_FAILURE;

        std::cout << "Loaded sources from: " << sources_list << std::endl;

        while (std::getline(sources, url)) {
            if (url.empty())
                break;

            std::cout << "Received Task: " << url << std::endl;
            run_single(url);
        }

        return EXIT_SUCCESS;
    }

    // Remove hard-coded URL, placeholder for future server_mode.

	const auto res = takeout::morab::object().get_html(url);
	auto results = takeout::extract_images(res);

	const auto fn = takeout::extract_filename_from_url(url, true);
	std::cout << "Target Directory: " << fn << std::endl;

    takeout::create_directory(fn, true);

	for (const auto& image_url : results)
	{
		const auto name = takeout::extract_filename_from_url(image_url);
		const auto joined = takeout::combine_path(fn, name);

		auto future = takeout::morab::object().pool.submit([](const std::string url_, const std::string path_)
		{
		    takeout::morab::object().download(url_, path_);
			std::cout << "Downloaded: " << url_ << std::endl;
		}, image_url, joined);

		// TODO: Wait futures.
	}

	return EXIT_SUCCESS;
}
