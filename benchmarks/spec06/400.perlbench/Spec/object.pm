$benchnum  = '400';
$benchname = 'perlbench';
$exename   = 'perlbench';
$benchlang = 'C';
@base_exe  = ($exename);

@sources=qw(av.c deb.c doio.c doop.c dump.c globals.c gv.c hv.c locale.c
            mg.c numeric.c op.c pad.c perl.c perlapi.c perlio.c perlmain.c
            perly.c pp.c pp_ctl.c pp_hot.c pp_pack.c pp_sort.c pp_sys.c
            regcomp.c regexec.c run.c scope.c sv.c taint.c toke.c
            universal.c utf8.c util.c xsutils.c Base64.c Cwd.c Dumper.c
            HiRes.c IO.c Peek.c attrs.c poll.c stdio.c
            DynaLoader.c MD5.c Storable.c Parser.c specrand.c
            Hostname.c Opcode.c
           );

$need_math='yes';
$bench_flags = '-DPERL_CORE';

if ($^O =~ /MSWin32/i && !$ENV{'SPEC_NOT_REALLY_WINDOWS'}) {
  my @nt_sources=qw(win32.c win32io.c win32sck.c win32thread.c perllib.c);
  push @sources, map { "win32/$_" } @nt_sources;
  $bench_flags .= ' -I. -Iwin32 -DPERLDLL';
}



sub invoke {
    my ($me) = @_;
    my $name;
    my @rc;

    my $exe = $me->exe_file;
    my $iswindows = ($^O =~ /MSWin32/i);

    for (sort $me->input_files_base) {
        next if m#/#o;  # Don't descend into subdirs
	if (($name) = m/(.*)\.(t|pl)$/) {
	   if ($iswindows) {
	       next if ($name eq 'test');
	   } else {
	       next if ($2 eq 't');
	   }
	    if ($name =~ /splitmail|perfect|diffmail|checkspam/) {
		# Suck the data from a file
		open(IN, "<$name.in") || next;	# Definitely don't die here
		my $params = '';
		while(defined($params = <IN>)) {
		    next if ($params =~ /^(#|$)/o);
		    my @params = split(/\s+/, $params);
		    push @rc, { 'command' => $exe,
				'args' => [ '-I./lib', $_, @params ],
				'output' => "$name.".join('.', @params).'.out',
				'error' => "$name.".join('.', @params).'.err',
			      };
		}
	    } else {
              my $cmdhash = { 'command' => $exe,
			      'args'    => [ '-I.', '-I./lib', $_ ], 
                              'output'  => "$name.out",
                              'error'   => "$name.err" };
              if (-f "$name.in") {
                $cmdhash->{'input'} = "$name.in";
              }
              push @rc, $cmdhash;
	    }
	}
    }
    @rc;
}

# If you include perl.h, you also include all this stuff...
my @perl_h_deps = qw(perl.h
                     config.h spec_config.h embed.h handy.h iperlsys.h
                     perlio.h regexp.h sv.h util.h form.h gv.h pad.h cv.h
                     opnames.h op.h cop.h av.h hv.h mg.h scope.h warnings.h
                     utf8.h perly.h thread.h pp.h proto.h pp_proto.h opcode.h
                     embedvar.h intrpvar.h thrdvar.h perlvars.h patchlevel.h);
%srcdeps = (
  'win32thread.c' => [
    'win32.h',
    'win32iop.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'pp_ctl.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'scope.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'taint.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'HiRes.c' => [
    'const-c.inc',
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'gv.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'universal.c' => [
    'perliol.h',
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'hv.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'win32.c' => [
    'win32iop.h',
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'poll.c' => [
    'poll.h',
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'perlmain.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'Dumper.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'perlio.c' => [
    'perlsdio.h',
    'perliol.h',
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'miniperlmain.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'mg.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'Hostname.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'regcomp.c' => [
    'INTERN.h',
    'regcomp.h',
    'regnodes.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'op.c' => [
    'keywords.h',
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'perly.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'perlhost.h' => [
    'iperlsys.h',
    'vmem.h',
    'vdir.h',
    'perlio.h'
  ],
  'perlapi.c' => [
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'attrs.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'MD5.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'pp.c' => [
    'reentr.h',
    'keywords.h',
    'specrand.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'XSUB.h' => [
    'perlapi.h'
  ],
  'util.c' => [
    'specrand.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'globals.c' => [
    'INTERN.h',
    'perlapi.h',
    @perl_h_deps
  ],
  'Peek.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'pp_sys.c' => [
    'reentr.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'win32sck.c' => [
    'win32iop.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'hparser.c' => [
    'hctype.h',
    'tokenpos.h'
  ],
  'doop.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'pp_hot.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'numeric.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'Cwd.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'doio.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'pp_sort.c' => [
    'specrand.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'run.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'DynaLoader.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'config.h' => [
    'spec_config.h'
  ],
  'regexec.c' => [
    'regcomp.h',
    'regnodes.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'Base64.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'xsutils.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'win32io.c' => [
    'perliol.h',
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'pad.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'toke.c' => [
    'keywords.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'av.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'utf8.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'Parser.c' => [
    'hparser.h',
    'parser-util.c',
    'hparser.c',
    'hctype.h',
    'tokenpos.h',
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'Storable.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'pp_pack.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'dump.c' => [
    'regcomp.h',
    'regnodes.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'locale.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'perl.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'iperlsys.h' => [
    'perlio.h'
  ],
  'sv.c' => [
    'regcomp.h',
    'regnodes.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'IO.c' => [
    'poll.h',
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ],
  'deb.c' => [
    'EXTERN.h',
    @perl_h_deps
  ],
  'proto.h' => [
    'pp_proto.h'
  ],
  'perllib.c' => [
    'XSUB.h',
    'perlapi.h',
    'EXTERN.h',
    @perl_h_deps
  ]
);

1;
