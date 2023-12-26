// Created: 2002-02-21
//
// Copyright (c) 2001-2013 OPEN CASCADE SAS
//
// This file is part of commercial software by OPEN CASCADE SAS.
//
// This software is furnished in accordance with the terms and conditions
// of the contract and with the inclusion of this copyright notice.
// This software or any other copy thereof may not be provided or otherwise
// be made available to any third party.
// No ownership title to the software is transferred hereby.
//
// OPEN CASCADE SAS makes no representation or warranties with respect to the
// performance of this software, and specifically disclaims any responsibility
// for any damages, special or consequential, connected with its use.

#ifndef _OCAFSample_AISDriver_HeaderFile
#define _OCAFSample_AISDriver_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TPrsStd_Driver.hxx>

#include <AIS_InteractiveObject.hxx>

class Standard_GUID;
class TDF_Label;
class AIS_InteractiveObject;
class OCAFSample_AISDriver;

DEFINE_STANDARD_HANDLE(OCAFSample_AISDriver, TPrsStd_Driver)

class OCAFSample_AISDriver : public TPrsStd_Driver {
public:
    static const Standard_GUID& GetID();
    OCAFSample_AISDriver() {}
    virtual Standard_Boolean Update(const TDF_Label& theLabel, Handle(AIS_InteractiveObject) & theAISObject) override;

    DEFINE_STANDARD_RTTIEXT(OCAFSample_AISDriver, TPrsStd_Driver)
};

#endif
