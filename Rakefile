require 'rubygems'
require 'rake/gempackagetask'

spec = Gem::Specification.load(File.dirname(__FILE__) + '/ruby-gd.gemspec')

Rake::GemPackageTask.new(spec) do |pkg|
  pkg.need_tar = true
end
