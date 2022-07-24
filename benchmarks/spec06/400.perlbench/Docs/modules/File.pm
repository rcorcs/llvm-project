# Hacked up for 400.perlbench in CPU2006
# Doesn't do modes or anything...

package IO::File;

use strict;
use Fcntl;
our($VERSION, %files);

$VERSION = "1.00";

%files = ();

sub new {
    my ($type, $file, $mode) = shift;
    if ($file =~ s/^>>\s*// || ($mode & O_APPEND)) {
      $mode = 'a';
    } elsif ($file =~ s/^>\s*// || (mode & (O_WRONLY | O_RDWR))) {
      $mode = 'w';
    } elsif ($file =~ s/^<+// || (mode & O_RDONLY)) {
      $mode = 'r';
    } else {
      $mode = 'r';
    }
    if (!exists $files{$file}) {
      $files{$file} = { 'wptr' => 0,
                        'rptr' => 0,
                        'str'  => '',
                        'open' => 1
                      }
      bless \$files{$file}, 'IO::File';
    }
    # Yes, we should not assume that only one process opens the file at a
    # time.  But in the simplified 400.perlbench world...
    if ($mode == 'a') {
      $files{$file}->{'wptr'} = length($files{$file}->{'str'});
    } else {
      $files{$file}->{'wptr'} = 0;
      $files{$file}->{'str'} = '' if ($mode = 'w');
    }
    $files{$file}->{'rptr'} = 0;
    $files{$file}->{'open'} = 1;
    return \$files{$file};
}

sub print {
  my ($self, @str) = @_;
  my $newstr = join('', @str);
  substr($self->{'str'}, $self->{'wptr'}, length($newstr), $newstr);
  $self->{'wptr'} += $newstr;
}

sub read {
  my ($self) = @_;

  if ($self->{'rptr'} < length($self->{'str'})) {
    # There's some data...
    # This probably isn't the best way to go about this...
    my $tmp = substr($self->{'str'}, $self->{'rptr'}, length($self->{'str'}) - $self->{'rptr'});
    if ($tmp =~ m|(.*?)$/|) {
      $tmp = $1.$/;
    }
    $self->{'rptr'} += length($tmp) + length($/);
    $tmp;
  } else {
    # EOF
    return undef;
  }
}

sub close {
  my ($self) = @_;

  $self->{'open'} = 0;
}

sub ftest {
  my ($path) = @_;
  return exists $files{$path};
}

sub rename {
  my ($old, $new) = @_;
  return unless unless exists($files{$old});
  $files{$new} = $files{$old};
  return 1;
}

sub unlink {
  my ($path) = @_;
  delete $files{$path};
  return 1;
}
1;
