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

#ifndef _BRepNaming_TopNaming_HeaderFile
#define _BRepNaming_TopNaming_HeaderFile

#include <TDF_Label.hxx>

class BRepNaming_TopNaming {
public:
    const TDF_Label& ResultLabel() const { return myResultLabel; }
    void             Init(const TDF_Label& ResultLabel);

protected:
    BRepNaming_TopNaming() {}
    BRepNaming_TopNaming(const TDF_Label& ResultLabel);

    TDF_Label myResultLabel;
};

#endif
