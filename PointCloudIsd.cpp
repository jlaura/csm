//#############################################################################
//
//    FILENAME:          PointCloudIsd.cpp
//
//    CLASSIFICATION:    Unclassified
//
//    DESCRIPTION:
//
//    This file contains the implementations for the virtual destructors for
//    all the ISD classes defined in CSM.
//
//    LIMITATIONS:       None
//
//    SOFTWARE HISTORY:
//     Date          Author   Comment
//     -----------   ------   -------
//     20-Mar-2013   SCM      Initial Coding
//
//    NOTES:
//
//#############################################################################

#define CSM_POINT_CLOUD_LIBRARY
#include "PointCloudIsd.h"

csm::Vlr::~Vlr() {}
csm::PointCloudIsd::~PointCloudIsd() {}
csm::LasIsd::~LasIsd() {}
csm::BpfIsd::~BpfIsd() {}
