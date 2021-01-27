
#include "morab.h"


void
takeout::morab::configure_requester(cURLpp::Easy& request) const
{
    request.setOpt(new curlpp::options::FollowLocation(true));
    request.setOpt(new curlpp::options::UserAgent(this->config_.user_agent()));

    request.setOpt(new cURLpp::options::Timeout(this->config_.read_timeout()));

    if (this->config_.proxy())
        request.setOpt(new cURLpp::options::Proxy(this->config_.proxy_address()));

    if (this->config_.verbose())
        request.setOpt(new curlpp::options::Verbose(true));
}


std::string
takeout::morab::get_html(const std::string& url) const
{
    cURLpp::Easy req;
    this->configure_requester(req);

    std::ostringstream out_stream;

    req.setOpt(new curlpp::options::Url(url));
    req.setOpt(new curlpp::options::WriteStream(&out_stream));

    req.perform();

    auto response = out_stream.str();

    return response;
}


void
takeout::morab::download(const std::string& url, const std::string& path) const
{
    cURLpp::Easy req;
    this->configure_requester(req);

    std::ofstream f;
    
    // TODO:
    //  If separated filename has no extension,
    // try to guess the extension from MIMEType
    // and rename generated file.

    f.open(path, std::ios::out | std::ios::binary);
    if (!f.is_open()) return;

    int retried = 0;
    while (retried < 3)
    {
        f.seekp(0);
        req.setOpt(new curlpp::options::Url(url));
        req.setOpt(new curlpp::options::WriteStream(&f));

        try {
            req.perform();
        }
        catch (const curlpp::LibcurlRuntimeError& e) {
            std::cout << "cURL Error: " << e.what() << std::endl;
            retried += 1;
            continue;
        }

        long status_code = 400;
        curlpp::infos::ResponseCode::get(req, status_code);
        if (400 <= status_code)
        {
            retried += 1;
            continue;
        }

        break;
    }

    double content_length = 0;
    curlpp::infos::ContentLengthDownload::get(req, content_length);
    std::cout << "Content-Length: " << content_length << std::endl;

    f.close();
}
