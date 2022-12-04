#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Params/Params.h"

void getParamsAndConverters(std::vector<std::string> files, std::vector<std::string> config, std::vector<std::shared_ptr<Params>> &params, std::vector<std::string> &converters);

void printHelp();