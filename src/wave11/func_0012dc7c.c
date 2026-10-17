#include "game/service_protocol.h"
unsigned func_0012dc7c(const struct ServiceObject *object)
{
    void *memory;
    unsigned result = func_0018fa44(func_0018fcb4(), &memory, object->handle);
    switch (result >> 31) {
    case 0:
        result = func_00136510(memory);
        switch (result >> 31) {
        case 0:
            result = func_001365e0(memory);
            switch (result >> 31) {
            case 0:
                return 0;
            }
        }
    }
    return result;
}
