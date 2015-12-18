require "stringio"
require_relative "../lib/measure"
require_relative "test_helper"

class MeasureTest < Minitest::Test
  def setup
    @output = StringIO.new
  end

  def test_process_empty_outputs_nothing
    input = StringIO.new ""
    measure = Measure.new(input: input, output: @output)
    measure.process!
    assert_equal "", @output.read
  end

  def test_process_single_line_outputs_nothing
    input = StringIO.new "1 1234\n"
    measure = Measure.new(input: input, output: @output)
    measure.process!
    @output.rewind
    assert_equal "", @output.read
  end

  def test_process_multi_line_outputs_states_with_time
    input = StringIO.new "1 10\n1 20\n1 30\n0 40\n0 50"
    measure = Measure.new(input: input, output: @output)
    measure.process!
    @output.rewind
    assert_equal "1 30\n0 10\n", @output.read
  end

  def test_process_multi_line_dangling_state_outputs_states_with_time
    input = StringIO.new "1 10\n0 20"
    measure = Measure.new(input: input, output: @output)
    measure.process!
    @output.rewind
    assert_equal "1 10\n", @output.read
  end
end
