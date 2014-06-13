#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <utility>

#include "ptBinning.h"

class logPtPrimeBinning : public ptBinning
{
    public:

      logPtPrimeBinning();
      ~logPtPrimeBinning();
      virtual std::string getName(int bin);
      void fillLogPtPrimeBins();

    private:

};


