Gem::Specification.new do |s|
  s.name        = 'rbpoly2tri'
  s.version     = '0.0.1'
  s.date        = '2012-11-15'
  s.summary     = "Ruby Bindings to poly2tri"
  s.description = "Ruby Bindings to poly2tri: Constrained Delaunay Triangulation"
  s.authors     = ["Mike Owens", "Dimitri Roche"]
  s.files       = Dir.glob('lib/**/*.rb') +
                  Dir.glob('ext/**/*.{c,cc,h,rb}')
  s.extensions  = ['ext/rbpoly2tri/extconf.rb']
  s.email       = 'dimroc@gmail.com'
  s.homepage    = 'https://github.com/dimroc/rbpoly2tri'
end
