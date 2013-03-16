#ifndef MAP_PARSER_H_INCLUDED
#define MAP_PARSER_H_INCLUDED

#include <iostream>
#include <fstream>
#include "meta.h"
#include "json.h"

void parse_meta(Json::Value root);
void parse_layers(Json::Value root);
void parse_object_layer(int depth, Json::Value layer);
void parse_object(int depth, Json::Value object);
bool load_map(std::string filename);

#endif // MAP_PARSER_H_INCLUDED
