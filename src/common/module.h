/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_MODULE_H
#define BAZEL_TEMPLATE_COMMON_MODULE_H

/*
//////////////////////////////////////////////////////////////////////////////
// examplaes

// ===========================================================================
// in foo.hpp
int FooDoSomeThing();

// ===========================================================================
// in foo.cpp
DEFINE_MODULE(foo)
{
    // do some staff
    g_foo = InitializeFoo();
    // return true to report success
    return true;
}

// ===========================================================================
// in bar.cpp
// import foo declare
#include "foo.hpp"

DEFINE_MODULE(bar)
{
    // bar requires foo be initialized firstly
    g_bar = InitializeBar(FooDoSomeThing());
    return true;
}

// declare this module depends on foo
USING_MODULE(foo);

// ===========================================================================
// in main.cpp
#include "bazel_template/common/module.h"

int main(int argc, char** argv)
{
    // initialize all modules firstly
    InitAllModules(&argc, &argv, true);

    // ok, all modules were initialized, do other things
    ...
    return 0;
}

// ===========================================================================
// in BUILD
cc_library(
    name = 'foo',
    srcs = 'foo.cpp',
    alwayslink = 1 # NOTE: this is required by module registerers
)

cc_library(
    name = 'bar',
    srcs = 'bar.cpp',
    deps = ':foo',
    alwayslink = 1 # NOTE: this is required by module registerers
)

cc_binary(
    name = 'main',
    srcs = 'main.cpp',
    deps = ':bar'
)
*/

#include <assert.h>

namespace testing {
void InitGoogleTest(int* argc, char** argv);
}

namespace bazel_template {
namespace common {
namespace internal {

class ModuleRegisterer;

typedef bool (*ModuleCtor)();
typedef void (*ModuleDtor)();

/// Manage all registered modules and their dependancies
class ModuleManager {
 private:
  ModuleManager();

 public:
  /// register module, used by the DEFINE_MODULE macro.
  static void RegisterModuleCtor(const char* filename, int line,
                                 const char* name, ModuleCtor ctor);

  static void RegisterModuleDtor(const char* filename, int line,
                                 const char* name, ModuleDtor dtor);

  /// register module dependency, used by the USING_MODULE macro.
  static void RegisterDependency(const char* filename, int line,
                                 const char* this_module, const char* dep_name,
                                 ModuleRegisterer* module_registerer);

  // For writing log in module dtor.
  static void LogModuleDtor(const char* name);

  /// initialize all registered modules after parsing command line flags
  static void InitializeAll(int* argc, char*** argv, bool remove_flags);
};

/// Register module to manager automatically
/// used by DEFINE_MODULE
class ModuleRegisterer {
 public:
  ModuleRegisterer(const char* filename, int line, const char* name,
                   ModuleCtor ctor) {
    ModuleManager::RegisterModuleCtor(filename, line, name, ctor);
  }
};

/// Register module dtor to manager automatically
/// used by DEFINE_MODULE_DTOR
class ModuleDtorRegisterer {
 public:
  ModuleDtorRegisterer(const char* filename, int line, const char* name,
                       ModuleDtor dtor) {
    ModuleManager::RegisterModuleDtor(filename, line, name, dtor);
  }
};

/// register module dependency to module manager automatically
/// used by USING_MODULE macro
class ModuleDependencyRegisterer {
 public:
  ModuleDependencyRegisterer(const char* filename, int line,
                             const char* this_module_name,
                             const char* dependency_name,
                             ModuleRegisterer* module_registerer = nullptr) {
    ModuleManager::RegisterDependency(filename, line, this_module_name,
                                      dependency_name, module_registerer);
  }
};

}  // namespace internal

/// Initialize all registered modules in dependency order.
/// @param argc pointer to argc of main
/// @param argv pointer to argv of main
/// @param remove_flags whether remove processed flags
///
/// @note
/// The *argc and *argv may be changed if remove_flags is specified.
///
/// This function always initialize gflags and glog internally, you should
/// never do it again.
///
/// Gtest defined some its own flags and only process and remove recognized
/// flags to it, but gflags is always trying process and remove all flags.
/// If you are using gtest, make sure to call InitGoogleTesting befor this
/// function, or gflags will report unknown flags and gtest will not receive
/// its flags.
inline void InitAllModules(int* argc, char*** argv, bool remove_flags = true) {
  ::bazel_template::common::internal::ModuleManager::InitializeAll(
      argc, argv, remove_flags);
}

/// Initialize all registered modules and also gtest
inline void InitAllModulesAndTest(int* argc, char*** argv,
                                  bool remove_flags = true) {
  ::testing::InitGoogleTest(argc, *argv);
  ::bazel_template::common::internal::ModuleManager::InitializeAll(
      argc, argv, remove_flags);
}

}  // namespace common
}  // namespace bazel_template

