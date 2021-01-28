#pragma once

#ifndef MORAB_SESSION_H
#define MORAB_SESSION_H

#include "stdafx.h"
#include "cookie.h"

namespace takeout {

    class session {
    public:

        const std::string default_user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:84.0) Gecko/20100101 Firefox/84.0";

        session() = default;

        void set_proxy(const std::string& proxy_address) {
            if (proxy_address.empty())
                return;

            this->proxy_address_ = proxy_address;
        }

        void set_verbose(const bool mode = true) {
            this->verbose_ = mode;
        }

        void set_user_agent(const std::string& agent) {
            if (agent.empty())
                return;

            this->user_agent_ = agent;
        }

        void set_header(const std::string& header, const std::string& value) {
            if (header.empty())
                return;

            std::stringstream ss;
            ss << header << ": " << value;

            this->headers_.push_back(ss.str());
        }


    protected:
        void configure_requester(cURLpp::Easy& easy) const {
            easy.setOpt(cURLpp::options::FollowLocation(true));
            easy.setOpt(cURLpp::options::UserAgent(this->user_agent_));

            easy.setOpt(cURLpp::options::Timeout(this->read_timeout_));

            // Proxy
            if (! this->proxy_address_.empty())
                easy.setOpt(cURLpp::options::Proxy(this->proxy_address_));

            easy.setOpt(cURLpp::options::Verbose(this->verbose_));

            // Headers
            if (! this->headers_.empty()) {
                easy.setOpt(cURLpp::options::HttpHeader(this->headers_));
            }

            // Cookies
            for (auto it = this->cookies_.begin(); it != cookies_.end(); ++it) {
                easy.setOpt(cURLpp::options::CookieList(*it));
            }
        }

    private:
        int read_timeout_ = 60;
        bool verbose_ = false;
        std::string proxy_address_;
        std::string user_agent_ = default_user_agent;

        std::list<std::string> headers_;
        std::list<std::string> cookies_;
    };
}


#endif //MORAB_SESSION_H
