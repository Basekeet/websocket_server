#include "utills.h"


Json::Value parseJson(const string &json) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(json, root);
    return root;
}

string stringifyJson(const Json::Value &val) {
    Json::StreamWriterBuilder wbuilder;
    wbuilder["commentStyle"] = "None";
    wbuilder["indentation"] = "";

    return Json::writeString(wbuilder, val);
}