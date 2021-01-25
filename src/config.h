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

        void read(const std::string& path)
        {
            std::ifstream f;
            f.open(path, std::ios::in);

            if (!f.is_open()) return;

            std::ostringstream ss;
            ss << f.rdbuf();

            auto parsed = json::parse(ss.str());
            this->proxy_ = parsed["proxy"];
            this->proxy_address_ = parsed["proxy_address"];
            //this->target_dir_ = parsed["target_dir"];
        }

        auto proxy() const { return this->proxy_; }
        auto proxy_address() const { return this->proxy_address_; }
        auto user_agent() const { return this->user_agent_; }
        auto target_dir() const
        {
            if (!this->target_dir_.empty())
                return this->target_dir_;

            return std::string();
        }

    protected:
        bool proxy_ = false;
        std::string proxy_address_;
        std::string user_agent_ = default_user_agent;
        std::string target_dir_;
    };
}

#endif