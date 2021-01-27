#pragma once


#ifndef MORAB_CONFIG_H
#define MORAB_CONFIG_H

#include "stdafx.h"

using json = nlohmann::json;

namespace takeout
{
    const std::string default_user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:84.0) Gecko/20100101 Firefox/84.0";

    class config
    {
    public:
        config() = default;
        ~config() = default;

        void read(const std::string&);

        auto proxy() const { return this->proxy_; }
        auto proxy_address() const { return this->proxy_address_; }
        auto user_agent() const { return this->user_agent_; }
        auto chdir() const { return this->chdir_; }

        auto verbose() const { return this->verbose_; }
        void set_verbose(const bool mode)
        {
            this->verbose_ = mode;
        }

    protected:
        bool proxy_ = false;
        std::string proxy_address_;
        std::string user_agent_ = default_user_agent;
        std::string chdir_;
        bool verbose_ = false;
    };
}

#endif
