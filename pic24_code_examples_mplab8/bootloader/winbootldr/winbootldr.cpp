// winbootldr.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"

using namespace winbootldr;

[STAThreadAttribute]

int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Look for command-line args
	Boolean blAndExit = false;
	if (args->Length > 0) {
		if (args[0] == "bl") {
			blAndExit = true;
		}
	}

	// Create the main window and run it
	Globals::form1 = gcnew Form1(blAndExit);
	Application::Run(Globals::form1);
	return 0;
}
