$benchnum  = '453';
$benchname = 'povray';
$exename   = 'povray';
$benchlang = 'CXX';
$need_math = 'yes';
@base_exe  = ($exename);

$reltol = { 'default' => 0.00005,
            'test'    => 0.0002,
            'train'   => 0.00005,
          };

$abstol   = 0.0;

$binary  = {'SPEC-benchmark.tga'   => 1,  'default' => undef};

$skiptol = {'SPEC-benchmark.tga'   => 50, 'default' => undef};

@sources=qw(
atmosph.cpp
bbox.cpp
bcyl.cpp
bezier.cpp
blob.cpp
boxes.cpp
bsphere.cpp
camera.cpp
chi2.cpp
colour.cpp
colutils.cpp
cones.cpp
csg.cpp
defaultplatformbase.cpp
defaultrenderfrontend.cpp
discs.cpp
express.cpp
fileinputoutput.cpp
fncode.cpp
fnintern.cpp
fnpovfpu.cpp
fnsyntax.cpp
fpmetric.cpp
fractal.cpp
function.cpp
hcmplx.cpp
hfield.cpp
histogra.cpp
iff.cpp
image.cpp
interior.cpp
isosurf.cpp
lathe.cpp
lbuffer.cpp
lightgrp.cpp
lighting.cpp
mathutil.cpp
matrices.cpp
media.cpp
mesh.cpp
messageoutput.cpp
normal.cpp
objects.cpp
octree.cpp
optout.cpp
parse.cpp
parsestr.cpp
parstxtr.cpp
pattern.cpp
pgm.cpp
photons.cpp
pigment.cpp
planes.cpp
point.cpp
poly.cpp
polygon.cpp
polysolv.cpp
pov_mem.cpp
pov_util.cpp
povms.cpp
povmscpp.cpp
povmsend.cpp
povmsrec.cpp
povray.cpp
ppm.cpp
prism.cpp
processoptions.cpp
processrenderoptions.cpp
quadrics.cpp
quatern.cpp
rad_data.cpp
radiosit.cpp
ray.cpp
rendctrl.cpp
render.cpp
renderfrontend.cpp
renderio.cpp
sor.cpp
specrand.cpp
spec_qsort.cpp
spheres.cpp
sphsweep.cpp
splines.cpp
statspov.cpp
stringutilities.cpp
super.cpp
targa.cpp
textstream.cpp
textstreambuffer.cpp
texture.cpp
tokenize.cpp
torus.cpp
triangle.cpp
truetype.cpp
txttest.cpp
userdisp.cpp
userio.cpp
vbuffer.cpp
vlbuffer.cpp
warps.cpp
 );

sub invoke {
    my ($me) = @_;
    my @rc;

    for ($me->input_files_base) {
        if (($name) = m/(.*).ini$/) {
            push (@rc, { 'command' => $me->exe_file,
                         'args'    => [ $_ ],
                         'output'  => "$name.stdout",
                         'error'   => "$name.stderr",
                        });
        }
    }
    return @rc; 
}
1;    
