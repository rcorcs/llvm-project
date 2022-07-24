$benchnum  = '999';
$benchname = 'specrand';
$exename   = 'specrand';
$benchlang = 'C';
@base_exe  = ($exename);

$floatcompare = 1;

@sources=qw(main.c specrand.c);

sub invoke {
    my ($me) = @_;
    my (@rc);

    my @temp = main::read_file('control');
    my $exe = $me->exe_file;

    for (@temp) {
	my ($seed, $count) = split;
	next if m/^\s*#/ || m/^\s*$/;
	push (@rc, { 'command' => $exe, 
	             'args'    => [ $seed, $count ], 
                     'output'  => "rand.$count.out",
                     'error'   => "rand.$count.err"
		    });
    }
    return @rc;
}

1;
