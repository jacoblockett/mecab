#include <napi.h>
#include "mecab-ko/mecab.h"
#include "mecab-jp/mecab.h"

class Mecab : public Napi::ObjectWrap<Mecab> {
private:
  std::string lang_;
  MeCabKo::Tagger* tagger_ko_;
  MeCabJp::Tagger* tagger_jp_;

public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Mecab", {
      InstanceMethod("parse", &Mecab::Parse)
    });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("Mecab", func);
    return exports;
  }

  Mecab(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Mecab>(info) {
    tagger_ko_ = nullptr;
    tagger_jp_ = nullptr;

    lang_ = info[0].As<Napi::String>().Utf8Value();
    std::string dicdir = info[1].As<Napi::String>().Utf8Value();
    std::string args = "--dicdir=" + dicdir;

    if (lang_ == "ko") {
      tagger_ko_ = MeCabKo::createTagger(args.c_str());
    } else {
      tagger_jp_ = MeCabJp::createTagger(args.c_str());
    }
  }

  ~Mecab() {
    if (tagger_ko_) delete tagger_ko_;
    if (tagger_jp_) delete tagger_jp_;
  }

  Napi::Value Parse(const Napi::CallbackInfo& info) {
    std::string input = info[0].As<Napi::String>().Utf8Value();
    const char* result;

    if (lang_ == "ko") {
      result = tagger_ko_->parse(input.c_str());
    } else {
      result = tagger_jp_->parse(input.c_str());
    }

    return Napi::String::New(info.Env(), result);
  }
};

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return Mecab::Init(env, exports);
}

NODE_API_MODULE(mecab, InitAll)
