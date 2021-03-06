Gem::Specification.new do |s|
	s.name = 'hunspell'
	s.version = '1.1.0'
	s.date = '2018-11-19'
	s.summary = 'Ruby interface to hunspell spell checker'
	s.email = 'segabor@gmail.com'
	s.homepage = 'https://github.com/segabor/Hunspell'
	s.license = 'LGPL-3.0'
	s.description = <<__TEXT
Hunspell is an easy native Ruby interface to the famous Hunspell spell checker
library which is part of OpenOffice and Mozilla products. With this bundle
you can start to develop your own AJAX based spell checker service for
Ruby on Rails.
__TEXT
	s.has_rdoc = false
	s.authors = ['Gábor SEBESTYÉN']
	s.files = %w(extconf.rb hunspell.c hunspell.rb README.md example.rb LGPL_LICENSE)
	s.extensions = ["extconf.rb"]
	s.requirements = ["hunspell 1.3 or newer"]
	s.require_paths = ["lib", "."]

	s.test_files = Dir.glob('test/test_*.rb')
end
