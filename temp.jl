
# setup buffer
buffer = IOBuffer()

# print to buffer
print(buffer, "ABCDEF")

#insert mark, cursor is here
mark(buffer)

# print more to buffer
print(buffer, "|GHIJKLMNOPQRSTUVWXYZ")

# jump back to mark
reset(buffer)

# insert carriage return
print(buffer, "\r")

# flush buffer to stdout
write(stdout, take!(buffer))