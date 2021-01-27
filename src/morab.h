
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

        void set_verbose(const bool mode = true) { this->config_.set_verbose(mode); }

        std::string get_html(const std::string&) const;
        void download(const std::string&, const std::string&) const;

        static std::string get_current_working_dir();
        static void change_directory(const std::string &path);

        ThreadPool pool;

        ~morab() = default;

        morab(morab&) = delete;
        void operator=(const morab&) = delete;

        morab(morab&&) = delete;
        morab& operator=(const morab&&) = delete;

    protected:
        void configure_requester(cURLpp::Easy&) const;

    private:
        morab() = default;
        config config_;
    };


    auto extract_images(const std::string&) -> std::vector<std::string>;

    std::string extract_filename_from_url(const std::string& in, bool remove_ext = false);
    void create_directory(const std::string& path, bool exist_ok = false);
    std::string combine_path(const std::string&, const std::string&);

    bool is_url(const std::string&);
}


#endif // MORAB_H
