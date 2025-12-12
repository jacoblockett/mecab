//
//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#endif

#include "mecab.h"
#include "tokenizer.h"
#include "utils.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace {
const char kUnknownError[] = "Unknown Error";
const size_t kErrorBufferSize = 256;
}

#if defined(_WIN32) && !defined(__CYGWIN__)
namespace {
DWORD g_tls_index = TLS_OUT_OF_INDEXES;
}

const char *getGlobalErrorJp() {
  LPVOID data = ::TlsGetValue(g_tls_index);
  return data == NULL ? kUnknownError : reinterpret_cast<const char *>(data);
}

void setGlobalErrorJp(const char *str) {
  char *data = reinterpret_cast<char *>(::TlsGetValue(g_tls_index));
  if (data == NULL) {
    return;
  }
  strncpy(data, str, kErrorBufferSize - 1);
  data[kErrorBufferSize - 1] = '\0';
}

HINSTANCE DllInstanceJp = 0;

extern "C" {
  BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID) {
    LPVOID data = 0;
    if (!DllInstanceJp) {
      DllInstanceJp = hinst;
    }
    switch (dwReason) {
      case DLL_PROCESS_ATTACH:
        if ((g_tls_index = ::TlsAlloc()) == TLS_OUT_OF_INDEXES) {
          return FALSE;
        }
        // Not break in order to initialize the TLS.
      case DLL_THREAD_ATTACH:
        data = (LPVOID)::LocalAlloc(LPTR, kErrorBufferSize);
        if (data) {
          ::TlsSetValue(g_tls_index, data);
        }
        break;
      case DLL_THREAD_DETACH:
        data = ::TlsGetValue(g_tls_index);
        if (data) {
          ::LocalFree((HLOCAL)data);
        }
        break;
      case DLL_PROCESS_DETACH:
        data = ::TlsGetValue(g_tls_index);
        if (data) {
          ::LocalFree((HLOCAL)data);
        }
        ::TlsFree(g_tls_index);
        g_tls_index = TLS_OUT_OF_INDEXES;
        break;
      default:
        break;
    }
    return TRUE;
  }
}
#else  // _WIN32
namespace {
#ifdef HAVE_TLS_KEYWORD
__thread char kErrorBuffer[kErrorBufferSize];
#else
char kErrorBuffer[kErrorBufferSize];
#endif
}

const char *getGlobalErrorJp() {
  return kErrorBuffer;
}

void setGlobalErrorJp(const char *str) {
  strncpy(kErrorBuffer, str, kErrorBufferSize - 1);
  kErrorBuffer[kErrorBufferSize - 1] = '\0';
}
#endif

mecab_jp_t* mecab_jp_new(int argc, char **argv) {
  MeCabJp::Tagger *tagger = MeCabJp::createTagger(argc, argv);
  if (!tagger) {
    MeCabJp::deleteTagger(tagger);
    return 0;
  }
  return reinterpret_cast<mecab_jp_t *>(tagger);
}

mecab_jp_t* mecab_jp_new2(const char *arg) {
  MeCabJp::Tagger *tagger = MeCabJp::createTagger(arg);
  if (!tagger) {
    MeCabJp::deleteTagger(tagger);
    return 0;
  }
  return reinterpret_cast<mecab_jp_t *>(tagger);
}

const char *mecab_jp_version() {
  return MeCabJp::Tagger::version();
}

const char* mecab_jp_strerror(mecab_jp_t *tagger) {
  if (!tagger) {
    return MeCabJp::getLastError();
  }
  return reinterpret_cast<MeCabJp::Tagger *>(tagger)->what();
}

void mecab_jp_destroy(mecab_jp_t *tagger) {
  MeCabJp::Tagger *ptr = reinterpret_cast<MeCabJp::Tagger *>(tagger);
  MeCabJp::deleteTagger(ptr);
  ptr = 0;
}

int  mecab_jp_get_partial(mecab_jp_t *tagger) {
  return reinterpret_cast<MeCabJp::Tagger *>(tagger)->partial();
}

void mecab_jp_set_partial(mecab_jp_t *tagger, int partial) {
  reinterpret_cast<MeCabJp::Tagger *>(tagger)->set_partial(partial);
}

float  mecab_jp_get_theta(mecab_jp_t *tagger) {
  return reinterpret_cast<MeCabJp::Tagger *>(tagger)->theta();
}

void mecab_jp_set_theta(mecab_jp_t *tagger, float theta) {
  reinterpret_cast<MeCabJp::Tagger *>(tagger)->set_theta(theta);
}

int  mecab_jp_get_lattice_level(mecab_jp_t *tagger) {
  return reinterpret_cast<MeCabJp::Tagger *>(tagger)->lattice_level();
}

