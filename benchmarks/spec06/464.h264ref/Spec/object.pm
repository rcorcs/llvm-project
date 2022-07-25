$benchnum  = '464';
$benchname = 'h264ref';
$exename   = 'h264ref';
$benchlang = 'C';

$floatcompare = 1;
$compwhite = 1;
$binary = { (map { $_ => 1 } qw(foreman_ref_baseline_leakybucketparam.cfg
                                 foreman_ref_main_leakybucketparam.cfg
                                 sss_main_leakybucketparam.cfg
                                 foreman_test_baseline_leakybucketparam.cfg
                                 foreman_train_baseline_leakybucketparam.cfg)),
            'default' => undef
          };

@sources = (qw(
                annexb.c
                biariencode.c
                block.c
                cabac.c
                configfile.c
                context_ini.c
                decoder.c
                explicit_gop.c
                fast_me.c
                filehandle.c
                fmo.c
                header.c
                image.c
                intrarefresh.c
                leaky_bucket.c
                lencod.c
                loopFilter.c
                macroblock.c
                mb_access.c
                mbuffer.c
                memalloc.c
                mv-search.c
                nal.c
                nalu.c
                nalucommon.c
                output.c
                parset.c
                parsetcommon.c
                q_matrix.c
                q_offsets.c
                ratectl.c
                rdopt.c
                rdopt_coding_state.c
                rdpicdecision.c
                refbuf.c
                rtp.c
                sei.c
                slice.c
                transform8x8.c
                vlc.c
                weighted_prediction.c
		specrand.c
	));
@base_exe = ($exename);

$need_math = 1;

sub invoke {
    my ($me) = @_;
    my $name;
    my @rc;

    my $exe = $me->exe_file;
    for my $file ($me->input_files_base) {
        if (($leading, $level) = ($file =~ m/(.*)encoder_(.*)\.cfg$/)) {
            push @rc, ({ 'command' => $exe,
                         'args'    => [ '-d', "${leading}encoder_$level.cfg" ],
                         'output'  => "${leading}${level}_encodelog.out",
                         'error'   => "${leading}${level}_encodelog.err",
                        });
        }
    }
    return @rc;
}

1;
