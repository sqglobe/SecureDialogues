#ifndef EXPORTDEFINITION_H
#define EXPORTDEFINITION_H

#if defined _WIN32 || defined __CYGWIN__
#ifdef WIN_EXPORT
// Exporting...
#define EXPORTED \
  __declspec(    \
      dllexport)  // Note: actually gcc seems to also supports this syntax.
#else
#define EXPORTED \
  __declspec(    \
      dllimport)  // Note: actually gcc seems to also supports this syntax.
#endif
#define NOT_EXPORTED
#else
#if __GNUC__ >= 4
#define EXPORTED __attribute__((visibility("default")))
#define NOT_EXPORTED __attribute__((visibility("hidden")))
#else
#define EXPORTED
#define NOT_EXPORTED
#endif
#endif

#endif  // EXPORTDEFINITION_H
