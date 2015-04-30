#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

using namespace AirDBImport;

[STAThread]
int main(array<String^>^ arg) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	MainForm form;
	Application::Run(%form);
}