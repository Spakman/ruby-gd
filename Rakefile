require 'rubygems'
Gem::manage_gems
require 'rake/gempackagetask'

spec = Gem::Specification.new do |s|
    s.name       = "ruby-gd"
    s.version    = "0.8.0"
    s.author     = "Ryuichi Tamura"
    s.email      = "tam at kais dot kyoto-u dot ac dot jp"
    s.homepage   = "http://tam.0xfa.com/ruby-gd"
    s.platform   = Gem::Platform::RUBY
    s.summary    = "An interface to Boutell GD library"
    s.files      = FileList["Changes", "extconf.rb", "GD.c", "readme.*", "{doc,sample}/**/*"].exclude("rdoc").to_a
    s.require_path      = "lib"
    s.extensions        = "extconf.rb"
    s.autorequire       = "GD"
    s.has_rdoc          = true
    s.extra_rdoc_files  = ['readme.en']
end

Rake::GemPackageTask.new(spec) do |pkg|
    pkg.need_tar = true
end
