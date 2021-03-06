/*
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)
Copyright (C) 2014 Bad Juju Games, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.

Contact us at legal@badjuju.com.

*/

#include "Common.h"
#include "VSInstallMissing.h"

#include "IPCServiceMain.h"
#include "IPCInstallMcf.h"

#include "ItemHandle.h"
#include "ItemInfo.h"
#include "usercore/UserCoreI.h"
#include "webcore/WebCoreI.h"
#include "User.h"

namespace UserCore
{
namespace ItemTask
{

VSInstallMissing::VSInstallMissing()
{
	m_bFinished = false;
	m_pIPCIM = nullptr;
}

VSInstallMissing::~VSInstallMissing()
{
	if (!m_bFinished)
		m_WaitCond.wait();

	if (m_pIPCIM)
		m_pIPCIM->destroy();

	m_pIPCIM = nullptr;
}

bool VSInstallMissing::doTask()
{
	const char* val = getUserCore()->getCVarValue("gc_corecount");
	gcException errFailMCFServ(ERR_NULLHANDLE, "Failed to create uninstall MCF service!\n");

	m_pIPCIM = getUserCore()->getInternal()->getServiceMain()->newInstallMcf();
	if (!m_pIPCIM)
	{
		onErrorEvent(errFailMCFServ);
		return false;
	}

	uint8 workers = 1;

	if (val)
		workers = atoi(val);

	if (workers == 0)
		workers = 1;

	m_pIPCIM->onCompleteEvent += delegate(this, &VSInstallMissing::onComplete);
	m_pIPCIM->onProgressEvent += delegate(&onProgressEvent);
	m_pIPCIM->onErrorEvent += delegate(&onErrorEvent);
	m_pIPCIM->start(m_hMcf->getFile(), getItemInfo()->getPath(), getItemInfo()->getInstallScriptPath(), workers);

	m_WaitCond.wait();

	m_bFinished = true;

	return isStopped() == false;
}

void VSInstallMissing::onStop()
{
	m_WaitCond.notify();
}

void VSInstallMissing::onComplete()
{
	m_WaitCond.notify();
}





}
}
