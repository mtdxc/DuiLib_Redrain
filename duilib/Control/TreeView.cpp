#include "StdAfx.h"
#include "TreeView.h"


namespace ui
{

TreeNode::TreeNode() :
	m_bExpand(true),
	m_pTreeView(nullptr),
	m_pParentTreeNode(nullptr),
	m_iDepth(ROOT_NODE_DEPTH),
	mTreeNodes()
{
	
}

bool TreeNode::IsVisible() const
{
	return ListContainerElement::IsVisible()
		&& (!m_pParentTreeNode || (m_pParentTreeNode && m_pParentTreeNode->IsExpand() && m_pParentTreeNode->IsVisible()));
}

void TreeNode::SetInternVisible(bool bVisible)
{
	Control::SetInternVisible(bVisible);
	if (m_items.empty()) return;

	for (auto it = m_items.begin(); it != m_items.end(); it++)
	{
		auto pControl = *it;
		// 控制子控件显示状态
		// InternVisible状态应由子控件自己控制
		pControl->SetInternVisible(Control::IsVisible());
	}
}

void TreeNode::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	for (auto it = mTreeNodes.begin(); it != mTreeNodes.end(); it++)
	{
		(*it)->SetWindow(pManager, this, bInit);
	}

	ListContainerElement::SetWindow(pManager, pParent, bInit);
}

void TreeNode::SetTreeView(TreeView* pTreeView)
{
	m_pTreeView = pTreeView;
}

bool TreeNode::OnClickItem(EventArgs* pMsg)
{
	TreeNode* pItem = static_cast<TreeNode*>(pMsg->pSender);
	pItem->SetExpand(!pItem->IsExpand());

	return true;
}

TreeNode* TreeNode::GetParentNode()
{
	return m_pParentTreeNode;
}

void TreeNode::SetParentNode(TreeNode* pParentTreeNode)
{
	m_pParentTreeNode = pParentTreeNode;
}

bool TreeNode::AddChildNode(TreeNode* pTreeNode)
{
	return AddChildNodeAt(pTreeNode, GetChildNodeCount());
}

bool TreeNode::AddChildNodeAt(TreeNode* pTreeNode, std::size_t iIndex)
{
	if( iIndex < 0 || iIndex > mTreeNodes.size() ) return false;
	mTreeNodes.insert(mTreeNodes.begin() + iIndex, pTreeNode);
		
	pTreeNode->m_iDepth = m_iDepth + 1;
	pTreeNode->SetParentNode(this);
	pTreeNode->SetTreeView(m_pTreeView);
	if( m_pWindow != NULL ) m_pWindow->InitControls(pTreeNode, NULL);
	pTreeNode->OnEvent[kEventClick] += Bind(&TreeNode::OnClickItem, this, std::placeholders::_1);

	UiRect padding = m_pLayout->GetPadding();
	int nodeIndex = -1;
	if (m_iDepth != ROOT_NODE_DEPTH) {
		nodeIndex = GetIndex();
		padding.left += m_pTreeView->GetIndent();
	}
	pTreeNode->m_pLayout->SetPadding(padding);

	std::size_t nGlobalIndex = iIndex;
	for (std::size_t i = 0; i < iIndex; i++)
	{
		nGlobalIndex += ((TreeNode*)mTreeNodes[i])->GetDescendantNodeCount();
	}

	return m_pTreeView->ListBox::AddAt(pTreeNode, (int)(nodeIndex + nGlobalIndex + 1));
}

bool TreeNode::RemoveChildNodeAt(std::size_t iIndex)
{
	if (iIndex < 0 || iIndex >= mTreeNodes.size()) {
		return false;
	}

	TreeNode* pTreeNode = ((TreeNode*)mTreeNodes[iIndex]);
	mTreeNodes.erase(mTreeNodes.begin() + iIndex);

	return pTreeNode->RemoveSelf();
}

bool TreeNode::RemoveChildNode(TreeNode* pTreeNode)
{
	auto it = std::find(mTreeNodes.begin(), mTreeNodes.end(), pTreeNode);
	if (it == mTreeNodes.end()) {
		return false;
	}
		
	int iIndex = it - mTreeNodes.begin();
	return RemoveChildNodeAt(iIndex);
}
	
void TreeNode::RemoveAllChildNode()
{
	while (mTreeNodes.size() > 0)
	{
		RemoveChildNodeAt(0);
	}
}

bool TreeNode::RemoveSelf()
{
	for( auto it = mTreeNodes.begin(); it != mTreeNodes.end(); it++ ) 
	{
		(*it)->RemoveSelf();
	}
	mTreeNodes.clear();

	if (m_iDepth != ROOT_NODE_DEPTH) {
		return m_pTreeView->ListBox::RemoveAt(GetIndex());
	}

	return false;
}

int TreeNode::GetDescendantNodeCount()
{
	int nodeCount = (int)GetChildNodeCount();
	for( auto it = mTreeNodes.begin(); it != mTreeNodes.end(); it++ )
	{
		nodeCount += (*it)->GetDescendantNodeCount();
	}

	return nodeCount;
}

std::size_t TreeNode::GetChildNodeCount()
{
	return mTreeNodes.size();
}
	
TreeNode* TreeNode::GetChildNode(std::size_t iIndex)
{
	if( iIndex < 0 || iIndex >= mTreeNodes.size() ) return NULL;
	return static_cast<TreeNode*>(mTreeNodes[iIndex]);
}
	
int TreeNode::GetChildNodeIndex(TreeNode* pTreeNode)
{
	auto it = std::find(mTreeNodes.begin(), mTreeNodes.end(), pTreeNode);
	if (it == mTreeNodes.end()) {
		return -1;
	}
	return it - mTreeNodes.begin();
}

bool TreeNode::IsExpand() const
{
	return m_bExpand;
}

void TreeNode::SetExpand(bool bExpand)
{
	if(m_bExpand == bExpand) {
		return;
	}
	m_bExpand = bExpand;
	m_pTreeView->Arrange();
}

int TreeNode::GetDepth()
{
	return m_iDepth;
}

TreeView::TreeView() :
	ListBox(new VLayout),
	m_iIndent(0),
	m_rootNode()
{
	m_rootNode.reset(new TreeNode());
	m_rootNode->SetTreeView(this);
}

void TreeView::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if( strName == _T("indent") ) {
		SetIndent(_ttoi(strValue.c_str()));
	}
	else {
		ListBox::SetAttribute(strName, strValue);
	}
}

bool TreeView::Add(Control* pControl)
{
	ASSERT(FALSE);
	return true;
}

bool TreeView::AddAt(Control* pControl, std::size_t iIndex)
{
	ASSERT(FALSE);
	return true;
}

bool TreeView::Remove(Control* pControl)
{
	ASSERT(FALSE);
	return true;
}

bool TreeView::RemoveAt(std::size_t iIndex)
{
	ASSERT(FALSE);
	return true;
}

void TreeView::RemoveAll()
{
	ASSERT(FALSE);
}

void TreeView::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	ListBox::SetWindow(pManager, pParent, bInit);
	m_rootNode->SetWindow(pManager, pParent, bInit);
}

}