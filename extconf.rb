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


dir_config('hunspell-1.3', HEADER_DIRS, LIB_DIRS)

if have_library('hunspell-1.3', 'Hunspell_create', ['hunspell/hunspell.h'])
then
  create_makefile('Hunspell')
else
  puts('Missing hunspell library')
end

