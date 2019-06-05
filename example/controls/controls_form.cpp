#include "stdafx.h"
#include "controls_form.h"

#include <fstream>

const std::wstring ControlForm::kClassName = L"Controls";

ControlForm::ControlForm()
{
}


ControlForm::~ControlForm()
{
}

std::wstring ControlForm::GetSkinFolder()
{
	return L"controls";
}

std::wstring ControlForm::GetSkinFile()
{
	return L"controls.xml";
}

std::wstring ControlForm::GetWindowClassName() const
{
	return kClassName;
}

void ControlForm::InitWindow()
{
	/**
	 * 为了让代码看起来相对容易理解，不需要频繁跟进才能看明白示例代码
	 * 我们将一些控件储存为局部变量，正确的使用应该是将他们作为成员变量
	 * 不要在每次使用的时候都需要 FindControl，否则会影响性能代码不易读
	 */

	/* Initialize ListBox data */
	ui::ListBox* list = dynamic_cast<ui::ListBox*>(FindControl(L"list"));
	for (auto i = 0; i < 30; i++)
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetText(ui::StringHelper::Printf(L"ListElement %d", i));
		element->SetClass(L"listitem");
		element->SetFixedHeight(20);
		list->Add(element);
	}

	/* Initialize TreeView data */
	ui::TreeView* tree = dynamic_cast<ui::TreeView*>(FindControl(L"tree"));
	ui::TreeNode* parent_node = nullptr;
	for (auto j = 0; j < 8; j++)
	{
		ui::TreeNode* node = new ui::TreeNode;
		node->SetClass(L"listitem");
		node->SetFixedHeight(20);
		if (parent_node)
		{
			node->SetText(ui::StringHelper::Printf(L"Child node %d", j));
			node->SetMargin({ 10, 0, 0, 0 });
			parent_node->AddChildNode(node);
		}
		else
		{
			node->SetText(ui::StringHelper::Printf(L"Parent node", j));
			tree->GetRootNode()->AddChildNode(node);
			parent_node = node;
		}
	}

	/* Load xml file content in global misc thread, and post update RichEdit task to UI thread */
	StdClosure closure = [this]() {
		std::streamoff length = 0;
		std::wstring xml;
		std::wstring controls_xml = ui::GlobalManager::GetResourcePath() + GetWindowResourcePath() + GetSkinFile();

		std::wifstream ifs(controls_xml.c_str());
		if (ifs.is_open())
		{
			ifs.seekg(0, std::ios_base::end);
			length = ifs.tellg();
			ifs.seekg(0, std::ios_base::beg);

			xml.resize(static_cast<unsigned int>(length) + 1);
			ifs.read(&xml[0], length);

			ifs.close();
		}

		OnLoadedResourceFile(xml);
	};
	// Using ToWeakCallback to protect closure when if [ControlForm] was destoryed
	// nbase::ThreadManager::PostTask(kThreadGlobalMisc, ToWeakCallback(closure)); // or Post2GlobalMisc(ToWeakCallback(closure));

	/* Post repeat task to update progress value 200 milliseconds once 
	StdClosure repeat_task = [this]() {
		int64_t timestamp_num = time(NULL);
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&ControlForm::OnProgressValueChagned, this, timestamp_num % 100));
	};
	nbase::ThreadManager::PostRepeatedTask(kThreadGlobalMisc, ToWeakCallback(repeat_task), nbase::TimeDelta::FromMilliseconds(200));
	*/
	/* Show settings menu */
	ui::Button* settings = dynamic_cast<ui::Button*>(FindControl(L"settings"));
	settings->AttachClick([this](ui::EventArgs* args) {
		RECT rect = args->pSender->GetPos();
		ui::CPoint point;
		point.x = rect.left - 175;
		point.y = rect.top + 10;
		ClientToScreen(m_hWnd, &point);

		ui::CMenuWnd* pMenu = new ui::CMenuWnd(NULL);
		ui::STRINGorID xml(L"settings_menu.xml");
		pMenu->Init(xml, _T("xml"), point);
		return true;
	});
}

LRESULT ControlForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void ControlForm::OnLoadedResourceFile(const std::wstring& xml)
{
	if (xml.empty())
		return;

	auto control_edit = dynamic_cast<ui::RichEdit*>(FindControl(L"edit"));
	control_edit->SetText(xml);
	control_edit->SetFocus();
	control_edit->HomeUp();
}

void ControlForm::OnProgressValueChagned(float value)
{
	auto progress = dynamic_cast<ui::Progress*>(FindControl(L"progress"));
	progress->SetValue(value);
}
