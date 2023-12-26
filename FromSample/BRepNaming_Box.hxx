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

#ifndef _BRepNaming_Box_HeaderFile
#define _BRepNaming_Box_HeaderFile

#include "BRepNaming_TopNaming.hxx"
#include "BRepNaming_TypeOfPrimitive3D.hxx"

class BRepPrimAPI_MakeBox;

class BRepNaming_Box : public BRepNaming_TopNaming {
public:
    BRepNaming_Box() {}
    BRepNaming_Box(const TDF_Label& ResultLabel) : BRepNaming_TopNaming(ResultLabel) {}
    void Load(BRepPrimAPI_MakeBox& MakeShape, const BRepNaming_TypeOfPrimitive3D TypeOfResult) const;

private:
    TDF_Label Back() const;
    TDF_Label Bottom() const;
    TDF_Label Front() const;
    TDF_Label Left() const;
    TDF_Label Right() const;
    TDF_Label Top() const;
};

#endif
