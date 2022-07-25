$benchnum  = '444';
$benchname = 'namd';
$exename   = 'namd';
$benchlang = 'CXX';
@base_exe  = ($exename);

$reltol = {'namd.out'   => undef, 'default' => undef};
$abstol = {'namd.out'   => 0.00001, 'default' => undef};

@sources=qw( Compute.C ComputeList.C ComputeNonbondedUtil.C
             LJTable.C Molecule.C Patch.C PatchList.C
             ResultSet.C SimParameters.C erf.C spec_namd.C );

$need_math = 'yes';

if ($^O =~ /MSWin32/i) {
  $bench_cxxflags = ' -DWIN32ERFC';
}

sub invoke {
    my ($me) = @_;
    my $name;
    my @rc;

    my $exe = $me->exe_file;
    my $iter = ($me->size eq 'ref') ? 38 : 1;

    for ($me->input_files_base) {
        if (($name) = m/(.*).input$/) {
            push (@rc, { 'command' => $exe, 
                         'args'    => [ "--input $name.input --iterations $iter --output $name.out "], 
                         'output'  => "$name.stdout",
                         'error'   => "$name.err",
                        });
        }
    }
    return @rc;
}


1;
