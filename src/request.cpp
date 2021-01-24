

#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"

#include "morab.h"

auto
takeout::get_html(const std::string& url) -> std::string
{
    curlpp::Easy request;
    auto* out_stream = new std::ostringstream();

    request.setOpt(new curlpp::options::Url(url));

    request.setOpt(new curlpp::options::UserAgent(config::get_instance().user_agent()));
    request.setOpt(new curlpp::options::WriteStream(out_stream));

    if (config::get_instance().proxy())
        request.setOpt(new curlpp::options::Proxy(config::get_instance().proxy_address()));

    request.perform();

    auto response = out_stream->str();
    return response;
}


void
takeout::download(const std::string& url, const std::string& path)
{
    curlpp::Easy request;

    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);
    if (!f.is_open()) return;

    request.setOpt(new curlpp::options::Url(url));

    request.setOpt(new curlpp::options::UserAgent(config::get_instance().user_agent()));
    request.setOpt(new curlpp::options::WriteStream(&f));

    if (config::get_instance().proxy())
        request.setOpt(new curlpp::options::Proxy(config::get_instance().proxy_address()));

    request.perform();
    f.close();
}
