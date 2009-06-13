Gem::Specification.new do |spec|
  spec.name              = 'bayon'
  spec.version           = '0.1.0'
  spec.summary           = 'Ruby bindings for bayon.'
  spec.files             = Dir.glob('lib/*.*') + Dir.glob('ext/*.*') + %w(README)
  spec.author            = 'winebarrel'
  spec.email             = 'sgwr_dts@yahoo.co.jp'
  spec.homepage          = 'http://bayon.rubyforge.org'
  spec.extensions        = 'ext/extconf.rb'
  spec.has_rdoc          = true
  spec.rdoc_options      << '--title' << 'bayon/Ruby - Ruby bindings for bayon.'
  spec.extra_rdoc_files  = %w(README)
  spec.rubyforge_project = 'bayon'
end