//////////////////////////////////////////////////////////////////////////////
// helper macros to make module easier to use
#ifdef __GNUC__
#define PP_DISALLOW_IN_HEADER_FILE()    \
  static_assert(__INCLUDE_LEVEL__ == 0, \
                "This macro can not be used in header files");
#else
#define PP_DISALLOW_IN_HEADER_FILE()
#endif

/// macro to make module registration easier
/// @param name symbolic module name, should be global unique
/// @note DEFINE_MODULE should be used in GLOBAL namespace
#define DEFINE_MODULE(name)                                                    \
  static const char THIS_MODULE_NAME[] = #name;                                \
  static bool name##_ModuleCtor();                                             \
  namespace modules {                                                          \
  PP_DISALLOW_IN_HEADER_FILE()                                                 \
  ::bazel_template::common::internal::ModuleRegisterer                         \
      name##_module_registerer(__FILE__, __LINE__, #name, &name##_ModuleCtor); \
  }                                                                            \
                                                                               \
  static bool name##_ModuleCtor()

/// macro to make module registration easier
/// @param name symbolic module name, should be global unique
/// @note DEFINE_MODULE_DTOR should be used in GLOBAL namespace
#define DEFINE_MODULE_DTOR(name)                                             \
  static void name##_ModuleDtor();                                           \
  namespace modules {                                                        \
  PP_DISALLOW_IN_HEADER_FILE()                                               \
  ::bazel_template::common::internal::ModuleDtorRegisterer                   \
      name##_module_dtor_registerer(__FILE__, __LINE__, #name,               \
                                    &name##_ModuleDtor);                     \
  }                                                                          \
                                                                             \
  static void name##_DoModuleDtor();                                         \
  static void name##_ModuleDtor() {                                          \
    ::bazel_template::common::internal::ModuleManager::LogModuleDtor(#name); \
    name##_DoModuleDtor();                                                   \
  }                                                                          \
  static void name##_DoModuleDtor()

/// macro to make module dependency registration easier.
/// using compile time symbolic name, name must be declared using DECLARE_MODULE
/// @note USING_MODULE should be used in GLOBAL namespace
#define USING_MODULE(dep_name)                                                 \
  namespace modules {                                                          \
  PP_DISALLOW_IN_HEADER_FILE()                                                 \
  extern ::bazel_template::common::internal::ModuleRegisterer                  \
      dep_name##_module_registerer;                                            \
  static ::bazel_template::common::internal::ModuleDependencyRegisterer        \
      using_module_##dep_name(__FILE__, __LINE__, THIS_MODULE_NAME, #dep_name, \
                              &dep_name##_module_registerer);                  \
  }

#define MODULE_DEP(module_name, dep_name)                                  \
  namespace modules {                                                      \
  PP_DISALLOW_IN_HEADER_FILE()                                             \
  extern ::bazel_template::common::internal::ModuleRegisterer              \
      dep_name##_module_registerer;                                        \
  static ::bazel_template::common::internal::ModuleDependencyRegisterer    \
      using_module_##dep_name(__FILE__, __LINE__, #module_name, #dep_name, \
                              &dep_name##_module_registerer);              \
  }

#endif  // BAZEL_TEMPLATE_COMMON_MODULE_H
