/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 21:40:45 by okoca             #+#    #+#             */
/*   Updated: 2024/10/03 08:16:51 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <cstddef>
#include <stdexcept>
#include <sys/types.h>
#include <dirent.h>

const std::string Response::_html_template = "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>Directory Listing</title>\n"
    "    <style>\n"
    "        body {\n"
    "            font-family: Arial, sans-serif;\n"
    "            line-height: 1.6;\n"
    "            color: #333;\n"
    "            max-width: 800px;\n"
    "            margin: 0 auto;\n"
    "            padding: 20px;\n"
    "        }\n"
    "        h1 {\n"
    "            color: #2c3e50;\n"
    "            border-bottom: 2px solid #ecf0f1;\n"
    "            padding-bottom: 10px;\n"
    "        }\n"
    "        ul {\n"
    "            list-style-type: none;\n"
    "            padding: 0;\n"
    "        }\n"
    "        li {\n"
    "            margin-bottom: 10px;\n"
    "            padding: 10px;\n"
    "            background-color: #f8f9fa;\n"
    "            border-radius: 5px;\n"
    "            transition: background-color 0.2s;\n"
    "        }\n"
    "        li:hover {\n"
    "            background-color: #e9ecef;\n"
    "        }\n"
    "        a {\n"
    "            color: #3498db;\n"
    "            text-decoration: none;\n"
    "        }\n"
    "        a:hover {\n"
    "            text-decoration: underline;\n"
    "        }\n"
    "        .file-icon, .folder-icon {\n"
    "            margin-right: 10px;\n"
    "        }\n"
    "        .file-icon::before {\n"
    "            content: \"📄\";\n"
    "        }\n"
    "        .folder-icon::before {\n"
    "            content: \"📁\";\n"
    "        }\n"
    "    </style>\n"
    "</head>\n"
    "<body>\n"
    "    <h1>Directory Listing</h1>\n"
    "    <ul>\n"
    "        <!-- {DIRECTORY_ITEMS} -->\n"
    "    </ul>\n"
    "</body>\n"
    "</html>";

std::string Response::directory_listing(const std::string &path, const std::string &file)
{
	std::string content = _html_template;
	std::string	items;
	DIR*		dir;
	dirent*		ent;

    if ((dir = opendir(path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string name = ent->d_name;
			// currently has issues with going back
			// if (name == "." || name == "..")
			// 	continue ;
			std::string icon = (ent->d_type == DT_DIR) ? "folder-icon": "file-icon";
			items += "<li><span class=\"" + icon + "\"></span><a href=\"" + file + '/' + name + "\">" + name + "</a></li>\n";
		}
		closedir(dir);
	}
    else
    {
        throw std::runtime_error("directory could not be opened");
    }

	size_t	pos = content.find("<!-- {DIRECTORY_ITEMS} -->");
	content.replace(pos, 28, items);

	return content;
}
