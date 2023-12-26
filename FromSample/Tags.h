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

// Define label tags

#ifndef TAGS_H
#define TAGS_H

namespace tag {
using Type = int;

static constexpr Type INVALID = -1;

enum Object {
    Argments = 1,
    Results,
};

enum Center { X = 1, Y, Z };

enum Box {
    DeltaX = 4,
    DeltaY,
    DeltaZ,
};

enum Cylinder { Radius = 4, Height };

enum Pipe {
    Result = 1,
    Ruled,
    Wires,
};

enum PipeShell {
    IsSolid = 1,
    IsFrenet,
    TransitionMode,
    Spine,
    Profile,
};

enum Sphere {
    Radius_ = 4,
};

enum BooleanOperation { Master = 1, Tool };

enum Prism { Height_ = 1, Reversed, Basis };

enum Selection { Context = 1 };

enum PipeShellTransition {
    Transform = 0,
    RightCorner,
    RoundCorner,
};

}    // namespace tag

// Error messages
#define DONE "Done"

#define NOTDONE "Can't be done"
#define NULL_ACCESS_NODE "Null access node"
#define NULL_BASIS_NODE "Null basis node"
#define NULL_ARGUMENT "Null argument"
#define NO_ROOT_FOUND "No root of the tree found"
#define ALGO_FAILED "Algorithm failed"
#define UNABLE_DUE_TO_CYCLE "Unable to set arguments, may be a cyclic dependence"
#define UNABLE_DUE_TO_NAMING "Unable to set arguments, may be a wrong naming"

#endif
