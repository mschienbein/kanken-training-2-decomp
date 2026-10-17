struct SceneControllerVTable;
struct SceneController {
    const SceneControllerVTable *vtable;
};

/* INTEGRATOR: move to include/game/scene_controller.h */
extern "C" {
extern SceneController *func_00174484(SceneController *object, unsigned first,
                                     unsigned second, unsigned third);
extern const SceneControllerVTable data_0022359c;
SceneController *func_001ab51c(SceneController *object);
}

SceneController *func_001ab51c(SceneController *object)
{
    object = func_00174484(object, 0, 0, 0);
    object->vtable = &data_0022359c;
    return object;
}
