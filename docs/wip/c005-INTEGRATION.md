# c005 (wave 5) — integration to do

Branch `c005-wave5`: 37 functions collected (re-derived MATCH at destination) into `src/wave5/`, candidates `config/candidates/c005-wave5.json`. Before prepare (after c004 is finalized and this branch is rebased onto main): unify these interface-gate findings (`tools/interfaces.py check` on accepted + c005), the way c004 was unified (shared prototypes/types into `include/game/`, evidence-based contracts). Authors noted: `data_00242a50` needs one typed `ServiceState` declaration (accepted wave-1 units use `unsigned char[]`); `func_0011ef54` prototype belongs in `game/runtime.h`.

```
error       bss_00290cac (declaration)
            src/wave1/func_00134f04.c:1  unsigned short
            src/wave5/func_0015d6e0.c:8  struct NotificationState
error       data_00242a50 (declaration)
            src/wave1/func_00195e84.c:1  array[] of (unsigned char)
            src/wave1/func_00195e94.c:1  array[] of (unsigned char)
            src/wave5/func_0018eed0.c:14  struct ServiceState
            src/wave5/func_00191d20.c:14  struct ServiceState
error       struct WordPair (layout)
            src/wave4/func_00136448.c:1  struct WordPair { unsigned int first; unsigned int second }
            src/wave5/func_001f28cc.c:12  struct WordPair { array[2] of (unsigned int) words }
policy      func_0017ded4 (inline-declaration)
            src/wave5/func_001533b8.c:12  function(pointer to (const struct RelativeReference)) -> pointer to (const void)
            src/wave5/func_001f28cc.c:28  function(pointer to (const struct RelativeReference)) -> pointer to (const void)
policy      func_0018eed0 (inline-declaration)
            src/wave5/func_00122b00.c:2  function(void) -> int
policy      func_0018fcb4 (inline-declaration)
            src/wave5/func_0019058c.c:5  function(void) -> pointer to (struct ServiceState)
policy      func_00191de8 (inline-declaration)
            src/wave5/func_00122b00.c:1  function(void) -> unsigned int
policy      func_00192400 (inline-declaration)
            src/wave5/func_00115040.c:1  function(unsigned int) -> void
```
