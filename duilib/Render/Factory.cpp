#include "StdAfx.h"

namespace ui {

IPen* RenderFactory_GdiPlus::CreatePen(DWORD color, int width /*= 1*/)
{
	return new Pen_GdiPlus(color, width);
}

IBrush* RenderFactory_GdiPlus::CreateBrush(DWORD color)
{
	return new Brush_Gdiplus(color);
}

IBrush* RenderFactory_GdiPlus::CreateBrush(HBITMAP bitmap)
{
	return new Brush_Gdiplus(bitmap);
}

IMatrix* RenderFactory_GdiPlus::CreateMatrix()
{
	return new Matrix_Gdiplus();
}

IPath* RenderFactory_GdiPlus::CreatePath()
{
	return new Path_Gdiplus();
}

IBitmap* RenderFactory_GdiPlus::CreateBitmap()
{
	return new GdiBitmap();
}

IRenderContext* RenderFactory_GdiPlus::CreateRenderContext()
{
	return new RenderContext_GdiPlus();
}

} // namespace ui
