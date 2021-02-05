

#include "morab.h"


const auto* target_tag  = reinterpret_cast<const xmlChar *>("img");
const std::vector<const xmlChar*> attrs = {
        reinterpret_cast<const xmlChar *>("src"),
        reinterpret_cast<const xmlChar *>("ess-data")
};


auto to_string(xmlChar* in)
{
    auto out = std::string((const char*)in);
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);

    return out;
}


void
search_element(xmlNode* entry, std::vector<std::string>& results)
{
    for (auto* current = entry; current; current = current->next)
    {
        if (XML_ELEMENT_NODE == current->type && \
            xmlStrEqual(target_tag, current->name))
        {
            for (const auto* attr : attrs)
            {
//                if (nullptr == xmlHasProp(current, attr))
//                    continue;

                auto *value = xmlGetProp(current, attr);
                if (nullptr == value)
                    continue;

                auto url = to_string(value);
                if (!takeout::is_url(url))
                    continue;

                takeout::clean_url(url);
                std::cout << "Found Url: " << url << std::endl;
                results.push_back(url);
            }
        }

        search_element(current->children, results);
    }
}


std::vector<std::string>
takeout::extract_images(const std::string& content)
{
    auto* doc = htmlReadDoc((xmlChar*)content.c_str(), nullptr, nullptr, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
    auto* entry = xmlDocGetRootElement(doc);

    std::vector<std::string> results;

    search_element(entry, results);
    xmlFreeDoc(doc);
    return results;
}
