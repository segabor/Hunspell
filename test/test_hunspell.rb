require 'test/unit'
require "Hunspell"

class HunspellTest < Test::Unit::TestCase
  def setup
    @sp = Hunspell.new("test/dict/hu_HU.aff", "test/dict/hu_HU.dic")
  end

  def test_spellcheck
    assert @sp.spellcheck('ablak')
  end

  def test_false_spellcheck
    assert !@sp.spellcheck("paprica")
  end

  def test_suggestions
    assert !@sp.spellcheck('paprica')

    suggestions = @sp.suggest("paprica")
    assert !suggestions.include?('paprica')
    assert suggestions.include?('paprika')
  end

  def test_analyze
    assert @sp.analyze('paprika') == [" st:paprika po:noun ts:NOM"]
  end
end
