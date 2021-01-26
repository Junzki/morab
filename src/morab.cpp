// morab.cpp : Defines the entry point for the application.
//

#include "morab.h"

#ifdef _WIN32
#include "wgetopt.hpp"
#endif


int main(const int argc, char* argv[])
{
	std::string path;
	auto opt = -1;

	while ((opt = getopt(argc, argv, "c:")) != -1)
	{
        switch (opt)
        {
		case 'c':
			path = std::string(optarg);
			takeout::morab::object().setup(path);
			std::cout << "Loaded config file from: " << path << std::endl;

			break;
		default:
			break;
        }
	}

	std::cout << takeout::morab::object().settings().proxy_address() << std::endl;

    const std::string url = "http://t66y.com/htm_mob/2101/8/4271499.html";
	const auto res = takeout::morab::object().get_html(url);
	auto results = takeout::extract_images(res);

	const auto fn = takeout::extract_filename_from_url(url, true);
	std::cout << "Target Directory: " << fn << std::endl;

	takeout::mkdir(fn, true);

	for (const auto& image_url : results)
	{
		const auto name = takeout::extract_filename_from_url(image_url);
		const auto joined = takeout::combine_path(fn, name);

		takeout::morab::object().download(image_url, joined);
		std::cout << "Downloaded: " << image_url << std::endl;
	}

	return 0;
}