void mecab_jp_set_lattice_level(mecab_jp_t *tagger, int level) {
  reinterpret_cast<MeCabJp::Tagger *>(tagger)->set_lattice_level(level);
}

int mecab_jp_get_all_morphs(mecab_jp_t *tagger) {
  return static_cast<int>(
      reinterpret_cast<MeCabJp::Tagger *>(tagger)->all_morphs());
}

void mecab_jp_set_all_morphs(mecab_jp_t *tagger, int all_morphs) {
  reinterpret_cast<MeCabJp::Tagger *>(tagger)->set_all_morphs(all_morphs);
}

const char* mecab_jp_sparse_tostr(mecab_jp_t *tagger, const char *str) {
  return reinterpret_cast<MeCabJp::Tagger *>(tagger)->parse(str);
}

const char* mecab_jp_sparse_tostr2(mecab_jp_t *tagger, const char *str, size_t len) {
  return reinterpret_cast<MeCabJp::Tagger *>(tagger)->parse(str, len);
}

char* mecab_jp_sparse_tostr3(mecab_jp_t *tagger, const char *str, size_t len,
                          char *out, size_t len2) {
  return const_cast<char *>(
      reinterpret_cast<MeCabJp::Tagger *>(tagger)->parse(
          str, len, out, len2));
}

const mecab_jp_node_t* mecab_jp_sparse_tonode(mecab_jp_t *tagger, const char *str) {
  return reinterpret_cast<const mecab_jp_node_t *>(
      reinterpret_cast<MeCabJp::Tagger *>(tagger)->parseToNode(str));
}

const mecab_jp_node_t* mecab_jp_sparse_tonode2(mecab_jp_t *tagger,
                                         const char *str, size_t len) {
  return reinterpret_cast<const mecab_jp_node_t *>(
      reinterpret_cast<MeCabJp::Tagger *>(tagger)->parseToNode(str, len));
}

const char* mecab_jp_nbest_sparse_tostr(mecab_jp_t *tagger, size_t N,
                                     const char *str) {
  return reinterpret_cast<MeCabJp::Tagger *>(tagger)->parseNBest(N, str);
}

const char* mecab_jp_nbest_sparse_tostr2(mecab_jp_t *tagger, size_t N,
                                      const char* str, size_t len) {
  return reinterpret_cast<MeCabJp::Tagger *>(
      tagger)->parseNBest(N, str, len);
}

char* mecab_jp_nbest_sparse_tostr3(mecab_jp_t *tagger, size_t N,
                                const char *str, size_t len,
                                char *out, size_t len2) {
  return const_cast<char *>(
      reinterpret_cast<MeCabJp::Tagger *>(
          tagger)->parseNBest(N, str, len, out, len2));
}

int mecab_jp_nbest_init(mecab_jp_t *tagger, const char *str) {
  return reinterpret_cast<
      MeCabJp::Tagger *>(tagger)->parseNBestInit(str);
}

int mecab_jp_nbest_init2(mecab_jp_t *tagger, const char *str, size_t len) {
  return reinterpret_cast<
      MeCabJp::Tagger *>(tagger)->parseNBestInit(str, len);
}

const char* mecab_jp_nbest_next_tostr(mecab_jp_t *tagger) {
  return reinterpret_cast<MeCabJp::Tagger *>(tagger)->next();
}

char* mecab_jp_nbest_next_tostr2(mecab_jp_t *tagger, char *out, size_t len2) {
  return const_cast<char *>(
      reinterpret_cast<MeCabJp::Tagger *>(tagger)->next(out, len2));
}

const mecab_jp_node_t* mecab_jp_nbest_next_tonode(mecab_jp_t *tagger) {
  return reinterpret_cast<const mecab_jp_node_t *>(
      reinterpret_cast<MeCabJp::Tagger *>(tagger)->nextNode());
}

const char* mecab_jp_format_node(mecab_jp_t *tagger, const mecab_jp_node_t* n) {
  return reinterpret_cast<MeCabJp::Tagger *>(tagger)->formatNode(n);
}

const mecab_jp_dictionary_info_t *mecab_jp_dictionary_info(mecab_jp_t *tagger) {
  return reinterpret_cast<const mecab_jp_dictionary_info_t *>(
      reinterpret_cast<MeCabJp::Tagger *>(tagger)->dictionary_info());
}

int mecab_jp_parse_lattice(mecab_jp_t *mecab, mecab_jp_lattice_t *lattice) {
  return static_cast<int>(
      reinterpret_cast<MeCabJp::Tagger *>(mecab)->parse(
          reinterpret_cast<MeCabJp::Lattice *>(lattice)));
}

mecab_jp_lattice_t *mecab_jp_lattice_new() {
  return reinterpret_cast<mecab_jp_lattice_t *>(MeCabJp::createLattice());
}

