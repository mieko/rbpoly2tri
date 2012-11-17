#include "../poly2tri/poly2tri.h"
#include <ruby.h>

using namespace p2t;

extern "C" {

/* Converts a ruby [x,y] to a Point */
static Point
value_to_point(VALUE ptval)
{
  Check_Type(ptval, T_ARRAY);
  if (RARRAY_LEN(ptval) != 2) {
    rb_raise(rb_eArgError, "%s", "Points are in format [x,y]");
  }
  return Point(NUM2DBL(rb_ary_entry(ptval, 0)),
               NUM2DBL(rb_ary_entry(ptval, 1)));
}

/* converts ruby point-arrays to vector<Point*> and return it.
 * if mem_track is given, each item returned is also appended
 * to it.  This allows the big-sweep delete finish. */
static std::vector<Point*>
ptary_to_vector(VALUE ary,
                std::vector<Point*> *mem_track = 0)
{
  /* We don't want to leave v in an inconsistent state if we
     find an error half-way through ary, so we copy to tmp,
     and then push into v if everything worked.  */
  std::vector<Point> tmp;

  Check_Type(ary, T_ARRAY);
  for (long i = 0; i != RARRAY_LEN(ary); i++) {
    VALUE item = rb_ary_entry(ary, i);
    tmp.push_back(value_to_point(item));
  }

  std::vector<Point*> result;
  for (size_t i = 0; i != tmp.size(); i++) {
    /* sigh, copy-ctor */
    Point* heaped = new Point(tmp[i].x, tmp[i].y);
    result.push_back(heaped);
    if (mem_track) {
      mem_track->push_back(heaped);
    }
  }
  return result;
}

/* Uh, for some reason, we own the points given to Poly2tri */
typedef struct {
  CDT *cdt;
  std::vector<Point*> mem_track;
} CDTWrap;

/* Return the underlying CDTWrap* of a value, type safe */
static inline CDTWrap*
XWRAP(VALUE x)
{
  CDTWrap *result;
  Data_Get_Struct(x, CDTWrap, result);
  return result;
}


static void
rb_cdt_free(void* ptr)
{
  CDTWrap *wrap = (CDTWrap*)(ptr);
  if (wrap->cdt) {
    delete wrap->cdt;
    wrap->cdt = 0;
  }

  /* every point given to the cdt should've ended up here */
  for (size_t i = 0; i != wrap->mem_track.size(); ++i) {
    delete wrap->mem_track[i];
  }
}

static VALUE
rb_cdt_new(VALUE klass, VALUE polyline)
{
  CDTWrap* ptr = new CDTWrap;
  ptr->cdt = 0;

  VALUE r = Data_Wrap_Struct(klass, 0, rb_cdt_free, ptr);
  rb_obj_call_init(r, 1, &polyline);
  return r;
}

static VALUE
rb_cdt_initialize(VALUE self, VALUE polyline)
{
  CDTWrap *ptr = XWRAP(self);

  if (! ptr->cdt) {
    ptr->cdt = new CDT(ptary_to_vector(polyline, &ptr->mem_track));
  }

  return Qnil;
}

static VALUE
rb_cdt_add_hole(VALUE self, VALUE polyline)
{
  CDTWrap *ptr = XWRAP(self);
  ptr->cdt->AddHole(ptary_to_vector(polyline, &ptr->mem_track));
  return Qnil;
}

static VALUE
rb_cdt_add_point(VALUE self, VALUE rp)
{
  /* Make sure it can be constructed from the ruby value before we go
   * off allocating memory */
  Point  p = value_to_point(rp);
  Point* heaped = new Point(p);

  /* Add it, and log so it can be free'd */
  CDTWrap* ptr = XWRAP(self);
  ptr->mem_track.push_back(heaped);
  ptr->cdt->AddPoint(heaped);

  return Qnil;
}

static VALUE
rb_cdt_triangulate(VALUE self)
{
  CDTWrap *ptr = XWRAP(self);

  /* I have never closely considered what would happen to the stack if I were
   * to rb_raise (longjmp) while the C++ stack is unwinding.  I'd prefer to 
   * keep that a mystery.
   */

  bool err = false;

  try {
    ptr->cdt->Triangulate();
  } catch (...) {
    err = true;
  }

  if (err) {
    rb_raise(rb_eRuntimeError, "Couldnt triangulate (duplicate vertices?)");
  }

  return Qnil;
}

static VALUE
rb_cdt_get_triangles(VALUE self)
{
  CDTWrap *ptr = XWRAP(self);
  CDT     *cdt = ptr->cdt;

  /* Notice I patched this in poly2tri to return a const-ref,
   * as this is a totally needless copy */
  const std::vector<Triangle*>& tris = cdt->GetTriangles();

  /* Result is a list of polys... */
  VALUE result = rb_ary_new();
  for(size_t i = 0; i != tris.size(); i++) {
    Triangle* tri = tris[i];
    /*  rb_tri is the current triangle... */
    VALUE rb_tri = rb_ary_new2(3);
    for(size_t ptidx = 0; ptidx != 3; ptidx++) {
      Point *pt = tri->GetPoint(ptidx);
      /* And we push each point as an [x,y] coordinate */
      rb_ary_push(rb_tri, rb_ary_new3(2, DBL2NUM(pt->x), DBL2NUM(pt->y)));
    }

    rb_ary_push(result, rb_tri);
  }

  return result;
}

typedef VALUE(*rubymethod)(...);

void Init_rbpoly2tri()
{
  VALUE m = rb_define_module("Poly2Tri");

  VALUE k = rb_define_class_under(m, "CDT", rb_cObject);
  rb_define_singleton_method(k, "new", (rubymethod) rb_cdt_new, 1);
  rb_define_method(k, "initialize", (rubymethod) rb_cdt_initialize, 1);
  rb_define_method(k, "add_hole", (rubymethod) rb_cdt_add_hole, 1);
  rb_define_method(k, "add_point", (rubymethod) rb_cdt_add_point, 1);
  rb_define_method(k, "triangulate!", (rubymethod) rb_cdt_triangulate, 0);
  rb_define_method(k, "triangles", (rubymethod) rb_cdt_get_triangles, 0);
//   rb_define_method(k, "map", (rubymethod) rb_cdt_get_map, 0);

}

} /* extern "C" */
