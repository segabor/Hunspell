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

#ifdef HAVE_RUBY_ENCODING_H

#include <ruby/encoding.h>

#define ENCODED_STR_NEW2(str, encoding) \
	  ({ \
	    VALUE _string = rb_str_new2((const char *)str); \
	    int _enc = rb_enc_find_index(encoding); \
	    if (_enc != -1) { \
	      rb_enc_associate_index(_string, _enc); \
	    } \
	    _string; \
	  })

#else

#define ENCODED_STR_NEW2(str, encoding) \
  rb_str_new2((const char *)str)

#endif


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
		const char *enc = Hunspell_get_dic_encoding(*ptr);

		// allocate enough space in new array
		ret = rb_ary_new2(n);
		for (i=0; i<n; i++) {
			// add string to list
			VALUE rb_str = ENCODED_STR_NEW2(lst[i], enc);
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
 * Get Morphological analysis of the word
 *
 * Input: word (String)
 * Output: list of analysis results (Array of Strings)
 *
 * Example:
 *   sp.analyze("paprika") => [' st:paprika po:noun ts:NOM']
 *
 */
static VALUE mHunspellAnalyze(VALUE self, VALUE str) {
	int n, i;
	char **lst;
	VALUE ret;

	Hunhandle **ptr;
	Data_Get_Struct(self, Hunhandle *, ptr);

	n = Hunspell_analyze(*ptr, &lst, (const char *)StringValueCStr(str));
	if (n > 0) {
		const char *enc = Hunspell_get_dic_encoding(*ptr);

		// allocate enough space in new array
		ret = rb_ary_new2(n);
		for (i=0; i<n; i++) {
			// add string to list
			VALUE rb_str = ENCODED_STR_NEW2(lst[i], enc);
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
	Hunhandle **ptr;
	Data_Get_Struct(self, Hunhandle *, ptr);

	char *enc = Hunspell_get_dic_encoding(*ptr);
#ifdef DEBUG
	printf("DEBUG: mHunspellEncoding: %s\n", enc);
#endif
	return ENCODED_STR_NEW2(enc, "UTF-8");
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
	rb_define_method(rb_cHunspell, "analyze", mHunspellAnalyze, 1);
	rb_define_method(rb_cHunspell, "encoding", mHunspellEncoding, 0);
}
