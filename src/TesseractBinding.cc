/*
 * \file    TesseractBinding.cc
 * \brief   Implements bindings for TessBaseAPI
 * \author  Giancarlo Villanueva
 *
 * Copyright (c) 2016 Giancarlo Villanueva
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <stdio.h>
#include "TesseractBinding.h"

using namespace v8;

Persistent<Function> TesseractBinding::constructor;

void TesseractBinding::Initialize(Local<Object> exports)
{
    Isolate *isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "TesseractBinding"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Set Prototype methods
    NODE_SET_PROTOTYPE(tpl, "close", Close);
    NODE_SET_PROTOTYPE(tpl, "end", End);
    NODE_SET_PROTOTYPE(tpl, "init", Init);
    NODE_SET_PROTOTYPE(tpl, "setImage", SetImage);
    NODE_SET_PROTOTYPE(tpl, "processImage", ProcessImage);
    NODE_SET_PROTOTYPE(tpl, "getText", GetText);

    // Expose constructor method
    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "TesseractBinding"), tpl->GetFunction());
}

void TesseractBinding::New(const FunctionCallbackInfo<Value> args)
{
    Isolate *isolate = args.GetIsolate();

    if (args.IsConstructCall())
    {
        // Invoked as constructor: `new TesseractBinding()`
        TesseractBinding* obj = new TesseractBinding();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
    else
    {
        // Invoked as plain function `TesseractBinding(...)`, turn into construct call.
        Local<Context> context = isolate->GetCurrentContext();
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        Local<Object> result = cons->NewInstance(context).ToLocalChecked();
        args.GetReturnValue().Set(result);
    }
}

void TesseractBinding::Init(const FunctionCallbackInfo<Value> args)
{
    Isolate *isolate = args.GetIsolate();
    REQUIRE_STRING(0);

    char *lang;
    if (arg_lang.length())
        lang = strdup(*arg_lang);
    else
        lang = (char*)"eng";

    TesseractBinding* obj = ObjectWrap::Unwrap<TesseractBinding>(args.This());
    int ret = obj->t_api->Init(NULL, lang);
    return scope.Close(Integer::New(ret));
}

void TesseractBinding::SetImage(const FunctionCallbackInfo<Value> args)
{
    Isolate *isolate = args.GetIsolate();
    REQUIRE_ARG_NUM(1);

    PIX* pix;
    if (args[0]->IsString()) {
       pix = pixRead(strdup(*(String::Utf8Value(args[0]))));
    } else {
        return ThrowException(Exception::Error(
        String::New("Please enter a valid image location.")));
    }
    if (pix == NULL) {
        return ThrowException(Exception::Error(
        String::New("Error retrieve image from the given location.")));
    }

    TesseractBinding* obj = ObjectWrap::Unwrap<TesseractBinding>(args.This());
    obj->t_api->SetImage(pix);
    return scope.Close(Null());
}

void TesseractBinding::ProcessImage(const FunctionCallbackInfo<Value> args)
{
    Isolate *isolate = args.GetIsolate();

    TesseractBinding* obj = ObjectWrap::Unwrap<TesseractBinding>(args.This());
    obj->t_api->Recognize(NULL);
    return scope.Close(Null());
}

void TesseractBinding::GetText(const FunctionCallbackInfo<Value> args)
{
    Isolate *isolate = args.GetIsolate();

    TesseractBinding* obj = ObjectWrap::Unwrap<TesseractBinding>(args.This());
    char *text = obj->t_api->GetUTF8Text();
    return scope.Close(String::New(text));
}

void TesseractBinding::Close(const FunctionCallbackInfo<Value> args)
{
    Isolate *isolate = args.GetIsolate();

    TesseractBinding* obj = ObjectWrap::Unwrap<TesseractBinding>(args.This());
    obj->t_api->Clear();
    return scope.Close(Null());
}

void TesseractBinding::End(const FunctionCallbackInfo<Value> args)
{
    Isolate *isolate = args.GetIsolate();

    TesseractBinding* obj = ObjectWrap::Unwrap<TesseractBinding>(args.This());
    obj->t_api->End();
    return scope.Close(Null());
}

TesseractBinding::TesseractBinding()
{
    t_api = new tesseract::TessBaseAPI();
}

TesseractBinding::~TesseractBinding()
{
    t_api->Clear();
    t_api->End();
}
