#pragma once
#include "document.h"
constexpr unsigned int INF = 100000000;

std::vector<std::vector<std::pair<int, int> > > parse_json(const char* filename, std::vector< std::vector<int> > weights){
    FILE* fp = fopen(filename, "r");
    char readBuffer[256];
    rapidjson::FileReadStream bis(fp, readBuffer, sizeof(readBuffer));
    rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(bis);  // wraps bis into eis
    rapidjson::Document d; // Document is GenericDocument<UTF8<> >
    d.ParseStream<0, rapidjson::AutoUTF<unsigned> >(eis); // This parses any UTF file into UTF-8 in memory

    assert(d.HasMember("graph"));
    assert(d["graph"].IsObject());
    assert(d["graph"].HasMember("nodes"));
    assert(d["graph"]["nodes"].IsArray());

    std::vector<int> vertices;
    for (rapidjson::Value::ConstValueIterator itr = d["graph"]["nodes"].Begin(); itr != d["graph"]["nodes"].End(); ++itr) {
        const rapidjson::Value& attribute = *itr;
        for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) {
//            std::cout << itr2->name.GetString() << " : " << itr2->value.GetString() << std::endl;
            vertices.push_back(atoi(itr2->value.GetString()));
        }
    }
    std::vector<int> row (vertices.size(), INF);
    for(int i = 0; i < vertices.size(); ++i){
        weights.push_back(row);
    }
    assert(d["graph"].HasMember("edges"));
    assert(d["graph"]["edges"].IsArray());
    std::vector<std::pair<int, int> > vertex_list;

    std::vector<std::vector<std::pair<int, int> > > ad_list (vertices.size(), vertex_list);

    for (rapidjson::Value::ConstValueIterator itr = d["graph"]["edges"].Begin(); itr != d["graph"]["edges"].End(); ++itr) {
        const rapidjson::Value& attribute = *itr;
//        std::cout << attribute["source"].GetString() << " : " << attribute["target"].GetString() << std::endl;
        ad_list[atoi(attribute["source"].GetString())-1].push_back(std::make_pair(
                atoi(attribute["target"].GetString())-1, atoi(attribute["weight"].GetString())));
        ad_list[atoi(attribute["target"].GetString())-1].push_back(std::make_pair(
                atoi(attribute["source"].GetString())-1, atoi(attribute["weight"].GetString())));
        weights[atoi(attribute["source"].GetString())-1][atoi(attribute["target"].GetString())-1]
                = atoi(attribute["weight"].GetString());
        weights[atoi(attribute["target"].GetString())-1][atoi(attribute["source"].GetString())-1]
                = atoi(attribute["weight"].GetString());
    }

    fclose(fp);
    return ad_list;
}
