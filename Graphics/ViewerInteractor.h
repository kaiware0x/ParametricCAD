//-----------------------------------------------------------------------------
// Created on: 15 February 2022
// Copyright (c) 2022, Quaoar Studio (natalia@quaoar.pro)
//----------------------------------------------------------------------------

#pragma once

#include <Aspect_VKeyFlags.hxx>
#include <Graphic3d_Vec2.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <Standard_Handle.hxx>

class ViewerWidget;

class AIS_InteractiveContext;
class AIS_RubberBand;

//! Handle key mouse events.
class ViewerInteractor {
    //! Viewer actions.
    enum class CurrentAction {
        Nothing,     //!< no action
        Zoom,        //!< zoom action
        Pan,         //!< pan action
        Rotation,    //!< rotation action
        Dragging     //!< drag action
    };

public:
    ViewerInteractor(ViewerWidget* viewer_widget);

    //! Fit all scene.
    void fitAll();

    //! Zoom scene.
    //! @param delta the zoom step.
    void zoom(const Standard_Real delta);

    //! Processing mouse press event to activate viewer action.
    //! @param point    clicked point in pixels
    //! @param keyMouse mouse button (left, middle, right)
    //! @param keyFlag  key modifier like SHIFT, CTRL and so on
    virtual void mousePressEvent(const Graphic3d_Vec2i& point, Aspect_VKeyMouse keyMouse, Aspect_VKeyFlags keyFlag);

    //! Processing mouse move event to process viewer action.
    //! @param point    clicked point in pixels
    //! @param keyMouse mouse button (left, middle, right)
    //! @param keyFlag  key modifier like SHIFT, CTRL and so on
    virtual void mouseMoveEvent(const Graphic3d_Vec2i& point, Aspect_VKeyMouse keyMouse, Aspect_VKeyFlags keyFlag);

    //! Processing mouse press event to finalize viewer action.
    //! @param point    clicked point in pixels
    //! @param keyMouse mouse button (left, middle, right)
    //! @param keyFlag  key modifier like SHIFT, CTRL and so on
    virtual void mouseReleaseEvent(const Graphic3d_Vec2i& point, Aspect_VKeyMouse keyMouse, Aspect_VKeyFlags keyFlag);

private:
    ViewerWidget* m_viewer_widget;
    Handle(AIS_RubberBand) m_rubberBand;             //!< rubber-band presentation for rectangle selection
    Handle(SelectMgr_EntityOwner) m_draggedOwner;    //!< dragged owner if current action is drag
    CurrentAction   m_currentAction;                 //!< current viewer action
    Graphic3d_Vec2i m_pressed_pos;
    Graphic3d_Vec2i m_moved_pos;
};
