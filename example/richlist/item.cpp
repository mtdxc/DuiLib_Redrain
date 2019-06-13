#include "stdafx.h"
#include "item.h"
#include <time.h>

Item::Item()
{
}


Item::~Item()
{
}

void Item::InitSubControls(const std::wstring& img, const std::wstring& title)
{
	// 查找 Item 下的控件
	control_img_	= dynamic_cast<ui::Control*>(FindSubControl(L"control_img"));
	label_title_	= dynamic_cast<ui::Label*>(FindSubControl(L"label_title"));
	progress_		= dynamic_cast<ui::Progress*>(FindSubControl(L"progress"));
	btn_del_		= dynamic_cast<ui::Button*>(FindSubControl(L"btn_del"));

	// 模拟进度条进度
	int64_t timestamp_num = GetTickCount();
	progress_->SetValue((double)(timestamp_num % 100));

	// 设置图标和任务名称
	control_img_->SetBkImage(img);
	label_title_->SetText(ui::StringHelper::Printf(L"%s %d%%", title.c_str(), timestamp_num % 100));

	// 绑定删除任务处理函数
	btn_del_->AttachClick(ui::Bind(&Item::OnRemove, this, std::placeholders::_1));
}

bool Item::OnRemove(ui::EventArgs* args)
{
	ui::ListBox* parent = dynamic_cast<ui::ListBox*>(this->GetParent());
	return parent->Remove(this);
}
