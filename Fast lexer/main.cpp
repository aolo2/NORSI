//
// Created by aolo2 on 7/9/17.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "fast_lexer.h"

int main(int argc, char **argv) {
    std::string path(argv[1]);
    std::string output(argv[2]);
    std::ifstream r(path);

    std::pair<std::vector<Node>, std::vector<Way>>
            nodes_and_ways = get_nodes_and_ways(r);

    std::ofstream w(output);

    w << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" <<
      "<gexf version=\"1.1\">\n" <<
      "<graph defaultedgetype=\"undirected\" idtype=\"string\" type=\"static\">\n" <<
      "<attributes class=\"node\" mode=\"static\">\n" <<
      "<attribute id=\"latitude\" title=\"latitude\" type=\"double\"/>\n" <<
      "<attribute id=\"longitude\" title=\"longitude\" type=\"double\"/>\n" <<
      "</attributes>" << std::endl;

    w << "<nodes count=\"" << nodes_and_ways.first.size() << "\">" << std::endl;

    for (const auto &node : nodes_and_ways.first) {
        w << "<node id=\"" << node.id << "\">\n" <<
          "<attvalues>\n" <<
          "<attvalue for=\"latutide\" value=\"" << node.lat << "\"/>\n" <<
          "<attvalue for=\"longitude\" value=\"" << node.lon << "\"/>\n" <<
          "</attvalues>\n" <<
          "</node>" << std::endl;
    }

    w << "</nodes>" << std::endl;
    w << "<edges count=\"" << nodes_and_ways.second.size() << "\">" << std::endl;

    for (const auto &way : nodes_and_ways.second) {
        for (unsigned int i = 0; i < way.nodes.size() - 1; i++) {
            w << "<edge id=\"" << way.id << "\" source=\"" << way.nodes[i] <<
              "\" target=\"" << way.nodes[i + 1] << "\"/>" << std::endl;
        }
    }

    w << "</edges>\n</graph>\n</gexf>" << std::endl;

    return 0;
}