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

#ifndef _OCAFSample_BoxDriver_HeaderFile
#define _OCAFSample_BoxDriver_HeaderFile

#include <Standard_DefineHandle.hxx>

#include "OCAFSample_Driver.hxx"

class TFunction_Logbook;
class OCAFSample_BoxDriver;

DEFINE_STANDARD_HANDLE(OCAFSample_BoxDriver, OCAFSample_Driver)

class OCAFSample_BoxDriver : public OCAFSample_Driver {
public:
    OCAFSample_BoxDriver() {}
    virtual Standard_Integer Execute(Handle(TFunction_Logbook) & theLogbook) const override;

    DEFINE_STANDARD_RTTIEXT(OCAFSample_BoxDriver, OCAFSample_Driver)
};

#endif
