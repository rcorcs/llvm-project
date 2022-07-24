use IO::File;

$benchnum  = '447';
$benchname = 'dealII';
$exename   = 'dealII';
$benchlang = 'CXX';
@base_exe  = ($exename);

$reltol = {'dealII.out'   => undef, 'default' => undef}; # No reltol

$abstol = .0000001;

# This is the master list of sources.  Everything that should be built should
# be in this list.  Later we'll filter the ones that will be handled by
# pre_build.
@orig_sources=qw( auto_derivative_function.cc block_sparse_matrix.cc
     block_sparse_matrix_ez.cc block_sparsity_pattern.cc block_vector.cc
     compressed_sparsity_pattern.cc                      data_out.cc
     data_out_base.cc data_out_faces.cc data_out_rotation.cc
     data_out_stack.cc derivative_approximation.cc dof_accessor.cc
     dof_constraints.cc dof_handler.cc dof_levels.cc
     dof_renumbering.cc dof_tools.cc error_estimator.cc
     exceptions.cc fe.cc fe_data.cc
     fe_dgp.cc fe_dgp_1d.cc fe_dgp_2d.cc
     fe_dgp_3d.cc fe_dgp_nonparametric.cc fe_dgq.cc
     fe_dgq_1d.cc fe_dgq_2d.cc fe_dgq_3d.cc
     fe_nedelec.cc fe_nedelec_1d.cc fe_nedelec_2d.cc
     fe_nedelec_3d.cc fe_q.cc fe_q_1d.cc
     fe_q_2d.cc fe_q_3d.cc fe_q_hierarchical.cc
     fe_raviart_thomas.cc fe_system.cc fe_tools.cc
     fe_values.cc filtered_matrix.cc full_matrix.double.cc
     full_matrix.float.cc function.cc function_derivative.cc
     function_lib.cc function_lib_cutoff.cc function_time.cc
     geometry_info.cc grid_generator.cc grid_in.cc
     grid_out.all_dimensions.cc grid_out.cc grid_refinement.cc
     grid_reordering.cc histogram.cc
     intergrid_map.cc job_identifier.cc log.cc
     mapping.cc mapping_c1.cc mapping_cartesian.cc
     mapping_q.cc mapping_q1.cc mapping_q1_eulerian.cc
     matrices.all_dimensions.cc matrices.cc matrix_lib.cc
     matrix_out.cc memory_consumption.cc mg_base.cc
     mg_dof_accessor.cc mg_dof_handler.cc mg_dof_tools.cc
     mg_smoother.cc mg_transfer_block.cc mg_transfer_prebuilt.cc
     mg_transfer_block.all_dimensions.cc
     multigrid.all_dimensions.cc multithread_info.cc parameter_handler.cc
     persistent_tria.cc polynomial.cc polynomial_space.cc
     programid.cc
     quadrature.cc quadrature_lib.cc 
     solution_transfer.cc solver_control.cc 
     sparse_matrix.double.cc
     sparse_matrix.float.cc sparse_matrix_ez.double.cc sparse_matrix_ez.float.cc
     sparsity_pattern.cc
     step-14.cc subscriptor.cc swappable_vector.cc
     tensor.cc 
     tensor_product_polynomials.cc 
     tria.all_dimensions.cc tria.cc
     tria_accessor.cc tria_boundary.cc tria_boundary_lib.cc
     vector.cc vector.long_double.cc vectors.all_dimensions.cc
  fe_dgp_monomial.cc fe_poly.cc 
  polynomials_bdm.cc
  polynomials_p.cc fe_dgp_monomial.cc fe_poly.cc
  polynomials_bdm.cc polynomials_p.cc
        vectors.cc);

