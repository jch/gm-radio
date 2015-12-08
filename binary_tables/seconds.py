#! /usr/bin/env python

data = []

for line in open('seconds.csv'):
    line = line.rstrip()
    if line.startswith('seconds'):
        continue
    (seconds, message, inverted) = line.split(',')
    data.append([int(seconds), message])

for (seconds, message) in data:
    checksum = sum(c == '1' for c in message) & 0x01
    if checksum != 0:
        raise RuntimeError("invalid checksum")
    # Reverse the bits:
    binary = message[::-1]
    # Discard the parity bit:
    binary = binary[1:]
    # Left-pad with zeros:
    binary = '0' * (8 - len(binary)) + binary
    # Now it's binary-coded decimal. Decode it, one nibble at a time:
    i = int(binary[:4], 2) * 10 + int(binary[4:], 2)
    decoded_seconds = i + 1
    if decoded_seconds != seconds:
        raise RuntimeError("invalid conversion")
    print seconds, binary, decoded_seconds
