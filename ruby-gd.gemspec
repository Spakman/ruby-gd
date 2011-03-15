Gem::Specification.new do |s|
  s.name       = "ruby-gd"
  s.version    = "0.8.0"
  s.author     = "Ryuichi Tamura"
  s.email      = "tam at kais dot kyoto-u dot ac dot jp"
  s.homepage   = "http://tam.0xfa.com/ruby-gd"
  s.platform   = Gem::Platform::RUBY
  s.summary    = "An interface to Boutell GD library"
  s.files      = ["Changes", "extconf.rb", "GD.c"] + Dir.glob("readme.*") + Dir.glob("{doc,sample}/**/*")
  s.require_path      = "lib"
  s.extensions        = "extconf.rb"
  s.autorequire       = "GD"
  s.has_rdoc          = true
  s.extra_rdoc_files  = ['readme.en']
end
