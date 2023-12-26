
#include "ViewerWidget.h"

ViewerWidget::ViewerWidget(Document* document, QWidget* parent)
    : QWidget(parent), m_document(document), m_startDrawing(false), m_interactor(new ViewerInteractor(this))
{
    initOccView();

    /// Qt documentation says "The contents of parent widgets are propagated by default
    /// to each of their children as long as Qt::WA_PaintOnScreen is not set.
    /// Custom widgets can be written to take advantage of this feature
    /// by updating irregular regions(to create non - rectangular child widgets),
    /// or painting with colors that have less than full alpha component.
    setAttribute(Qt::WA_PaintOnScreen);

    // Qt documentation says "Indicates that the widget has no background, i.e.
    // when the widget receives paint events, the background is not automatically repainted.
    setAttribute(Qt::WA_NoSystemBackground);

    setMouseTracking(true);
}

void ViewerWidget::myUpdate()
{
    update();
    m_view->MustBeResized();
    m_view->Redraw();
}

Handle(V3d_View) ViewerWidget::activeView(const Handle(AIS_InteractiveContext) & context)
{
    Handle(V3d_View) view;
    if (context.IsNull()) return view;

    const Handle(V3d_Viewer)& viewer = context->CurrentViewer();
    if (viewer.IsNull()) return view;

    if (!viewer->ActiveViews().IsEmpty()) {
        return viewer->ActiveViews().First();
    }
    else if (!viewer->DefinedViews().IsEmpty()) {
        return viewer->DefinedViews().First();
    }
    return view;
}

//-----------------------------------------------------------------------------

void ViewerWidget::paintEvent(QPaintEvent* /*theEvent*/)
{
    m_view->Redraw();
}

//-----------------------------------------------------------------------------

void ViewerWidget::resizeEvent(QResizeEvent* /*theEvent*/)
{
    m_view->MustBeResized();
}

//-----------------------------------------------------------------------------

void ViewerWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    m_startDrawing = false;
}

//-----------------------------------------------------------------------------

void ViewerWidget::mousePressEvent(QMouseEvent* event)
{
    const Graphic3d_Vec2i point(event->x(), event->y());
    const auto            key_mouse = keyMouse(event->button());
    const auto            key_flag  = keyFlag(event->modifiers());
    m_interactor->mousePressEvent(point, key_mouse, key_flag);
}

//-----------------------------------------------------------------------------

void ViewerWidget::mouseReleaseEvent(QMouseEvent* event)
{
    //    QPoint newPoint = event->pos();
    //    m_viewer->drawPoint(newPoint);
    //    if (!m_startP.isNull()) m_viewer->drawLine(m_startP, newPoint);
    //    m_startP = newPoint;

    const Graphic3d_Vec2i point(event->x(), event->y());
    const auto            key_mouse = keyMouse(event->button());
    const auto            key_flag  = keyFlag(event->modifiers());
    m_interactor->mouseReleaseEvent(point, key_mouse, key_flag);
}

//-----------------------------------------------------------------------------

void ViewerWidget::mouseMoveEvent(QMouseEvent* event)
{
    const Graphic3d_Vec2i point(event->x(), event->y());
    const auto            key_mouse = keyMouse(event->button());
    const auto            key_flag  = keyFlag(event->modifiers());
    m_interactor->mouseMoveEvent(point, key_mouse, key_flag);
}

//-----------------------------------------------------------------------------

Aspect_VKeyFlags ViewerWidget::keyFlag(const int modifierId)
{
    switch (modifierId) {
        case Qt::NoModifier:
            return Aspect_VKeyFlags_NONE;
        case Qt::ShiftModifier:
            return Aspect_VKeyFlags_SHIFT;
        case Qt::ControlModifier:
            return Aspect_VKeyFlags_CTRL;
        default:
            break;
    }
    return Aspect_VKeyFlags_NONE;
}

//-----------------------------------------------------------------------------

Aspect_VKeyMouse ViewerWidget::keyMouse(const int buttonId)
{
    switch (buttonId) {
        case Qt::NoButton:
            return Aspect_VKeyMouse_NONE;
        case Qt::LeftButton:
            return Aspect_VKeyMouse_LeftButton;
        case Qt::RightButton:
            return Aspect_VKeyMouse_RightButton;
        case Qt::MiddleButton:
            return Aspect_VKeyMouse_MiddleButton;
        default:
            break;
    }
    return Aspect_VKeyMouse_NONE;
}

void ViewerWidget::initOccView()
{
    static Handle(Aspect_DisplayConnection) displayConnection;
    //
    if (displayConnection.IsNull()) displayConnection = new Aspect_DisplayConnection();

    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection, false);

    m_viewer = new V3d_Viewer(graphicDriver);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    // Main view creation.
    m_view = m_viewer->CreateView();
    m_view->SetImmediateUpdate(false);
    m_view->SetBackgroundColor(Quantity_NOC_SKYBLUE1);
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.1);

    // Aspect window creation
    HWND winHandle = (HWND)winId();
    if (winHandle == NULL) return;
    Handle(Aspect_Window) wnd = new WNT_Window(winHandle);
    m_view->SetWindow(wnd, nullptr);
    //
    if (!wnd->IsMapped()) {
        wnd->Map();
    }
    m_view->MustBeResized();

    // View settings.
    m_view->SetShadingModel(V3d_PHONG);

    // Configure rendering parameters
    Graphic3d_RenderingParams& RenderParams = m_view->ChangeRenderingParams();
    RenderParams.IsAntialiasingEnabled      = true;
    RenderParams.NbMsaaSamples              = 8;    // Anti-aliasing by multi-sampling
    RenderParams.IsShadowEnabled            = false;
    RenderParams.CollectedStats             = Graphic3d_RenderingParams::PerfCounters_NONE;
}

void ViewerWidget::drawPoint(const QPoint& p)
{
    //    Standard_Real x, y, z;
    //    m_view->Convert(static_cast<int>(p.x()), static_cast<int>(p.y()), x, y, z);

    //    Handle(Geom_Point) gP    = new Geom_CartesianPoint(gp_Pnt(x, y, z));
    //    Handle(AIS_Point) pntPrs = new AIS_Point(gP);

    //    getContext()->Display(pntPrs, true);
}

void ViewerWidget::drawLine(const QPoint& p1, const QPoint& p2)
{
    //    Standard_Real x, y, z;
    //    m_view->Convert(static_cast<int>(p1.x()), static_cast<int>(p1.y()), x, y, z);
    //    Handle(Geom_Point) gP1 = new Geom_CartesianPoint(gp_Pnt(x, y, z));

    //    m_view->Convert(static_cast<int>(p2.x()), static_cast<int>(p2.y()), x, y, z);
    //    Handle(Geom_Point) gP2 = new Geom_CartesianPoint(gp_Pnt(x, y, z));
    //    //
    //    Handle(AIS_Line) linePrs = new AIS_Line(gP1, gP2);

    //    getContext()->Display(linePrs, true);
}
