# Hunspell [![Gem Version](https://badge.fury.io/rb/hunspell.svg)](https://badge.fury.io/rb/hunspell) [![Build Status](https://travis-ci.org/segabor/Hunspell.svg?branch=master)](https://travis-ci.org/segabor/Hunspell)

Ruby interface to hunspell spell checker
Copyright 2007, Gabor SEBESTYEN

## WHAT IS THIS?

Hunspell is an simple native Ruby interface to the famous [Hunspell](http://hunspell.sourceforge.net/) spell checker library which is part of [OpenOffice](http://openoffice.org/) and Mozilla products. With this bundle you can start to develop your own AJAX based spell checker service for Ruby on Rails.

## REQUIREMENTS

Before installing Hunspell ensure you have the following components already installed:

- Ruby >= 1.8
- rubygems
- hunspell native library >= 1.3 (libhunspell-1.3)
- dictionary files: download from [LibreOffice](http://cgit.freedesktop.org/libreoffice/dictionaries/tree/) or [OpenOffice](http://wiki.services.openoffice.org/wiki/Dictionaries) websites.

## INSTALLATION

The best way to get Hunspell is to use gem package manager
```sh
gem install hunspell
```
NOTE: if install fails on newer OSX versions, try this command
```sh
sudo ARCHFLAGS="-arch x86_64" gem install hunspell
```
If you want to build Hunspell from scratch grab the source from [here](https://github.com/segabor/Hunspell) and execute the
following command
```sh
ruby extconf.rb && make
```

FreeBSD gotcha: hunspell spell checker package does not deploy its header
files. Run gem or ruby command with an extra option:
```sh
gem install hunspell -- --with-include-dir=<incdir>
```
or
```sh
ruby extconf.rb --with-include-dir=<incdir> && make
```	
Replace _incdir_ with the path pointing to hunspell includes.

## FIRST STEPS

Here's a basic example how to use Hunspell. Cut it and run in ruby.

### example.rb

```rb
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
```

**Note** you might run this example with -Ke ruby option if you want to see accented letters instead of backslash prefixed utf codes.

## FEEDBACK

Any help or report are warmly appreciated. Please visit the project's homepage at https://github.com/segabor/Hunspell and open an issue.
