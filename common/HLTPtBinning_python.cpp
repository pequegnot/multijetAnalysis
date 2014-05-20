#include "HLTPtBinning.h"
#include <boost/python.hpp>
 
BOOST_PYTHON_MODULE(HLTPtBinning)
{
    using namespace boost::python;
    class_<ptBinning>("ptBinning")
        .def("getSize", &ptBinning::getSize)
        .def("getBinValueInf", &ptBinning::getBinValueInf)
        .def("getBinValueSup", &ptBinning::getBinValueSup)
    ;

    class_<HLTPtBinning, bases<ptBinning> >("HLTPtBinning")
        .def("fillHLTPtBins", &HLTPtBinning::fillHLTPtBins)
    ;
}
