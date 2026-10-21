#!/usr/bin/perl
# ms_limited.pl <seconds> <stdin file> <stdout file> <stderr file> <status file> <cwd> <command...>
#
# Runs the command in <cwd>, in its own process group, with the given
# files as stdin, stdout and stderr, and kills the whole group after
# <seconds>. The status file gets "<exit status> <signal> <timed out>".
use strict;
use warnings;
use Time::HiRes qw(alarm);

my ($limit, $in, $out, $err, $stat, $cwd, @cmd) = @ARGV;
my $pid = fork();
die "fork: $!\n" unless defined $pid;
if ($pid == 0) {
	setpgrp(0, 0);
	open(STDIN, '<', $in) or exit 127;
	open(STDOUT, '>', $out) or exit 127;
	open(STDERR, '>', $err) or exit 127;
	chdir($cwd) or exit 127;
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
exit 0;
