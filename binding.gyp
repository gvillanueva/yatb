{
  "targets": [
    {
      'target_name': 'yatb',
      'dependencies': [
        'src/tesseract/tesseract.gyp:libtesseract',
        'src/leptonica/leptonica.gyp:libleptonica',
      ],
      'include_dirs': [
        'src/leptonica/src',
        'src/tesseract/api',
        'src/tesseract/ccmain',
        'src/tesseract/ccstruct',
        'src/tesseract/ccutil',
        'src/tesseract/classify',
        'src/tesseract/cutil',
        'src/tesseract/dict',
        'src/tesseract/image',
        'src/tesseract/textord',
        'src/tesseract/viewer',
        'src/tesseract/wordrec',
      ],
      'sources': [
        'src/module.cc',
        'src/TesseractBinding.h',
        'src/TesseractBinding.cc'
      ],
      'cflags!': ['-fno-exceptions'],
      'cflags_cc!': ['-fno-exceptions'],
      'conditions': [
        ['OS=="mac"',
          {
            'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            },
            'configurations': {
              'Debug': {
                'msvs_settings': {
                  'VCCLCompilerTool': {
                    'ExceptionHandling': '1',
                  },
                },
              },
              'Release': {
                'msvs_settings': {
                  'VCCLCompilerTool': {
                    'ExceptionHandling': '1',
                  },
                },
              },
            },
          }
        ],
      ],
    },
  ]
}
