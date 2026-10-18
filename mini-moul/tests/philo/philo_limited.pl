#!/usr/bin/perl
# philo_limited.pl <seconds> <stdout file> <stderr file> <status file> <command...>
#
# Runs the command in its own process group with stdin from /dev/null and
# kills the whole group after <seconds> (fractions allowed). The status
# file gets "<exit status> <elapsed ms> <leftover> <signal> <timed out>":
# signal is the signal that killed the command (0 if none), timed out is 1
# when the time ran out; leftover is 1 when a process of the group was still alive 0.3s after the command
# itself ended (philo_bonus children that were not cleaned up).
use strict;
use warnings;
use Time::HiRes qw(time sleep alarm);

my ($limit, $out, $err, $stat, @cmd) = @ARGV;
my $start = time;
my $pid = fork();
die "fork: $!\n" unless defined $pid;
if ($pid == 0) {
	setpgrp(0, 0);
	open(STDIN, '<', '/dev/null');
	open(STDOUT, '>', $out) or exit 127;
	open(STDERR, '>', $err) or exit 127;
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
my $elapsed = int((time - $start) * 1000);
my $signal = $status & 127;
my $code = $status >> 8;
my $leftover = 0;
if (!$timed_out) {
	sleep(0.3);
	$leftover = 1 if kill(0, -$pid) > 0;
}
kill 'KILL', -$pid;
open(my $fh, '>', $stat) or die "$stat: $!\n";
print $fh "$code $elapsed $leftover $signal $timed_out\n";
close $fh;
exit 0;
