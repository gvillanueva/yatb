# yatb

Yet another tesseract binding for Node

## Description

Node.js bindings for tesseract. Inspired by the efforts of [tesseract-js](https://www.npmjs.com/package/tesseract-js) and [tesseract](https://www.npmjs.com/package/tesseract).

## Features

* Compatible with Node v6.2.0
* Exposes essential methods of TessBaseAPI
* Strives to match syntax of C library
* Asynchronous API

## Requirements

* node-gyp
* Windows
* [Visual C++ Build Tools](http://go.microsoft.com/fwlink/?LinkId=691126)
* [Python 2.7](https://www.python.org/downloads/)

## Installation

    npm install yatb

## Usage
```javascript
var tesseract = require("yatb");
var api = new tesseract.TessBaseAPI();
if (api.init("eng")) {
    console.log("Could not initialize tesseract.");
    process.exit(1);
}

// Open input image with leptonica library
var image = leptonica.pixRead("/usr/src/tesseract/testing/phototest.tif");
api.SetImage(image);
// Get OCR result
outText = api.GetUTF8Text();
console.log("OCR output:\n%s", outText);

// Destroy used object and release memory
api.End();
leptonica.pixDestroy(image);
```
