/*!
@file epl.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Interface for EpLibrary Pre-compiled header
@version 2.0

@section LICENSE

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

@section DESCRIPTION

An Interface for EpLibrary Pre-compiled header

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

#include "epCoroutine.h"
#include "epCStringEx.h"
#include "epDelegate.h"
#include "epDynamicArray.h"

//Debugger
#include "epBaseOutputter.h"
#include "epProfiler.h"
#include "epSimpleLogger.h"

//File System
#include "epBinaryFile.h"
#include "epBaseTextFile.h"
#include "epFolderHelper.h"
#include "epPropertiesFile.h"
#include "epXMLFile.h"
#include "epXMLite.h"
#include "epTextFile.h"
#include "epLogWriter.h"

//IPC
#include "epIpcClient.h"
#include "epIpcClientInterfaces.h"
#include "epIpcConf.h"
#include "epIpcPipe.h"
#include "epIpcServer.h"
#include "epIpcServerInterfaces.h"

//Frameworks
#include "epBaseLock.h"
#include "epCriticalSectionEx.h"
#include "epEventEx.h"
#include "epMutex.h"
#include "epSemaphore.h"
#include "epNoLock.h"
#include "epInterlockedEx.h"
#include "epCmdLineOptions.h"

#include "epSingletonHolder.h"
#include "epSmartObject.h"
#include "epThreadSafeClass.h"

//GUI
#include "epTaskbarNotifier.h"
#include "epWinProcessHelper.h"
#include "epWinResizer.h"

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
#include "epThread.h"

#endif //__EP_EPL_H__