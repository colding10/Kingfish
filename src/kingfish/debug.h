#ifndef KINGFISH_DEBUG_H
#define KINGFISH_DEBUG_H

#include <functional>
#include <sstream>
#include <string>
#include <string_view>

//#define KINGFISH_ASSERTS_ON

#ifndef NDEBUG
#define KINGFISH_ASSERTS_ON
#endif
#ifdef KINGFISH_ASSERTS_ON

#define KINGFISH_ASSERT(cond, msg)                         \
    do {                                                   \
        if (!(cond)) {                                     \
            std::stringstream ASSERTSSTREAM_;              \
            ASSERTSSTREAM_ << msg;                         \
            std::string ASSERTSTR_ = ASSERTSSTREAM_.str(); \
            ::kingfish::debug::assertionFailure(      \
                __FILE__, __func__, __LINE__, ASSERTSTR_); \
        }                                                  \
    } while (false)

#else

#define KINGFISH_ASSERT(cond, msg) \
    do {                           \
    } while (false)

#endif

#define KINGFISH_NOT_NULL(ptr) \
    KINGFISH_ASSERT(ptr != nullptr, "'" #ptr "' cannot be NULL.")

namespace kingfish::debug {

bool assertsEnabledInLib();

using AssertionFailHandler = std::function<void(const char*      fileName,
                                                const char*      funcName,
                                                int              line,
                                                std::string_view msg)>;

/**
	Asserts that a given condition is true. If the assertion fails, the last set assertion fail
	handler is called. By default, this handler dumps the assertion failure message to STDERR,
	awaits for a keystroke (std::cin.get()) and exits the program. The handler can be changed
	via 'setAssertFailHandler'.

	Note: It is not advisable to call this function directly. Use the macro KINGFISH_ASSERT instead.
*/
void assertionFailure(const char*      fileName,
                      const char*      funcName,
                      int              line,
                      std::string_view msg);

/**
	Sets the assertion handler. If nullptr is passed, resets to the default assertion handler.
*/
void setAssertFailHandler(AssertionFailHandler h);

} // namespace kingfish::debug

#endif // KINGFISH_DEBUG_H