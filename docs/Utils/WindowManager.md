### 窗口管理

UI组件提供了窗口管理类，UI组件的窗口管理接口位于`WindowsManager`类中。UI组件的各个窗口全部使用窗口管理类来打开。我们建议但并非强求您也使用窗口管理类。使用窗口管理类可以获得以下好处：

* 统一的管理和维护所有使用窗口管理类创建的窗口（包括UI组件的窗口和您自己开发的窗口）。
* 统一销毁所有使用窗口管理类创建的窗口。
* 随时获取使用窗口管理类创建的窗口指针。
* 唯一的打开某个窗口。

如果您打算让自己开发的窗口可以被窗口管理类维护，那么您的窗口类需要继承UI组件中的`nim_comp::WindowEx`类，并实现一些特定的接口，示例如下（具体实现请参考：[云信Duilib](./doc/nim_duilib.md)）：

```
class LoginForm : public nim_comp::WindowEx
{
public:
	LoginForm();
	~LoginForm();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	virtual void InitWindow() override;

	virtual UINT GetClassStyle() const override;
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;
	
	virtual std::wstring GetWindowId() const override;
}
```

* 使用窗口管理类唯一的打开某个窗口：

```
窗口类 *form = nim_ui::WindowsManager::SingletonShow<窗口类>(窗口类::kClassName);
```

* 根据窗口类名和id获取窗口指针：

```
nim_comp::WindowEx* form = nim_ui::WindowsManager::GetInstance()->GetWindow(窗口类名, 窗口ID);
```
* 获取所有窗口:

```
 nim_comp::WindowList form_list = nim_ui::WindowsManager::GetInstance()->GetAllWindows();
```
* 获取指定class对应的所有窗口

```
 nim_comp::WindowList form_list = nim_ui::WindowsManager::GetInstance()->GetWindowsByClassNam(窗口类名);
```
* 关闭所有窗口

```
 nim_ui::WindowsManager::GetInstance()->DestroyAllWindows();
```

如果您不打算使用UI组件提供的窗口管理类来管理自己的窗口，我们建议您自己实现另外的窗口管理类来管理自己的窗口。