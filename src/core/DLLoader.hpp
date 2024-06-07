/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "error/Error.hpp"
#include <dlfcn.h>
#include <memory>

using std::unique_ptr;

namespace arcade::core {
    /**
     * @class DlCloser
     * @brief A functor class used to close a dynamic library handle.
     *
     * This class provides a functor that can be used
     * as a custom deleter for a unique_ptr
     * to close a dynamic library handle obtained from dlopen().
     */
    class DlCloser {
        public:
            void operator()(void *const ptr) const
            {
                ::dlclose(ptr);
            }
        };

    template <typename T>
    /**
     * @class DLLoader
     * @brief A class for loading dynamic-link libraries (DLLs)
     * and obtaining instances from them.
     */
    class DLLoader {
        public:
            explicit DLLoader(const char *const libraryPath)
            {
                mHandle = unique_ptr<void, DlCloser>(
                    ::dlopen(libraryPath, RTLD_LAZY)
                );
                if (mHandle == nullptr) {
                    throw LibraryError(::dlerror());
                }
            }

            T *getInstance()
            {
                using CreateInstanceFunc = T *(*)();
                auto createInstance = reinterpret_cast<CreateInstanceFunc>(
                    ::dlsym(mHandle.get(), "create")
                );

                if (!createInstance) {
                    throw LibraryError(::dlerror());
                }
                return createInstance();
            }

            [[nodiscard]] void *get() const
            {
                return mHandle.get();
            }
        private:
            unique_ptr<void, DlCloser> mHandle;
    };
}
