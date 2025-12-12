{
  "targets": [
    {
      "target_name": "mecab_binding",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ 
        "src/binding.cc",
        # OG
        "src/mecab-jp/libmecab.cpp",
        "src/mecab-jp/feature_index.cpp",
        "src/mecab-jp/param.cpp",
        "src/mecab-jp/learner.cpp",
        "src/mecab-jp/string_buffer.cpp",
        "src/mecab-jp/char_property.cpp",
        "src/mecab-jp/learner_tagger.cpp",
        "src/mecab-jp/tagger.cpp",
        "src/mecab-jp/connector.cpp",
        "src/mecab-jp/tokenizer.cpp",
        "src/mecab-jp/context_id.cpp",
        "src/mecab-jp/dictionary.cpp",
        "src/mecab-jp/utils.cpp",
        "src/mecab-jp/dictionary_compiler.cpp",
        "src/mecab-jp/viterbi.cpp",
        "src/mecab-jp/dictionary_generator.cpp",
        "src/mecab-jp/writer.cpp",
        "src/mecab-jp/iconv_utils.cpp",
        "src/mecab-jp/dictionary_rewriter.cpp",
        "src/mecab-jp/lbfgs.cpp",
        "src/mecab-jp/eval.cpp",
        "src/mecab-jp/nbest_generator.cpp",
        # Korean patch
        "src/mecab-ko/libmecab.cpp",
        "src/mecab-ko/feature_index.cpp",
        "src/mecab-ko/param.cpp",
        "src/mecab-ko/learner.cpp",
        "src/mecab-ko/string_buffer.cpp",
        "src/mecab-ko/char_property.cpp",
        "src/mecab-ko/learner_tagger.cpp",
        "src/mecab-ko/tagger.cpp",
        "src/mecab-ko/connector.cpp",
        "src/mecab-ko/tokenizer.cpp",
        "src/mecab-ko/context_id.cpp",
        "src/mecab-ko/dictionary.cpp",
        "src/mecab-ko/utils.cpp",
        "src/mecab-ko/dictionary_compiler.cpp",
        "src/mecab-ko/viterbi.cpp",
        "src/mecab-ko/dictionary_generator.cpp",
        "src/mecab-ko/writer.cpp",
        "src/mecab-ko/iconv_utils.cpp",
        "src/mecab-ko/dictionary_rewriter.cpp",
        "src/mecab-ko/lbfgs.cpp",
        "src/mecab-ko/eval.cpp",
        "src/mecab-ko/nbest_generator.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "src",
      ],
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS",
        "MECAB_USE_THREAD",
        "HAVE_WINDOWS_H",
        "HAVE_GETENV",
        "UNICODE",
        "_UNICODE",
        # These are required to make the code compile without the autotools config.h
        "DIC_VERSION=102",
        "VERSION=\"0.996\"",
        "PACKAGE=\"mecab\"",
        "MECAB_DEFAULT_RC=\"c:\\\\mecab\\\\mecabrc\"", # Ignored by the patch, but needed for syntax
        "_ALLOW_KEYWORD_MACROS",
        "register="
      ],
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1,
          "DisableSpecificWarnings": [ "4800", "4305", "4244", "4267" ]
        }
      }
    }
  ]
}
