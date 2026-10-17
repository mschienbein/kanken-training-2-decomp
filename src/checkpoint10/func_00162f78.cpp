struct SceneControllerVTable;
struct SceneController {
    const SceneControllerVTable *vtable;
};

/* INTEGRATOR: move to include/game/scene_controller.h */
extern "C" {
extern SceneController *func_00174484(SceneController *object, unsigned first,
                                     unsigned second, unsigned third);
extern const SceneControllerVTable data_00223518;
SceneController *func_00162f78(SceneController *object);
}

SceneController *func_00162f78(SceneController *object)
{
    object = func_00174484(object, 0, 0, 0);
    object->vtable = &data_00223518;
    return object;
}
