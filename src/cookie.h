#pragma once

#ifndef MORAB_COOKIE_H
#define MORAB_COOKIE_H

#include "stdafx.h"

#define UTC_DATETIME_SIZE 30

typedef enum {
    SAME_SITE_NOT_CONFIGURED = 0,
    SAME_SITE_DEFAULT_MODE,
    SAME_SITE_LAX_MODE,
    SAME_SITE_STRICT_MODE,
    SAME_SITE_NONE_MODE
} same_site_t;


inline bool is_token(char c) {
    if (isalpha(c))
        return true;

    if (isdigit(c))
        return true;

    switch (c) {
    case '!':
    case '#':
    case '$':
    case '%':
    case '&':
    case '\'':
    case '*':
    case '+':
    case '-':
    case '.':
    case '^':
    case '_':
    case '`':
    case '|':
    case '~':
        return true;
    default:
        return false;
    }
}

inline bool valid_cookie_name(const std::string& name) {
    if (name.empty())
        return false;

    return std::all_of(name.begin(), name.end(), is_token);
}

inline bool valid_cookie_value(const std::string& value) {
    if (value.empty())
        return false;

    return std::all_of(value.begin(), value.end(), [](char c){
        return 0x20 <= c && c < 0x7f && c != '"' && c != ';' && c != '\\';
    });
}

// sanitize_cookie_value produces a suitable cookie-value from value.
// https://tools.ietf.org/html/rfc6265#section-4.1.1
// cookie-value      = *cookie-octet / ( DQUOTE *cookie-octet DQUOTE )
// cookie-octet      = %x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E
//           ; US-ASCII characters excluding CTLs,
//           ; whitespace DQUOTE, comma, semicolon,
//           ; and backslash
// We loosen this as spaces and commas are common in cookie values
// but we produce a quoted cookie-value if and only if value contains
// commas or spaces.
inline std::string sanitize_cookie_value(const std::string& value) {
    if (value.empty())
        return value;

    if (!((std::string::npos != value.find(' ')) || (std::string::npos != value.find(','))))
        return value;

    std::ostringstream oss;
    oss << '\"' << value << '\"';

    return oss.str();
}

inline bool valid_cookie_path(const std::string& path) {
    if (path.empty())
        return false;

    return std::all_of(path.begin(), path.end(), [](char c){
        return 0x20 <= c && c < 0x7f && c != ';';
    });
}

inline bool is_cookie_domain_name(const std::string& domain) {
    if (domain.empty())
        return false;

    if (255 < domain.length())
        return false;

    size_t index = 0;
    if ('.' == domain[0])
        ++index;  // Remove leading '.'

    char last = '.';
    bool ok = false;
    char part_length = 0;
    for (; index < domain.length(); ++index) {
        auto c = domain[index];

        if (isalpha(c) || isdigit(c)) {
            ok = true;
            ++part_length;
            last = c;
            continue;
        }

        if ('-' == c) {
            if ('.' == last)
                return false;

            ++part_length;
        }

        if ('.' == c) {
            if ('.' == last || '-' == last)
                return false;

            if (part_length > 63 || 0 == part_length)
                return false;

            part_length = 0;
        }

        last = c;
    }

    if ('-' == last || 63 < part_length)
        return false;

    return ok;
}

inline bool valid_cookie_domain(const std::string& domain) {
    if (is_cookie_domain_name(domain))
        return true;

    // TODO: Validate IP address.
    return false;
}

inline bool valid_cookie_expires(const struct tm& expires) {
    // IETF RFC 6265 Section 5.1.1.5, the year must not be less than 1601
    // However `struct tm` should have year larger than 1900
    return expires.tm_year >= 1900;
}


class cookie {
public:
    const size_t cookie_max_size = 4096;
    const char*  utc_date_time_format = "%a, %d %b %y %T %z";


    std::string key;
    std::string value;

    std::string path;
    std::string domain;
    struct tm   expires{};

    bool         secure    = false;
    unsigned int max_age   = 0;
    bool         http_only = false;
    same_site_t  same_site = SAME_SITE_NOT_CONFIGURED;

    std::string raw;

    cookie() = default;

    std::string str() const;

protected:
    char raw_expires_[UTC_DATETIME_SIZE];
    std::vector<std::string> unparsed_;

};

#endif //MORAB_COOKIE_H
