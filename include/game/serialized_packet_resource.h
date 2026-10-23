#ifndef GAME_SERIALIZED_PACKET_RESOURCE_H
#define GAME_SERIALIZED_PACKET_RESOURCE_H

/* [INFERENCE] This is the serialized resource's addressed word table, not
 * a native material PackedResource object or a runtime storage alias. */
struct SerializedPacketResource {
    unsigned reserved00;
    unsigned section_word_offset;
    unsigned words[];
};

#ifdef __cplusplus
extern "C" {
#endif
unsigned func_0010cb9c(const struct SerializedPacketResource *resource, unsigned input_size);
#ifdef __cplusplus
}
#endif
#endif
