require 'mkmf'

Config::CONFIG['CPP'] = 'g++ -E'

if have_library('stdc++') and have_library('bayon')
  have_header('google/dense_hash_map') or have_header('ext/hash_map')
  create_makefile('bayonext')
end
