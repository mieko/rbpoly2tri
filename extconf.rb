require 'mkmf'

abs_root = File.expand_path(File.dirname(__FILE__))

$defs.push "-DNDEBUG -I#{abs_root}/throw_assert -isystem #{abs_root}/throw_assert"

# Collect all the poly2tri sources
$srcs = %w(rbpoly2tri.cc)
$srcs += Dir["./poly2tri/common/*.cc"]
$srcs += Dir["./poly2tri/sweep/*.cc"]

$objs = $srcs.map{|src| src.gsub(/\.cc$/, '.o')}

$cleanfiles = $objs

create_makefile('poly2tri')
