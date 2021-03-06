﻿// Copyright © 2010-2015 The CefSharp Authors. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

#pragma once

#include "include/cef_browser.h"
#include "include/cef_runnable.h"
#include "include/cef_v8.h"

#include "TypeUtils.h"
#include "Stdafx.h"
#include "JavascriptRootObjectWrapper.h"
#include "Async/JavascriptAsyncMethodCallback.h"

using namespace CefSharp::Internals;
using namespace CefSharp::Internals::Async;
using namespace System;
using namespace System::ServiceModel;
using namespace System::Threading;
using namespace System::Threading::Tasks;

namespace CefSharp
{
    // "Master class" for wrapping everything that the Cef Subprocess needs 
    // for ONE CefBrowser.
    public ref class CefBrowserWrapper
    {
    private:
        MCefRefPtr<CefBrowser> _cefBrowser;
    
    internal:
        property JavascriptRootObjectWrapper^ JavascriptRootObjectWrapper;

    public:
        CefBrowserWrapper(CefRefPtr<CefBrowser> cefBrowser)
        {
            _cefBrowser = cefBrowser;
            BrowserId = cefBrowser->GetIdentifier();
            IsPopup = cefBrowser->IsPopup();
        }
        
        !CefBrowserWrapper()
        {
            _cefBrowser = nullptr;
        }

        ~CefBrowserWrapper()
        {
            this->!CefBrowserWrapper();

            if (JavascriptRootObjectWrapper != nullptr)
            {
                delete JavascriptRootObjectWrapper;

                JavascriptRootObjectWrapper = nullptr;
            }
        }

        property int BrowserId;
        property bool IsPopup;

        // This allows us to create the WCF proxies back to our parent process.
        property ChannelFactory<IBrowserProcess^>^ ChannelFactory;

        // The serialized registered object data waiting to be used (only contains methods and bound async).
        property JavascriptRootObject^ JavascriptAsyncRootObject;

        // The serialized registered object data waiting to be used.
        property JavascriptRootObject^ JavascriptRootObject;

        // The WCF proxy to the parent process.
        property IBrowserProcess^ BrowserProcess;
    };
}
