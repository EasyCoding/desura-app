///////////// Copyright 2010 Desura Pty Ltd. All rights reserved.  /////////////
//
//   Project     : desura_libcef_dll_wrapper
//   File        : JavaScriptFactory.h
//   Description :
//      [TODO: Write the purpose of JavaScriptFactory.h.]
//
//   Created On: 5/28/2010 2:01:09 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#ifndef DESURA_JAVASCRIPTFACTORY_H
#define DESURA_JAVASCRIPTFACTORY_H
#ifdef _WIN32
#pragma once
#endif

#include "cef_desura_includes/ChromiumBrowserI.h"
#include "include/cef_app.h"

class ObjectWrapper : public CefRefPtr<CefV8Accessor>
{
public:
	ObjectWrapper(void* data)
	{
		m_pData = data;
	}

	void* getData()
	{
		return m_pData;
	}

private:
	void* m_pData;
};

class JavaScriptFactory : public ChromiumDLL::JavaScriptFactoryI
{
public:
	JavaScriptFactory();
	~JavaScriptFactory();

	virtual ChromiumDLL::JSObjHandle CreateUndefined();
	virtual ChromiumDLL::JSObjHandle CreateNull();
	virtual ChromiumDLL::JSObjHandle CreateBool(bool value);
	virtual ChromiumDLL::JSObjHandle CreateInt(int value);
	virtual ChromiumDLL::JSObjHandle CreateDouble(double value);
	virtual ChromiumDLL::JSObjHandle CreateString(const char* value);
	virtual ChromiumDLL::JSObjHandle CreateArray(int length);
	virtual ChromiumDLL::JSObjHandle CreateObject();
	virtual ChromiumDLL::JSObjHandle CreateObject(void* userData);
	virtual ChromiumDLL::JSObjHandle CreateException(const char* value);
	virtual ChromiumDLL::JSObjHandle CreateFunction(const char* name, ChromiumDLL::JavaScriptExtenderI* handler);
};

ChromiumDLL::JavaScriptFactoryI* GetJSFactory();

#endif //DESURA_JAVASCRIPTFACTORY_H