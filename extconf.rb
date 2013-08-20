require 'mkmf'

# Min version
HUNSPELL_VER='1.2'

find_library("hunspell-#{HUNSPELL_VER}", 'Hunspell_create')

# dir_config("Hunspell" , inc_dir, good_prefix+"/lib")

create_makefile("Hunspell")
