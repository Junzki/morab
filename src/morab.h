// takeout.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#ifndef MORAB_H
#define MORAB_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>

#ifdef WIN32
#include <Windows.h>
#include <Shlwapi.h>
#endif

namespace takeout
{
    using json = nlohmann::json;

    class config
    {
    public:
        config(config& other) = delete;
        void operator=(const config&) = delete;

        ~config() = default;

        static config& get_instance()
        {
            static config instance;

            return instance;
        }

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
            if (! this->target_dir_.empty())
                return this->target_dir_;

            return std::string();
        }

    protected:
        config() = default;

        bool proxy_ = false;
        std::string proxy_address_;
        std::string user_agent_ = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:84.0) Gecko/20100101 Firefox/84.0";
        std::string target_dir_;
    };



    auto get_html(const std::string&) -> std::string;
    void download(const std::string&, const std::string&);

    auto extract_images(const std::string&) -> std::vector<std::string>;

    std::string extract_filename_from_url(const std::string& in, const bool remove_ext = false);
    void mkdir(const std::string& path, const bool exist_ok = false);
    std::string combine_path(const std::string&, const std::string&);
}


#endif // MORAB_H
