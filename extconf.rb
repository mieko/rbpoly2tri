require 'mkmf'

# Collect all the poly2tri sources
$srcs = %w(rbpoly2tri.cc)
$srcs += Dir["./poly2tri/common/*.cc"]
$srcs += Dir["./poly2tri/sweep/*.cc"]

$objs = $srcs.map{|src| src.gsub(/\.cc$/, '.o')}

$defs.push "-DNDEBUG"
$cleanfiles = $objs

puts $objs
create_makefile('poly2tri')
