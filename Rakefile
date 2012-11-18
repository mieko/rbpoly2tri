require 'rake/testtask'
require 'rake/clean'

NAME = 'rbpoly2tri'

file "lib/#{NAME}/#{NAME}.o" => Dir.glob("ext/#{NAME}/*{.rb,.c,.cc,.h}") do
  Dir.chdir("ext/#{NAME}") do
    ruby "extconf.rb"
    sh "make"
  end
  cp "ext/#{NAME}/#{NAME}.o", "lib/#{NAME}"
end

# make the :test task depend on the shared
# object, so it will be built automatically
# before running the tests
task :test => "lib/#{NAME}/#{NAME}.o"

# use 'rake clean' and 'rake clobber' to
# easily delete generated files
CLEAN.include('ext/**/*{.o,.log,.so}')
CLEAN.include('ext/**/Makefile')
CLEAN.include('ext/**/*.bundle')
CLEAN.include("lib/#{NAME}")
CLOBBER.include('lib/**/*{.o,.so}')

# the same as before
Rake::TestTask.new do |t|
  t.libs << 'test'
end

desc "Run tests"
task :default => :test
