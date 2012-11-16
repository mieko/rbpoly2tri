/*
 * poly2tri handles input errors with assert().  Some CPP trickery 
 * (extconf.rb) puts this directory ahead of the system directories, so
 * we can intercept <assert.h> and <cassert> and give this definition, 
 * which throws.
 *
 * Note, since poly2tri expects the program to terminate on a bad 
 * assert(), I wouldn't be surprised to learn if it leaves resources
 * dangling in certain code paths.
 *
 */

#undef assert
inline void assert(bool x) {
  if (! x) {
    throw false;
  }
}

