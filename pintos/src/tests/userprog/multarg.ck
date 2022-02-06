# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(args) begin
(args) argc = 4
(args) argv[0] = 'multarg'
(args) argv[1] = '-l'
(args) argv[2] = 'foo'
(args) argv[3] = 'bar'
(args) argv[4] = null
(args) end
multarg: exit(0)
EOF
pass;
