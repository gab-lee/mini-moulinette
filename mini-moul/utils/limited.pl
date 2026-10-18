#!/usr/bin/perl
# limited.pl <seconds> <status file> <command...>
#
# Runs the command (stdin, stdout and stderr are inherited) in its own
# process group and kills the whole group after <seconds>. The status file
# gets "<exit status> <signal> <timed out>": a program that exits with 255 is
# told apart from one killed by a signal. The exit status of limited.pl is
# the command's, 128 plus the signal, or 142 on a timeout. perl, not
# timeout(1), because macOS has no timeout(1).
use strict;
use warnings;
use Time::HiRes qw(alarm);

my ($limit, $stat, @cmd) = @ARGV;
my $pid = fork();
die "fork: $!\n" unless defined $pid;
if ($pid == 0) {
	setpgrp(0, 0);
	exec { $cmd[0] } @cmd;
	exit 127;
}
my $timed_out = 0;
$SIG{ALRM} = sub { $timed_out = 1; kill 'KILL', -$pid; };
$SIG{$_} = sub { kill 'KILL', -$pid; exit 130; } for qw(INT TERM HUP);
alarm($limit);
while (waitpid($pid, 0) == -1 && $!{EINTR}) { }
my $status = $?;
alarm(0);
kill 'KILL', -$pid;
my $signal = $status & 127;
my $code = $status >> 8;
open(my $fh, '>', $stat) or die "$stat: $!\n";
print $fh "$code $signal $timed_out\n";
close $fh;
exit($timed_out ? 142 : $signal ? 128 + $signal : $code);
