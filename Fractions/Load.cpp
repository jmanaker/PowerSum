#pragma unmanaged

#include <Python.h>
#include <Windows.h>

bool WINAPI DllMain(HINSTANCE, unsigned long reason, void*)
{
	switch (reason)
	{
	case 1:
		Py_Initialize();
		break;
	case 0:
		Py_Finalize();
		break;
	}
	return true;
}