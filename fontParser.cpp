#include "stdafx.h"
#include "fontParser.h"
#include <fstream>

fontParser::fontParser()
{

}

fontParser::~fontParser()
{
    if (!fontInfoList.empty())
    {
        for (auto& fontInfo : fontInfoList)
        {
            SAFE_DEL(fontInfo.second);
        }
    }
}

void fontParser::parseFnt(int fontIdx, string fileName)
{
    vector<CD>* cD = new vector<CD>;
    cD->resize(128);
    ifstream fntFile;
    fntFile.open(fileName, ifstream::binary);
    if (fntFile)
    {
        char descriptionUnit[20];
        while (fntFile.read(descriptionUnit, sizeof(descriptionUnit)))
        {
            size_t currID = reinterpret_cast<DWORD*>(&descriptionUnit)[0];
            WORD* data = reinterpret_cast<WORD*>(&descriptionUnit[4]);
            (*cD)[currID].x = data[0];
            (*cD)[currID].y = data[1];
            (*cD)[currID].width = data[2];
            (*cD)[currID].height = data[3];
        }
        fntFile.close();
        fontInfoList.insert(make_pair(fontIdx, cD));
    }
    else SAFE_DEL(cD);
}