void mecab_jp_lattice_destroy(mecab_jp_lattice_t *lattice) {
  MeCabJp::Lattice *ptr = reinterpret_cast<MeCabJp::Lattice *>(lattice);
  MeCabJp::deleteLattice(ptr);
  ptr = 0;
}

void mecab_jp_lattice_clear(mecab_jp_lattice_t *lattice) {
  reinterpret_cast<MeCabJp::Lattice *>(lattice)->clear();
}

int mecab_jp_lattice_is_available(mecab_jp_lattice_t *lattice) {
  return static_cast<int>(
      reinterpret_cast<MeCabJp::Lattice *>(lattice)->is_available());
}
mecab_jp_node_t *mecab_jp_lattice_get_bos_node(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<mecab_jp_node_t *>(
      reinterpret_cast<MeCabJp::Lattice *>(lattice)->bos_node());
}

mecab_jp_node_t *mecab_jp_lattice_get_eos_node(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<mecab_jp_node_t *>(
      reinterpret_cast<MeCabJp::Lattice *>(lattice)->eos_node());
}

mecab_jp_node_t **mecab_jp_lattice_get_all_begin_nodes(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<mecab_jp_node_t **>(
      reinterpret_cast<MeCabJp::Lattice *>(lattice)->begin_nodes());
}

mecab_jp_node_t **mecab_jp_lattice_get_all_end_nodes(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<mecab_jp_node_t **>(
      reinterpret_cast<MeCabJp::Lattice *>(lattice)->end_nodes());
}

mecab_jp_node_t *mecab_jp_lattice_get_begin_nodes(mecab_jp_lattice_t *lattice,
                                            size_t pos) {
  return reinterpret_cast<mecab_jp_node_t *>(
      reinterpret_cast<MeCabJp::Lattice *>(lattice)->begin_nodes(pos));
}

mecab_jp_node_t    *mecab_jp_lattice_get_end_nodes(mecab_jp_lattice_t *lattice,
                                             size_t pos) {
  return reinterpret_cast<mecab_jp_node_t *>(
      reinterpret_cast<MeCabJp::Lattice *>(lattice)->end_nodes(pos));
}

const char  *mecab_jp_lattice_get_sentence(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<MeCabJp::Lattice *>(lattice)->sentence();
}

void  mecab_jp_lattice_set_sentence(mecab_jp_lattice_t *lattice,
                                 const char *sentence) {
  reinterpret_cast<MeCabJp::Lattice *>(lattice)->set_sentence(sentence);
}

void mecab_jp_lattice_set_sentence2(mecab_jp_lattice_t *lattice,
                                 const char *sentence, size_t len) {
  reinterpret_cast<MeCabJp::Lattice *>(lattice)->set_sentence(
      sentence, len);
}

size_t mecab_jp_lattice_get_size(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<MeCabJp::Lattice *>(lattice)->size();
}

double mecab_jp_lattice_get_z(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<MeCabJp::Lattice *>(lattice)->Z();
}

void mecab_jp_lattice_set_z(mecab_jp_lattice_t *lattice, double Z) {
  reinterpret_cast<MeCabJp::Lattice *>(lattice)->set_Z(Z);
}

double mecab_jp_lattice_get_theta(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<MeCabJp::Lattice *>(lattice)->theta();
}

void mecab_jp_lattice_set_theta(mecab_jp_lattice_t *lattice, double theta) {
  reinterpret_cast<MeCabJp::Lattice *>(lattice)->set_theta(theta);
}

int mecab_jp_lattice_next(mecab_jp_lattice_t *lattice) {
  return static_cast<int>(
      reinterpret_cast<MeCabJp::Lattice *>(lattice)->next());
}

int mecab_jp_lattice_get_request_type(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<MeCabJp::Lattice *>(lattice)->request_type();
}

int MECAB_JP_lattice_has_request_type(mecab_jp_lattice_t *lattice,
                                   int request_type) {
  return reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->has_request_type(request_type);
}

void mecab_jp_lattice_set_request_type(mecab_jp_lattice_t *lattice,
                                    int request_type) {
  reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->set_request_type(request_type);
}

void mecab_jp_lattice_add_request_type(mecab_jp_lattice_t *lattice,
                                    int request_type) {
  reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->add_request_type(request_type);
}

void mecab_jp_lattice_remove_request_type(mecab_jp_lattice_t *lattice,
                                       int request_type) {
  return reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->remove_request_type(request_type);
}

mecab_jp_node_t    *mecab_jp_lattice_new_node(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<mecab_jp_node_t *>(
      reinterpret_cast<MeCabJp::Lattice *>(lattice)->newNode());
}

