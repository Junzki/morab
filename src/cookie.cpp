
#include "cookie.h"


using ::cookie;


std::string
cookie::str() const {

    if (! ::valid_cookie_name(this->key))
        return std::string();

    std::stringstream ss;
    ss << this->key << '=' << ::sanitize_cookie_value(this->value);

    if (::valid_cookie_path(this->path)) {
        ss << "; Path=" << this->path;
    }

    if (::valid_cookie_domain(this->domain))
        ss << "; Domain=" << this->domain;

    if (::valid_cookie_expires(this->expires)) {
        if (0 != strftime((char*) this->raw_expires_, UTC_DATETIME_SIZE, this->utc_date_time_format, &this->expires))
            ss << "; Expires=" << this->raw_expires_;
    }

    ss << "; Max-Age=" << std::to_string(max_age);

    if (this->http_only)
        ss << "; HttpOnly";

    if (this->secure)
        ss << "; Secure";

    switch (this->same_site) {
    case SAME_SITE_DEFAULT_MODE:
        ss << "; SameSite";
        break;
    case SAME_SITE_NONE_MODE:
        ss << "; SameSite=None";
        break;
    case SAME_SITE_LAX_MODE:
        ss << "; SameSite=Lax";
        break;
    case SAME_SITE_STRICT_MODE:
        ss << "; SameSite=Strict";
        break;
    default:
        break;
    }

    return ss.str();
}

std::string
cookie::set_cookie() const {
    std::ostringstream oss;
    oss << "Set-Cookie: " << this->str();

    return oss.str();
}
