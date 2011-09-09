#ifndef MAGICDEFS_H
#define MAGICDEFS_H

#if __GNUC__
#define GCC_VERSION  (__GNUC__ * 10000 \
  + __GNUC_MINOR__ * 100 \
  + __GNUC_PATCHLEVEL__)
#else
#define GCC_VERSION 0
#endif

#if GCC_VERSION < 40600
#define nullptr 0
#warning nullptr defined as 0
#else
#warning assume nullptr is defined properly
#endif


#endif // MAGICDEFS_H
