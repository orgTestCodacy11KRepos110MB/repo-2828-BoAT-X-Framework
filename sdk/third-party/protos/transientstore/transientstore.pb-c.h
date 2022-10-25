/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: transientstore/transientstore.proto */

#ifndef PROTOBUF_C_transientstore_2ftransientstore_2eproto__INCLUDED
#define PROTOBUF_C_transientstore_2ftransientstore_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "ledger/rwset/rwset.pb-c.h"
#include "common/collection.pb-c.h"

typedef struct _Transientstore__TxPvtReadWriteSetWithConfigInfo Transientstore__TxPvtReadWriteSetWithConfigInfo;
typedef struct _Transientstore__TxPvtReadWriteSetWithConfigInfo__CollectionConfigsEntry Transientstore__TxPvtReadWriteSetWithConfigInfo__CollectionConfigsEntry;


/* --- enums --- */


/* --- messages --- */

struct  _Transientstore__TxPvtReadWriteSetWithConfigInfo__CollectionConfigsEntry
{
  ProtobufCMessage base;
  char *key;
  Common__CollectionConfigPackage *value;
};
#define TRANSIENTSTORE__TX_PVT_READ_WRITE_SET_WITH_CONFIG_INFO__COLLECTION_CONFIGS_ENTRY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&transientstore__tx_pvt_read_write_set_with_config_info__collection_configs_entry__descriptor) \
    , NULL, NULL }


/*
 * TxPvtReadWriteSetWithConfigInfo encapsulates the transaction's private
 * read-write set and additional information about the configurations such as
 * the latest collection config when the transaction is simulated
 */
struct  _Transientstore__TxPvtReadWriteSetWithConfigInfo
{
  ProtobufCMessage base;
  protobuf_c_boolean has_endorsed_at;
  uint64_t endorsed_at;
  Rwset__TxPvtReadWriteSet *pvt_rwset;
  size_t n_collection_configs;
  Transientstore__TxPvtReadWriteSetWithConfigInfo__CollectionConfigsEntry **collection_configs;
};
#define TRANSIENTSTORE__TX_PVT_READ_WRITE_SET_WITH_CONFIG_INFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&transientstore__tx_pvt_read_write_set_with_config_info__descriptor) \
    , 0,0, NULL, 0,NULL }


/* Transientstore__TxPvtReadWriteSetWithConfigInfo__CollectionConfigsEntry methods */
void   transientstore__tx_pvt_read_write_set_with_config_info__collection_configs_entry__init
                     (Transientstore__TxPvtReadWriteSetWithConfigInfo__CollectionConfigsEntry         *message);
/* Transientstore__TxPvtReadWriteSetWithConfigInfo methods */
void   transientstore__tx_pvt_read_write_set_with_config_info__init
                     (Transientstore__TxPvtReadWriteSetWithConfigInfo         *message);
size_t transientstore__tx_pvt_read_write_set_with_config_info__get_packed_size
                     (const Transientstore__TxPvtReadWriteSetWithConfigInfo   *message);
size_t transientstore__tx_pvt_read_write_set_with_config_info__pack
                     (const Transientstore__TxPvtReadWriteSetWithConfigInfo   *message,
                      uint8_t             *out);
size_t transientstore__tx_pvt_read_write_set_with_config_info__pack_to_buffer
                     (const Transientstore__TxPvtReadWriteSetWithConfigInfo   *message,
                      ProtobufCBuffer     *buffer);
Transientstore__TxPvtReadWriteSetWithConfigInfo *
       transientstore__tx_pvt_read_write_set_with_config_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   transientstore__tx_pvt_read_write_set_with_config_info__free_unpacked
                     (Transientstore__TxPvtReadWriteSetWithConfigInfo *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Transientstore__TxPvtReadWriteSetWithConfigInfo__CollectionConfigsEntry_Closure)
                 (const Transientstore__TxPvtReadWriteSetWithConfigInfo__CollectionConfigsEntry *message,
                  void *closure_data);
typedef void (*Transientstore__TxPvtReadWriteSetWithConfigInfo_Closure)
                 (const Transientstore__TxPvtReadWriteSetWithConfigInfo *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor transientstore__tx_pvt_read_write_set_with_config_info__descriptor;
extern const ProtobufCMessageDescriptor transientstore__tx_pvt_read_write_set_with_config_info__collection_configs_entry__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_transientstore_2ftransientstore_2eproto__INCLUDED */