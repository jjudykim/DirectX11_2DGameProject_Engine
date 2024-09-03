#pragma once
#define REGISTER_STATE(stateClass) \
    StateFactory::Instance()->RegisterState(#stateClass, []() -> CState* { return new stateClass(); })

