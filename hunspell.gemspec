Gem::Specification.new do |s|
	s.name = 'hunspell'
	s.version = '0.1.2'
	s.date = '2013-08-20'
	s.summary = 'Ruby interface to hunspell spell checker'
	s.email = 'segabor@gmail.com'
	s.homepage = 'https://github.com/segabor/Hunspell'
	s.description = <<__TEXT
Hunspell is an easy native Ruby interface to the famous Hunspell spell checker
library which is part of OpenOffice and Mozilla products. With this bundle
you can start to develop your own AJAX based spell checker service for
Ruby on Rails.
__TEXT
	s.has_rdoc = false
	s.authors = ['Gábor SEBESTYÉN']
	s.files = %w(extconf.rb hunspell.c hunspell.rb TODO README example.rb LGPL_LICENSE)
	s.extensions = ["extconf.rb"]
	s.requirements = ["hunspell 1.2 or newer"]
	s.require_paths = ["lib", "."]
end
