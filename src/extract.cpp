

#include "morab.h"


const auto* target_tag  = (const xmlChar*)"img";
const auto* target_attr = (const xmlChar*)"ess-data";


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
            xmlStrEqual(target_tag, current->name) && \
            nullptr != xmlHasProp(current, target_attr))
        {
            auto* value = xmlGetProp(current, target_attr);
            if (nullptr == value)
                continue;

            const auto url = to_string(value);
            std::cout << "Found Url: " << url << std::endl;
            results.push_back(url);
            continue;
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

    search_element(entry->last, results);
    xmlFreeDoc(doc);
    return results;
}
