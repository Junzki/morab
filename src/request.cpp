
#include "morab.h"


cURLpp::Easy*
takeout::morab::build_requester() const
{
    auto* request = new cURLpp::Easy();

    request->setOpt(new curlpp::options::UserAgent(this->config_.user_agent()));
    if (this->config_.proxy())
        request->setOpt(new cURLpp::options::Proxy(this->config_.proxy_address()));

    return request;
}


std::string
takeout::morab::get_html(const std::string& url) const
{
    auto* req = this->build_requester();
    std::ostringstream out_stream;

    req->setOpt(new curlpp::options::Url(url));
    req->setOpt(new curlpp::options::WriteStream(&out_stream));

    req->perform();

    auto response = out_stream.str();

    delete req;
    return response;
}


void
takeout::morab::download(const std::string& url, const std::string& path) const
{
    auto* req = this->build_requester();

    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);
    if (!f.is_open()) return;

    req->setOpt(new curlpp::options::Url(url));
    req->setOpt(new curlpp::options::WriteStream(&f));
    
    req->perform();
    f.close();

    delete req;
}
