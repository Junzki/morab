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
	bool server_mode = false;
	auto opt = -1;

	while ((opt = getopt(argc, argv, "c:d")) != -1)
	{
        switch (opt)
        {
		case 'c':
			path = std::string(optarg);
			takeout::morab::object().setup(path);
			std::cout << "Loaded config file from: " << path << std::endl;

			break;
        case 'd':
            server_mode = true;
            break;

		default:
			break;
        }
	}

	if (!takeout::morab::object().settings().chdir().empty())
	    takeout::morab::object().change_directory(takeout::morab::object().settings().chdir());

    std::cout << "Working directory: " << takeout::morab::get_current_working_dir() << std::endl;
	std::cout << "Proxy: " << takeout::morab::object().settings().proxy_address() << std::endl;

    std::string url;
    if (!server_mode) {
#ifdef __GENERIC_UNIX__
        if (!isatty(STDIN_FILENO)) {
            while (std::getline(std::cin, url)) {
                if (url.empty())
                    break;

                std::cout << "Received Task: " << url << std::endl;
                run_single(url);
            }
        }
#endif
        return EXIT_SUCCESS;
    }

    url = "http://t66y.com/htm_mob/2101/8/4271499.html";

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
