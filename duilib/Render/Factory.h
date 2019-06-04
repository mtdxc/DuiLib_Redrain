#ifndef UI_CORE_RENDER_FACTORY_H_
#define UI_CORE_RENDER_FACTORY_H_
#pragma once

namespace ui 
{

class UILIB_API RenderFactory_GdiPlus : public IRenderFactory
{
public:
	virtual IPen* CreatePen(DWORD color, int width = 1) override;
	virtual IBrush* CreateBrush(DWORD corlor) override;
	virtual IBrush* CreateBrush(HBITMAP bitmap) override;
	virtual IMatrix* CreateMatrix() override;
	virtual IPath* CreatePath() override;
	virtual IBitmap* CreateBitmap() override;
	virtual IRenderContext* CreateRenderContext() override;
};

} // namespace ui

#endif // UI_CORE_RENDER_FACTORY_H_
