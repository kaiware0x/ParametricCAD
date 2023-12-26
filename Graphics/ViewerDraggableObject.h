//-----------------------------------------------------------------------------
// Created on: 14 June 2022
// Copyright (c) 2022, Quaoar Studio (natalia@quaoar.pro)
//----------------------------------------------------------------------------

#pragma once

#include <AIS_InteractiveObject.hxx>
#include <AIS_Shape.hxx>

#include <gp_Trsf.hxx>

//! Presentation that demonstrates drag processing.
class ViewerDraggableObject : public AIS_Shape {
public:
    ViewerDraggableObject(const TopoDS_Shape& shape);

    //! Processes drag. It changes local transformation of the presentation
    virtual Standard_Boolean ProcessDragging(const Handle(AIS_InteractiveContext) & context,
                                             const Handle(V3d_View) & view, const Handle(SelectMgr_EntityOwner) & owner,
                                             const Graphic3d_Vec2i& dragFrom, const Graphic3d_Vec2i& dragTo,
                                             const AIS_DragAction action) override;

private:
    Graphic3d_Vec2i m_startPosition;           //!< mouse position of the start drag
    gp_Trsf         m_startTrsf;               //!< transformation at the start drag
    Handle(Prs3d_Drawer) m_startAttributes;    //!< drawer when drag is started
};
