/*
 * Hunspell
 * Ruby interface to hunspell spell checker
 *
 * Copyright 2007, Gabor SEBESTYEN
 *
 * == LICENSE
 *
 * This file is part of Hunspell.
 *
 * Hunspell is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hunspell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ruby.h"
#include "hunspell/hunspell.h"



/*
 * Object allocator
 */
static VALUE cHunspellAllocate (VALUE klass)
{
	// bind the pointer to hunspell hander to Hunspell object
	Hunhandle **ptr = malloc(sizeof(Hunhandle *));
	*ptr = (Hunhandle *)0;

	return Data_Wrap_Struct(klass, 0, 0, ptr);
}



/*
 * Constructor function
 *
 * Instantiate Hunspell with paths pointing to affix and dictionary files.
 *
 * Example:
 *   sp = Hunspell.new("hu.aff", "hu.dic") => (Hunspell object)
 *
 */
static VALUE mHunspellInitialize(VALUE self, VALUE affPath, VALUE dictPath) {
	Hunhandle *handler = Hunspell_create(StringValueCStr(affPath), StringValueCStr(dictPath));

	Hunhandle **ptr;
	Data_Get_Struct(self, Hunhandle *, ptr);
	
	if (!handler) {
		// crash!
		rb_raise(rb_eRuntimeError, "Failed to initialize Hunspell.");
	} else {
		*ptr = handler;
	}

#ifdef DEBUG
	printf("DEBUG: mHunspellInitialize: 0x%x\n", handler);
#endif
	return self;
}



/*
 * Spellcheck a word
 *
 * Input: word (String)
 * Output: found correct? (Boolean)
 *
 * Example:
 *   sp.spellcheck("ruby") => true
 *
 */
static VALUE mHunspellSpell(VALUE self, VALUE str) {
	int result;
	
	Hunhandle **ptr;
	Data_Get_Struct(self, Hunhandle *, ptr);

	result = Hunspell_spell(*ptr, (const char *)StringValueCStr(str));
#ifdef DEBUG
	printf("DEBUG: mHunspellSpell: %s -> %d\n", StringValueCStr(str), result);
#endif
	return (result == 1 ? Qtrue : Qfalse);
}



/*
 * Get suggestions to a word
 *
 * Input: word (String)
 * Output: list of suggestions (Array of Strings)
 *
 * Example:
 *   sp.suggest("sunmer") => ['summer', ...]
 *
 */
static VALUE mHunspellSuggest(VALUE self, VALUE str) {
	int n, i;
	char **lst;
	VALUE ret;
	
	Hunhandle **ptr;
	Data_Get_Struct(self, Hunhandle *, ptr);

	n = Hunspell_suggest(*ptr, &lst, (const char *)StringValueCStr(str));
	if (n > 0) {
		// allocate enough space in new array
		ret = rb_ary_new2(n);
		for (i=0; i<n; i++) {
			// add string to list
			VALUE rb_str = rb_str_new2(lst[i]);
			rb_ary_push(ret, rb_str);
		}
	} else {
		// create empty array
		ret = rb_ary_new();
	}

#ifdef DEBUG
	printf("mHunspellSuggest %s -> %d\n", StringValueCStr(str), n);
#endif

	return ret;
}



/*
 * Returns dictionary encoding
 *
 * Note: it may return 'ISO-8859-1' if dictionary not found or invalid.
 *
 * Output: encoding (String)
 *
 */
static VALUE mHunspellEncoding(VALUE self) {
	int result;
	
	Hunhandle **ptr;
	Data_Get_Struct(self, Hunhandle *, ptr);

	char *enc = Hunspell_get_dic_encoding(*ptr);
#ifdef DEBUG
	printf("DEBUG: mHunspellEncoding: %s\n", enc);
#endif
	return rb_str_new2(enc);
}


/*
 * Module entry point
 */
void Init_Hunspell() {
	VALUE	rb_cHunspell;
	
	// create Hunspell class
	rb_cHunspell = rb_define_class("Hunspell", rb_cObject);

	// register own allocator
	rb_define_alloc_func(rb_cHunspell, cHunspellAllocate);
	
	// register instance methods
	rb_define_method(rb_cHunspell, "initialize", mHunspellInitialize, 2);
	rb_define_method(rb_cHunspell, "spellcheck", mHunspellSpell, 1);
	rb_define_method(rb_cHunspell, "suggest", mHunspellSuggest, 1);
	rb_define_method(rb_cHunspell, "encoding", mHunspellEncoding, 0);
}
