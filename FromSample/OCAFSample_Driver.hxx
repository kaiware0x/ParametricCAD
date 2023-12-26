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

#ifndef _OCAFSample_Driver_HeaderFile
#define _OCAFSample_Driver_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_LabelMap.hxx>
#include <TFunction_Driver.hxx>

class TFunction_Logbook;
class OCAFSample_Driver;

DEFINE_STANDARD_HANDLE(OCAFSample_Driver, TFunction_Driver)

class OCAFSample_Driver : public TFunction_Driver {
public:
    void                     Validate(Handle(TFunction_Logbook) & log) const override;
    virtual Standard_Boolean MustExecute(const Handle(TFunction_Logbook) & log) const override;
    virtual Standard_Boolean Arguments(TDF_LabelMap& theArgs) const;
    virtual Standard_Boolean Results(TDF_LabelMap& theRes) const;

    DEFINE_STANDARD_RTTIEXT(OCAFSample_Driver, TFunction_Driver)

protected:
    OCAFSample_Driver() {}
};

#endif
