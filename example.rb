require "rubygems" # import gem package manager
gem "hunspell"     # load Hunspell module
require "Hunspell" # inject Hunspell class to Ruby namespace

# instantiate Hunspell with Hungarian affix and dictionary files
#
sp = Hunspell.new("hu_HU.aff", "hu_HU.dic") 

# spell check Hungarian word 'ablak' (window) => true
#
puts "Is 'ablak' correct? #{sp.spellcheck('ablak')}"

# get suggestions for mispelled word 'paprika'
#   => ["kaprica", "patrica", "paprika", "papcica",
#       "papráca", "papruca", "paprima", "paprikáz",
#       "paprikása", "paprikás", "Papradnó"
#      ]
#
puts "Suggestions for 'paprica': " + sp.suggest("paprica").inspect