const char *mecab_jp_lattice_tostr(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<MeCabJp::Lattice *>(lattice)->toString();
}

const char *mecab_jp_lattice_tostr2(mecab_jp_lattice_t *lattice,
                                 char *buf, size_t size) {
  return reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->toString(buf, size);
}
const char *mecab_jp_lattice_nbest_tostr(mecab_jp_lattice_t *lattice,
                                      size_t N) {
  return reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->enumNBestAsString(N);
}
const char *mecab_jp_lattice_nbest_tostr2(mecab_jp_lattice_t *lattice,
                                       size_t N, char *buf, size_t size) {
  return reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->enumNBestAsString(N, buf, size);
}

int MECAB_JP_lattice_has_constraint(mecab_jp_lattice_t *lattice) {
  return static_cast<bool>(reinterpret_cast<MeCabJp::Lattice *>(
                               lattice)->has_constraint());
}

int mecab_jp_lattice_get_boundary_constraint(mecab_jp_lattice_t *lattice,
                                          size_t pos) {
  return reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->boundary_constraint(pos);
}

const char *mecab_jp_lattice_get_feature_constraint(mecab_jp_lattice_t *lattice,
                                                 size_t pos) {
  return reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->feature_constraint(pos);
}

void mecab_jp_lattice_set_boundary_constraint(mecab_jp_lattice_t *lattice,
                                           size_t pos, int boundary_type) {
  return reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->set_boundary_constraint(pos, boundary_type);
}

void mecab_jp_lattice_set_feature_constraint(mecab_jp_lattice_t *lattice,
                                          size_t begin_pos, size_t end_pos,
                                          const char *feature) {
  return reinterpret_cast<MeCabJp::Lattice *>(
      lattice)->set_feature_constraint(begin_pos, end_pos, feature);
}

void mecab_jp_lattice_set_result(mecab_jp_lattice_t *lattice,
                              const char *result) {
  return reinterpret_cast<MeCabJp::Lattice *>(lattice)->set_result(result);
}

const char *mecab_jp_lattice_strerror(mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<MeCabJp::Lattice *>(lattice)->what();
}

mecab_jp_model_t *mecab_jp_model_new(int argc, char **argv) {
  MeCabJp::Model *model = MeCabJp::createModel(argc, argv);
  if (!model) {
    MeCabJp::deleteModel(model);
    return 0;
  }
  return reinterpret_cast<mecab_jp_model_t *>(model);
}

mecab_jp_model_t *mecab_jp_model_new2(const char *arg) {
  MeCabJp::Model *model = MeCabJp::createModel(arg);
  if (!model) {
    MeCabJp::deleteModel(model);
    return 0;
  }
  return reinterpret_cast<mecab_jp_model_t *>(model);
}

void mecab_jp_model_destroy(mecab_jp_model_t *model) {
  MeCabJp::Model *ptr = reinterpret_cast<MeCabJp::Model *>(model);
  MeCabJp::deleteModel(ptr);
  ptr = 0;
}

mecab_jp_t *mecab_jp_model_new_tagger(mecab_jp_model_t *model) {
  return reinterpret_cast<mecab_jp_t *>(
      reinterpret_cast<MeCabJp::Model *>(model)->createTagger());
}

mecab_jp_lattice_t *mecab_jp_model_new_lattice(mecab_jp_model_t *model) {
  return reinterpret_cast<mecab_jp_lattice_t *>(
      reinterpret_cast<MeCabJp::Model *>(model)->createLattice());
}

int mecab_jp_model_swap(mecab_jp_model_t *model, mecab_jp_model_t *new_model) {
  return static_cast<int>(
      reinterpret_cast<MeCabJp::Model *>(model)->swap(
          reinterpret_cast<MeCabJp::Model *>(new_model)));
}

const mecab_jp_dictionary_info_t* mecab_jp_model_dictionary_info(
    mecab_jp_model_t *model) {
  return reinterpret_cast<const mecab_jp_dictionary_info_t *>(
      reinterpret_cast<MeCabJp::Model *>(model)->dictionary_info());
}

int mecab_jp_model_transition_cost(mecab_jp_model_t *model,
                                unsigned short rcAttr,
                                unsigned short lcAttr) {
  return reinterpret_cast<MeCabJp::Model *>(model)->transition_cost(
      rcAttr, lcAttr);
}

mecab_jp_node_t *mecab_jp_model_lookup(mecab_jp_model_t *model,
                                 const char *begin,
                                 const char *end,
                                 mecab_jp_lattice_t *lattice) {
  return reinterpret_cast<mecab_jp_node_t *>(
      reinterpret_cast<MeCabJp::Model *>(model)->lookup(
          begin, end,
          reinterpret_cast<MeCabJp::Lattice *>(lattice)));
}
