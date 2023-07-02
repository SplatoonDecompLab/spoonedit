//
// Created by tv on 09.05.23.
//

#ifndef SPOONTOOL_ALL_H
#define SPOONTOOL_ALL_H
#include<SpecialObjectParams.h>


inline SpecialObjectParams* GetSpecParamsBySpoonID(std::string spoontype){
    return new SpecialObjectParams();
}

inline SpecialObjectParams* GetSpecParamsBySubID(std::string spoontype){
    return new SpecialObjectParams();
}

#endif //SPOONTOOL_ALL_H
