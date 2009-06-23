Gem::Specification.new do |spec|
  spec.name              = 'bayon1.9'
  spec.version           = '0.1.1'
  spec.platform          = 'mswin32'
  spec.summary           = 'Ruby bindings for bayon.'
  spec.require_paths     = %w(lib lib/i386-mswin32)
  spec.files             = Dir.glob('lib/*.*') + %w(lib/i386-mswin32/bayonext.so README COPYING)
  spec.author            = 'winebarrel'
  spec.email             = 'sgwr_dts@yahoo.co.jp'
  spec.homepage          = 'http://bayon.rubyforge.org'
  spec.has_rdoc          = true
  spec.rdoc_options      << '--title' << 'bayon/Ruby - Ruby bindings for bayon.'
  spec.extra_rdoc_files  = %w(README)
  spec.rubyforge_project = 'bayon'
end
