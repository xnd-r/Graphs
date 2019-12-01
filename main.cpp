#include <iostream>
#include "document.h"
#include "filereadstream.h"

using namespace rapidjson;

using namespace std;
int main(){
    FILE* fp = fopen("/home/xndr/devel/repos/pyrgg/test_graph.json", "r");

    char readBuffer[256];
    FileReadStream bis(fp, readBuffer, sizeof(readBuffer));

    AutoUTFInputStream<unsigned, FileReadStream> eis(bis);  // wraps bis into eis

    Document d; // Document is GenericDocument<UTF8<> >
    d.ParseStream<0, AutoUTF<unsigned> >(eis); // This parses any UTF file into UTF-8 in memory
    assert(d.HasMember("graph"));
    assert(d["graph"].IsObject());
    assert(d["graph"].HasMember("nodes"));
    assert(d["graph"]["nodes"].IsArray());
    for (rapidjson::Value::ConstValueIterator itr = d["graph"]["nodes"].Begin(); itr != d["graph"]["nodes"].End(); ++itr) {
        const rapidjson::Value& attribute = *itr;
        assert(attribute.IsObject()); // each attribute is an object
        for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) {
            std::cout << itr2->name.GetString() << " : " << itr2->value.GetString() << std::endl;
        }
    }
    assert(d["graph"].HasMember("edges"));
    assert(d["graph"]["edges"].IsArray());
    for (rapidjson::Value::ConstValueIterator itr = d["graph"]["edges"].Begin(); itr != d["graph"]["edges"].End(); ++itr) {
        const rapidjson::Value& attribute = *itr;
        assert(attribute.HasMember("source")); // each attribute is an object
        assert(attribute["source"].IsString());
        std::cout << attribute["source"].GetString() << " : " << attribute["target"].GetString() << std::endl;

    }
    fclose(fp);
    std::cout << "Here I stand" << std::endl;
    return 0;
}