# These are the files which will need to be built with different values for
# deal_II_dimension.
@dimensioned_files = qw(
  data_out.cc
  data_out_faces.cc
  data_out_rotation.cc
  data_out_stack.cc
  derivative_approximation.cc
  dof_accessor.cc
  dof_handler.cc
  dof_renumbering.cc
  dof_tools.cc
  error_estimator.cc
  fe.cc
  fe_dgp.cc
  fe_dgp_1d.cc
  fe_dgp_2d.cc
  fe_dgp_3d.cc
  fe_dgp_monomial.cc
  fe_dgp_nonparametric.cc
  fe_dgq.cc
  fe_dgq_1d.cc
  fe_dgq_2d.cc
  fe_dgq_3d.cc
  fe_nedelec.cc
  fe_nedelec_1d.cc
  fe_nedelec_2d.cc
  fe_nedelec_3d.cc
  fe_poly.cc
  fe_q.cc
  fe_q_1d.cc
  fe_q_2d.cc
  fe_q_3d.cc
  fe_q_hierarchical.cc
  fe_raviart_thomas.cc
  fe_system.cc
  fe_tools.cc
  fe_values.cc
  grid_generator.cc
  grid_in.cc
  grid_out.cc
  grid_refinement.cc
  grid_reordering.cc
  intergrid_map.cc
  mapping.cc
  mapping_c1.cc
  mapping_cartesian.cc
  mapping_q.cc
  mapping_q1.cc
  mapping_q1_eulerian.cc
  matrices.cc
  mg_dof_accessor.cc
  mg_dof_handler.cc
  mg_dof_tools.cc
  mg_smoother.cc
  mg_transfer_block.cc
  mg_transfer_prebuilt.cc
  persistent_tria.cc
  solution_transfer.cc
  tria.cc
  tria_accessor.cc
  tria_boundary.cc
  tria_boundary_lib.cc
  vectors.cc
);

@sources = @orig_sources;       # For now, but pre_build might change it

$bench_cxxflags = '-Iinclude -DBOOST_DISABLE_THREADS';  # pre_build might change it

sub invoke {
    my ($me) = @_;
    my $name = $me->name;

    my %iter = ( 'test'  => 8,
                 'train' => 10,
                 'ref'   => 23 );

    return ({ 'command' => $me->exe_file, 
		 'args'    => [ $iter{$me->size} ], 
		 'output'  => "log",
		 'error'   => "$name.err",
		});
}

sub pre_build {
  my ($me, $path, $isbuild) = @_;
  my @sources = ();

  # This setup happens after the sources have been copied and before any
  # src.alts are applied.  And (very important!) before Makefiles are written.

  # Remove the BENCH_CXXFLAGS junk (if any) that might have been added earlier.
  $me->{'BENCH_CXXFLAGS'} =~ s/ -Ddeal_II_dimension=\d+//;

  if (::istrue($me->accessor_nowarn('explicit_dimensions'))) {
      if ($isbuild) {
          # It's impossible to munge a bunch of files that aren't there.
          # If $isbuild is false, then we're just here to fix up BENCH_CXXFLAGS
          # anyway.

          # Make the real list of sources
          my %sources = map { $_ => 1 } @orig_sources;
          foreach my $dim_src (@dimensioned_files) {
            delete $sources{$dim_src};
            for(my $i = 1; $i <= 3; $i++) {
              $sources{"dim${i}_$dim_src"} = 1;
            }
          }
          foreach my $src (sort keys %sources) {
            my $name = $src;
            if ($name =~ s/([\/\\]?)dim([123])_/$1/o) {
              my $dim = $2;
              my $ifh = new IO::File '<'.::jp($path, $name);
              if (!defined($ifh)) {
                ::Log(0, "ERROR: pre-build-setup: could not open $name for reading: $!\n");
                return 1;
              }
              my $ofh = new IO::File '>'.::jp($path, $src);
              if (!defined($ofh)) {
                $ifh->close();
                ::Log(0, "ERROR: pre-build-setup: could not open $src for writing: $!\n");
                return 1;
              }
              $ofh->print("#define deal_II_dimension $dim\n\n");
              $ofh->print(join('', <$ifh>));
              $ifh->close();
              $ofh->close();
            }
            push @sources, $src;
          }
      }
  } else {
    # Don't do triple build, so no need to munge the sources list
    @sources = @orig_sources;

    # Don't forget to specify the dimension
    $me->{'BENCH_CXXFLAGS'} .= ' -Ddeal_II_dimension=3';
  }

  # Look!  This is *BAD*!  $me is supposed to be an opaque object!
  $me->{'sources'} = \@sources;

  return 0;
}

1;

