/*!
@file epl.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Interface for EP Foundation Library
@version 2.0

@section LICENSE

Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section DESCRIPTION

An Interface for EP Foundation Library

*/
#ifndef __EP_EPL_H__
#define __EP_EPL_H__

// Algo
#include "epFastLog.h"
#include "epFastSqrt.h"
#include "epPrimeNum.h"
#include "epRandom.h"

#include "epBinarySearch.h"

#include "epInsertionSort.h"
#include "epMergeSort.h"
#include "epQuickSort.h"

#include "epAlgorithm.h"

//Container
#include "epFileStream.h"
#include "epNetworkStream.h"
#include "epStream.h"

#include "epThreadSafePQueue.h"
#include "epThreadSafeQueue.h"

#include "epCStringEx.h"
#include "epDelegate.h"
#include "epDynamicArray.h"

//Debugger
#include "epBaseOutputter.h"
#include "epProfiler.h"
#include "epSimpleLogger.h"

//File System
#include "epBaseFile.h"
#include "epFolderHelper.h"
#include "epPropertiesFile.h"
#include "epXMLFile.h"
#include "epXMLite.h"

//Frameworks
#include "epBaseLock.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epSemaphore.h"
#include "epNoLock.h"

#include "epSingletonHolder.h"
#include "epSmartObject.h"
#include "epThreadSafeClass.h"

//GUI
#include "epTaskbarNotifier.h"
#include "epWinProcessHelper.h"

//Server
#include "epBaseClient.h"
#include "epBaseClientEx.h"
#include "epBaseServer.h"
#include "epBaseServerEx.h"
#include "epBaseServerWorker.h"
#include "epBaseServerWorkerEx.h"
#include "epPacket.h"
#include "epPacketContainer.h"

//System
#include "epAssert.h"
#include "epLocale.h"
#include "epConsoleHelper.h"
#include "epDateTimeHelper.h"
#include "epEndian.h"
#include "epMemory.h"
#include "epPlatform.h"
#include "epRegistryHelper.h"
#include "epSystem.h"
#include "epTinyObject.h"
#include "epException.h"

//Thread System
#include "epBaseJob.h"
#include "epBaseJobProcessor.h"

#include "epJobScheduleQueue.h"

#include "epWorkerThreadInfinite.h"
#include "epWorkerThreadSingle.h"

#include "epBaseWorkerThread.h"
#include "epWorkerThreadDelegate.h"
#include "epWorkerThreadFactory.h"

#endif //__EP_EPL_H__