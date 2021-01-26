
#pragma once

#ifndef MORAB_H
#define MORAB_H

#include "stdafx.h"
#include "config.h"

namespace takeout
{

    class morab
    {
    public:
        static morab& object()
        {
            static morab instance;
            return instance;
        }

        void setup(const std::string& path)
        {
            this->config_.read(path);
        }

        config settings() const
        {
            return this->config_;
        }

        std::string get_html(const std::string&) const;
        void download(const std::string&, const std::string&) const;

        ThreadPool pool;

        ~morab() = default;

        morab(morab&) = delete;
        void operator=(const morab&) = delete;

        morab(morab&&) = delete;
        morab& operator=(const morab&&) = delete;

    protected:
        cURLpp::Easy* build_requester() const;

    private:
        morab() = default;
        config config_;
    };


    auto extract_images(const std::string&) -> std::vector<std::string>;

    std::string extract_filename_from_url(const std::string& in, bool remove_ext = false);
    void mkdir(const std::string& path, bool exist_ok = false);
    std::string combine_path(const std::string&, const std::string&);
}


#endif // MORAB_H
