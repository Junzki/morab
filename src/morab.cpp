// morab.cpp : Defines the entry point for the application.
//

#include "morab.h"

#ifdef _WIN32
#include "wgetopt.hpp"
#endif

std::atomic_int exit_flag;
std::thread server_thread;


void clean_url(std::string& url) {
    if (url.find("//") == 0) {
        url.insert(0, "http:");
    }
}


std::thread::id
run_single(const std::string& url) {
    const auto res = takeout::morab::object().get_html(url);
    auto results = takeout::extract_images(res);

    const auto fn = takeout::extract_filename_from_url(url, true);
    std::cout << "Target Directory: " << fn << std::endl;

    takeout::create_directory(fn, true);

    for (auto& image_url : results)
    {
        clean_url(image_url);
        const auto name = takeout::extract_filename_from_url(image_url);
        const auto joined = takeout::combine_path(fn, name);
        takeout::morab::object().download(image_url, joined);
        std::cout << "Downloaded: " << image_url << std::endl;
    }

    return std::this_thread::get_id();
}


void
handle_exit(int signo)
{
    if (0 < exit_flag) {
        std::cout << "Ctrl-C received, shutting down gracefully.";
        --exit_flag;
    } else {
        std::cout << "Multiple Ctrl-C received, force quit.";
        exit(EXIT_SUCCESS);
    }
}


std::thread::id
spawn_stdin()
{
    const auto thread_id = std::this_thread::get_id();
    std::string url;
    while (std::getline(std::cin, url)) {
        if (url.empty())
        {
            if (! exit_flag)
            {
                return thread_id;
            }

            continue;
        }

        std::cout << "Received Task: " << url << std::endl;
        takeout::morab::object().pool.submit(run_single, url);
    }

    return thread_id;
}


int main(const int argc, char* argv[])
{
    ++exit_flag;
    signal(SIGINT, handle_exit);

	std::string path;
	auto server_mode = 1;

	auto opt = -1;
    while (true) {
        static struct option options[] = {
            {"verbose", no_argument,       nullptr, 'v'},
            {"config",  required_argument, nullptr, 'c'},
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

        case '?':
            break;

        default:
            abort();
        }
    }

    std::list<std::string> sources;
    if (optind < argc)  // Has additional non-option argv elements
    {
        while (optind < argc)
        {
            const auto* src = argv[optind];
            if (nullptr == src)
                continue;

            sources.emplace_back(src);
            server_mode = 0;
            optind++;
        }
    }

    // Change working directory.
	if (!takeout::morab::object().settings().chdir().empty())
	    takeout::morab::object().change_directory(takeout::morab::object().settings().chdir());

    std::cout << "Working directory: " << takeout::morab::get_current_working_dir() << std::endl;
	std::cout << "Proxy: " << takeout::morab::object().settings().proxy_address() << std::endl;

    // Emit tasks.
    std::list<std::future<std::thread::id>> futures;
    if (server_mode && sources.empty()) {
        server_thread = std::thread(spawn_stdin);
    }
    else if (! sources.empty()) {
        std::string url;
        for (const auto& source : sources)
        {
            if (takeout::is_url(source))
            {
                std::cout << "Received Task: " << source << std::endl;
                futures.emplace_back(takeout::morab::object().pool.submit(run_single, source));
            }

            std::ifstream f(source);
            if (!f.is_open())
            {
                std::cout << "Cannot open file: " << source << " , skipped" << std::endl;
                continue;
            }

            std::cout << "Loaded sources from: " << source << std::endl;

            while (std::getline(f, url)) {
                if (url.empty())
                    break;

                std::cout << "Received Task: " << source << std::endl;
                futures.emplace_back(takeout::morab::object().pool.submit(run_single, source));
            }
        }
    }

    if (server_thread.joinable())
        server_thread.join();

    for (auto& future : futures)
    {
        future.wait();
    }

    return EXIT_SUCCESS;
}
