#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <strings.h>
#include <vector>
using namespace std;

typedef struct
{
    string path;
    string dependencies;
} ProjectFile;

typedef struct
{
    string name;
    bool expanded;
    vector<ProjectFile> files;
} ProjectGroup;

typedef struct
{
    map<string,string> properties;
    vector<string> localIncludes;
    vector<string> systemIncludes;
    vector<ProjectGroup> groups;
    vector<string> libraries;
} PaladinProject;

int ReadPaladinProject(const char *path, PaladinProject &proj)
{
    ifstream file;
    file.open(path, ifstream::in);
    if (!file.is_open())
    {
        cout << "Couldn't open the file " << path << endl;
        return -1;
    }

    proj.properties.clear();
    proj.localIncludes.clear();
    proj.systemIncludes.clear();
    proj.groups.clear();

    while (!file.eof())
    {
        string strData;
        getline(file, strData);
        if (strData.empty())
            continue;
        size_t pos = strData.find('=');
        if (pos == string::npos)
            continue;
        string key = strData.substr(0, pos);
        string value = strData.substr(pos + 1, string::npos);
        if (key.compare("GROUP") == 0)
        {
            ProjectGroup newGroup;
            newGroup.name = value;
            proj.groups.push_back(newGroup);
        }
        else if (key.compare("EXPANDGROUP") == 0)
        {
            if (!proj.groups.empty())
            proj.groups.back().expanded = strcasecmp(value.c_str(), "yes");
        }
        else if (key.compare("SOURCEFILE") == 0)
        {
            ProjectFile newFile;
            newFile.path = value;
            proj.groups.back().files.push_back(newFile);
        }
        else if (key.compare("DEPENDENCY") == 0)
            proj.groups.back().files.back().dependencies = value;
        else if (key.compare("LIBRARY") == 0)
            proj.libraries.push_back(value);
        else
            proj.properties[key] = value;
    }
    return 0;
}

int main(int argc, char **argv)
{
    PaladinProject project;
    if (argc == 2)
        ReadPaladinProject(argv[1], project);
    else
        cout << "Usage: " << argv[0] << " <path>\n";
    map<string,string>::iterator i;
    for (i = project.properties.begin(); i != project.properties.end(); i++)
        cout << i->first << ": " << i->second << endl;
        
    //attempt at making makefile:
    ofstream file;
    file.open(argv[1], ofstream:: out);
    file << "all: \n     g++" << endl;
    for(int x = 0; x<project.groups.size(); x++)
    {
        for(int y = 0; y<project.groups[x].size(); y++)
        {
            file << project.groups.at(x).at(y).path() << " ";
        }
    }
    
    file << "-o paladinReader" << endl;
    file << "clean: \n    rm -rf *o paladinReader";
    cout << file;         
    return 0;
}