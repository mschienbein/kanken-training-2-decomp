/* Independently authored preprocessing adapter, not SDK source.
 * The discovery build selects release assertions. No SDK function is defined.
 */
#pragma once
#ifndef NW_RELEASE
#error "This adapter supports release assertion configuration only"
#endif
#define NN_ASSERT_(condition) ((void)0)
#define NN_ASSERT(condition) ((void)0)
#define NN_ASSERTMSG(condition, ...) ((void)0)
#define NN_ASSERT_MSG(condition, ...) ((void)0)
#define NN_ASSERT_MINMAX(value, minimum, maximum) ((void)0)
#define NN_ASSERT_MINMAXLT(value, minimum, maximum) ((void)0)
#define NN_ASSERT_SDK(condition) ((void)0)
#define NN_ASSERT_SDK_MSG(condition, ...) ((void)0)
#define NN_ASSERT_SDK_MIN(value, minimum) ((void)0)
#define NN_ASSERT_SDK_MAX(value, maximum) ((void)0)
#define NN_ASSERT_SDK_RESULT(condition, result) ((void)0)

#define NN_POINTER_ASSERT_(pointer) ((void)0)
#define NN_POINTER_ASSERT(pointer) ((void)0)