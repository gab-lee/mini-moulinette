#!/usr/bin/perl
# ms_filter.pl calibrate <dir>
# ms_filter.pl clean <calibration file> <stream: out|err> <input lines file> < raw output
#
# Fed from a pipe, a readline-based shell still prints its prompt and the
# line it reads; some also print "exit" at the end of the input. The
# prompt differs from shell to shell, so "calibrate" learns it from five
# runs of the shell (files <dir>/{true,colon,empty,exit,here}.{out,err}) and
# "clean" removes it from the raw output of a test, leaving what the
# commands printed.
use strict;
use warnings;
use Data::Dumper;

sub slurp
{
	my ($file) = @_;
	open(my $fh, '<', $file) or return '';
	local $/;
	my $text = <$fh>;
	close $fh;
	return defined $text ? $text : '';
}

sub calibrate_stream
{
	my ($true, $empty, $exit, $here) = @_;
	my %c = (P => '', H => '', echo => 0, Neof => '', Nexit => '');
	my $e = $empty;
	if ($e =~ /(\n?exit\n?)$/) {
		$c{Neof} = $1;
		$e = substr($e, 0, length($e) - length($c{Neof}));
	}
	$c{P} = $e;
	$c{echo} = ($true =~ /^\Q$c{P}\Etrue\n/) ? 1 : 0;
	my $x = $exit;
	$x =~ s/^\Q$c{P}\E//;
	$x =~ s/^exit\n// if $c{echo};
	$c{Nexit} = $x;
	my $h = $here;
	$h =~ s/^\Q$c{P}\E//;
	if ($c{echo}) {
		$c{H} = $1 if $h =~ /^cat << MSEOF\n(.*?)MSL1\n/s;
	} elsif ($h =~ /^(.*?)MSL1\n/s) {
		my $r = $1;
		my $half = length($r) / 2;
		$c{H} = substr($r, 0, $half) if length($r) > 0 && length($r) % 2 == 0
			&& substr($r, 0, $half) eq substr($r, $half);
	}
	return \%c;
}

sub strip_suffix
{
	my ($text, $suffix) = @_;
	return $text if $suffix eq '' || length($text) < length($suffix);
	return substr($text, 0, length($text) - length($suffix))
		if substr($text, -length($suffix)) eq $suffix;
	return $text;
}

sub clean
{
	my ($raw, $c, $lines) = @_;
	my ($P, $H) = ($c->{P}, $c->{H});
	my $out = '';
	if (!$c->{echo}) {
		$raw =~ s/\Q$H\E//g if length $H;
		$raw =~ s/\Q$P\E//g if length $P;
		$out = $raw;
	} else {
		my $pos = 0;
		foreach my $l (@$lines) {
			my $a = index($raw, $P . $l . "\n", $pos);
			my $b = length($H) ? index($raw, $H . $l . "\n", $pos) : -1;
			my ($idx, $len);
			if ($a >= 0 && ($b < 0 || $a <= $b)) {
				($idx, $len) = ($a, length($P . $l . "\n"));
			} elsif ($b >= 0) {
				($idx, $len) = ($b, length($H . $l . "\n"));
			} else {
				last;
			}
			$out .= substr($raw, $pos, $idx - $pos);
			$pos = $idx + $len;
		}
		$out .= substr($raw, $pos);
	}
	# What the shell prints when it stops: a last prompt and the end of
	# input, or the exit builtin
	my $stripped = strip_suffix($out, $P . $c->{Neof});
	if ($stripped eq $out) {
		$stripped = strip_suffix($out, $c->{Nexit});
		$stripped = strip_suffix($out, $c->{Neof}) if $stripped eq $out;
		$stripped = strip_suffix($out, $P) if $stripped eq $out;
	}
	return $stripped;
}

my $mode = shift @ARGV // '';
if ($mode eq 'calibrate') {
	my ($dir) = @ARGV;
	my %cal;
	foreach my $s ('out', 'err') {
		$cal{$s} = calibrate_stream(map { slurp("$dir/$_.$s") } qw(true empty exit here));
	}
	local $Data::Dumper::Terse = 1;
	local $Data::Dumper::Indent = 0;
	print Dumper(\%cal), "\n";
} elsif ($mode eq 'clean') {
	my ($calfile, $stream, $inputfile) = @ARGV;
	my $cal = eval slurp($calfile) or die "bad calibration file\n";
	my @lines = split /\n/, slurp($inputfile), -1;
	pop @lines if @lines && $lines[-1] eq '';
	local $/;
	my $raw = <STDIN>;
	$raw = '' unless defined $raw;
	print clean($raw, $cal->{$stream}, \@lines);
} else {
	die "usage: ms_filter.pl calibrate|clean ...\n";
}
