#ifndef NUKLEAR_GAMEPAD_NONE_H__
#define NUKLEAR_GAMEPAD_NONE_H__

#if !defined(NK_GAMEPAD_MAX)
#define NK_GAMEPAD_MAX 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A gamepad input source that does not provide any callbacks.
 */
NK_API struct nk_gamepad_input_source nk_gamepad_none_input_source(void* user_data);

#ifdef __cplusplus
}
#endif

#endif  /* NUKLEAR_GAMEPAD_NONE_H__ */

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_NONE_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_NONE_IMPLEMENTATION_ONCE

#ifdef __cplusplus
extern "C" {
#endif

NK_API struct nk_gamepad_input_source nk_gamepad_none_input_source(void* user_data) {
    struct nk_gamepad_input_source source;
    source.user_data = user_data;
    source.init = NULL;
    source.update = NULL;
    source.free = NULL;
    source.name = NULL;
    source.input_source_name = "None";
    source.id = NK_GAMEPAD_INPUT_SOURCE_NONE;
    return source;
}

#ifdef __cplusplus
}
#endif

#endif  /* NUKLEAR_GAMEPAD_NONE_IMPLEMENTATION_ONCE */
#endif  /* NK_GAMEPAD_IMPLEMENTATION */
