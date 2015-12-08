require "pp"

h = Hash.new(0)
lines = File.readlines("messages.out")
lines.each do |line|
  line.chomp!
  h[line] += 1
end

pp h
