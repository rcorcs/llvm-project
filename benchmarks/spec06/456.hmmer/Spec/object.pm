$benchnum  = '456';
$benchname = 'hmmer';
$exename   = 'hmmer';
$benchlang = 'C';
$need_math = 'yes';
@base_exe  = ($exename);

$reltol   = 0.002;
$abstol   = 1.0e-5;
$skiptol  = 10;

@sources=qw(
alphabet.c
core_algorithms.c
debug.c
display.c
emit.c
emulation.c
fast_algorithms.c
histogram.c
hmmio.c
hmmcalibrate.c
hmmsearch.c
mathsupport.c
masks.c
misc.c
modelmakers.c
plan7.c
plan9.c
postprob.c
prior.c
tophits.c
trace.c 
ucbqsort.c
a2m.c
aligneval.c
alignio.c
clustal.c
cluster.c
dayhoff.c
eps.c
file.c
getopt.c
gki.c
gsi.c
hsregex.c
iupac.c
msa.c
msf.c
phylip.c
revcomp.c
rk.c
selex.c
seqencode.c
shuffle.c
sqerror.c
sqio.c
squidcore.c
sre_ctype.c
sre_math.c
sre_random.c
sre_string.c
ssi.c
stack.c
stockholm.c
translate.c
types.c
vectorops.c
weight.c
            );

sub invoke {
    my ($me) = @_;
    my @rc;

    for ($me->input_files_base) {
      if (($name) = m/(.*).hmm$/) {
	if ($name eq 'bombesin') {
	    push @rc, { 'command' => $me->exe_file, 
		        'args'    => [ "--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0", $_ ],
			'output'  => "$name.out",
			'error'   => "$name.err",
		      };
	} elsif ($name eq 'leng100') {
	    push @rc, { 'command' => $me->exe_file, 
		        'args'    => [ "--fixed 0 --mean 425 --num 85000 --sd 300 --seed 0", $_ ],
			'output'  => "$name.out",
			'error'   => "$name.err",
		      };
	} elsif ($name eq 'nph3') {
	    push @rc, { 'command' => $me->exe_file, 
		        'args'    => [ $_, 'swiss41'],
			'output'  => "$name.out",
			'error'   => "$name.err",
		      };
	} elsif ($name eq 'retro') {
	    push @rc, { 'command' => $me->exe_file, 
		        'args'    => [ "--fixed 0 --mean 500 --num 500000 --sd 350 --seed 0", $_ ],
			'output'  => "$name.out",
			'error'   => "$name.err",
		      };
	} 
      }
    }
    return @rc;
}
1;    
