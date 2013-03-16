#include "map_parser.h"

void parse_meta(Json::Value root)
{
    const char * prefix="parse_meta(): ";

}

void parse_layers(Json::Value root)
{
    const char * prefix="parse_layers(): ";
    //report function call
    std::cout << prefix << "Called\n";

    const Json::Value layers = root["layers"];

    for ( int index = 0; index < layers.size(); ++index )  // Iterates over the sequence elements.
    {
        std::string type=layers[index]["type"].asString();

        //report layer processing
        std::cout << prefix << "Processing layer: id " << index << " : name `" << layers[index]["name"].asString() << "` : type `" << type << "`\n";

        //if the layer is not of `objectgroup` type then skip, aka. a tile layer
        if (type!="objectgroup")
        {
            //report invalid layer
            std::cout << prefix << "Warning: invalid layer, `" << type
            << "` type detected, only `objectgroup` is supported\n";
            continue;
        }

        parse_object_layer(index, layers[index]);
    }
}

void parse_object_layer( int depth, Json::Value layer)
{
    const char * prefix="parse_object_layer(): ";
    //report function call
    std::cout << prefix << "Called\n";


    const Json::Value objects = layer["objects"];

    for ( int index = 0; index < objects.size(); ++index )  // Iterates over the sequence elements.
    {
        parse_object(depth, objects[index]);
    }
}

void parse_object(int depth, Json::Value object)
{
    const char * prefix="parse_object(): ";
    //report function call
    std::cout << prefix << "Called\n";

    std::string name=object["name"].asString();
    std::string type=object["type"].asString();
    std::cout << prefix << "Processing object: name `" << name << "` : type `" << type << "`\n";

    if (type=="poly")
    {

    }
}

bool load_map(std::string filename)
{
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;

    std::ifstream file( filename.c_str(), std::ifstream::binary);
    bool parsingSuccessful = reader.parse( file, root );

    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "Failed to parse configuration\n" << reader.getFormattedErrorMessages();
        return 0;
    }

    parse_meta(root);
    parse_layers(root);

    return 1;
}
