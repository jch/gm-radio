class Measure
  def initialize(input:, output: $stdout)
    @input = input
    @output = output
  end

  # For a table given on stdin:
  #
  # state, time (header not included)
  # 1 0
  # 1 5
  # 1 7
  # 0 10
  # 0 20
  # 1 33
  # 1 40
  #
  # Output a table of for the duration of each state:
  #
  # state, time
  # 1 10 (pull high for 5us)
  # 0 23 (pull low for 23us 33 - 10)
  # 1 7 (pull high for 7us 40 - 33)
  def process!
    # initialize values
    previous_state, previous_time = @input.readline.split(" ")

    until @input.eof?
      state, time = @input.readline.split(" ")

      # for the last state, always print the duration even if it hasn't flipped
      if @input.eof?
        duration = time.to_i - previous_time.to_i
        @output.puts "#{previous_state} #{duration}"
        break
      else
        # didn't flip in state, look ahead
        next if state == previous_state

        duration = time.to_i - previous_time.to_i
        @output.puts "#{previous_state} #{duration}"

        previous_state, previous_time = state, time
      end
    end
  rescue EOFError => e
  end
end
