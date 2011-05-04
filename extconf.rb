require 'mkmf'

prefix_list = %w(/usr/local /opt/local)

if with_config('prefix')
	prefix_list << with_config('prefix')
end

# try to find hunspell lib
good_prefix = '/usr/local'
prefix_list.each do |prefix|
	if find_library('hunspell-1.1', 'Hunspell_create', prefix + "/lib")
		good_prefix = prefix
		break
	end
end

# workaround for FreeBSD where hunspell package does not deploy header files
# you can add include dir explicitly
inc_dir = if with_config("include-dir")
	with_config("include-dir")
else
	good_prefix+"/include"
end

dir_config("Hunspell" , inc_dir, good_prefix+"/lib")

create_makefile("Hunspell")
