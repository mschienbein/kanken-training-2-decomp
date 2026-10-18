/* [INFERENCE] One-parent runtime-type metadata; original class name unrecovered. */
struct RuntimeTypeInfo {
    const RuntimeTypeInfo *parent;

    explicit RuntimeTypeInfo(const RuntimeTypeInfo *value) : parent(value) {}
};

/* INTEGRATOR: move to include/game/runtime_type_info.h */
extern "C" {
extern RuntimeTypeInfo data_00243814;
}

RuntimeTypeInfo data_00243810(&data_00243814);
