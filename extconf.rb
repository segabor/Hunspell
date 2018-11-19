require 'mkmf'

LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']

HEADER_DIRS = [
  '/usr/local/include',

  INCLUDEDIR,

  '/usr/include'
]

LIB_DIRS = [
  '/usr/local/lib',

  LIBDIR,

  '/usr/lib'
]

HUNSPELL_TEST_HEADERS = ['hunspell/hunspell.h']
HUNSPELL_TEST_FUNCTION = 'Hunspell_create'

# Hunspell libraries to test for, in order of precedence
HUNSPELL_LIBRARIES = %w(1.7 1.6 1.5 1.4 1.3).map {|ver| "hunspell-#{ver}"}

HUNSPELL_LIBRARIES.each do |library|
  dir_config(library, HEADER_DIRS, LIB_DIRS)
end

# Link with first matching library
if HUNSPELL_LIBRARIES.any? { |library| have_library(library, HUNSPELL_TEST_FUNCTION, HUNSPELL_TEST_HEADERS) }
then
  create_makefile('Hunspell')
else
  STDERR.puts('Missing hunspell library')
  STDERR.puts('Supported versions: ' + HUNSPELL_LIBRARIES.join(' '))
end

