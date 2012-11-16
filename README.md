Synopsis
==========
These are Ruby 1.9.2+ bindings to poly2tri.  It produces very nice triangulated
meshes out of pretty arbitrary polygons.

This release contains an a (slightly-modified) version of Revision 31c314fae1
of the poly2tri C++ branch.  Both poly2tri and rbpoly2tri are available under
BSD/MIT-style permissive licenses.  See poly2tri/LICENSE and LICENSE for
details.

[poly2tri Homepage](http://code.google.com/p/poly2tri)

To install via RubyGems:

    gem install rbpoly2tri

To install from repo:

    gem build rbpoly2tri.gemspec
    gem install rbpoly2tri-<version>.gem

Simple Usage:
===========
    require 'rbpoly2tri'

    # The initial input must be simple (non-intersecting, no holes), and
    # repeated vertices are not allowed.  Holes may be added later.

    inp = [[100.0, 0.0], [0.0, 0.0], [0.0, 47.85714326530613],
           [-4.999999, 50.0], [0.0, 50.0], [0.0, 100.0], [100.0, 100.0],
           [100.0, 50.0], [200.0, 50.0], [100.0, 5.0]]

    # CDT.  You know, Constrained Delaunay Triangulation.
    tess = Poly2Tri::CDT.new(inp)

    # if you're in the mood to add a hole to this polygon, now's the time:
    tess.add_hole [[10, 10], [10,30], [30, 30], [30,10]]

    tess.triangulate!
    triangles = tess.triangles # => A huge amount of vertices, man.

See Also:
=========
 * If you're into this type of thing, you might want to check out
   [rbclipper](http://github.com/mieko/rbclipper), my bindings to Clipper, a
   polygon clipping library.
