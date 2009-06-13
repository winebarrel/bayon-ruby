require 'mkmf'

if have_library('stdc++') and have_library('bayon')
  create_makefile('bayonext')
end
